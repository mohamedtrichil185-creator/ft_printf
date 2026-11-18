/*
** test_all.c - full ft_printf tester (all specifiers, all flags)
**
** HOW IT WORKS
**   Every test runs the real printf and your ft_printf with the same
**   arguments, then compares the text (byte by byte) and the return value.
**   Each test prints one line: PASS or FAIL.
**   On FAIL you see what printf gave (expected) and what yours gave.
**   Your ft_printf runs in a child process: a segfault = FAIL, not a stop.
**
** EACH TEST BELONGS TO A SPECIFIER AND A LEVEL
**   specifiers : c  s  p  d/i  u  x  X  mix     (the %% conversion is NOT tested)
**   levels     : MANDATORY  (no flags, required by the subject)
**                BONUS      (flags - 0 . # + space and width)
**                OPTIONAL   (undefined in C, glibc may differ: ignore it)
**
** At the end, a TABLE shows, for each specifier, how many tests pass.
** The first line with KO in the table is the specifier to fix first.
**
** RUN (with run_all.sh)
**   sh run_all.sh            all tests
**   sh run_all.sh -q         only the FAIL
**   sh run_all.sh d          only %d and %i       (c s p d u x X mix)
**   sh run_all.sh -q x       only the FAIL of %x
**   (option -m : run only the MANDATORY tests, no flags)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include "ft_printf.h"

#define BUFSZ 4096
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YEL   "\033[33m"
#define BLUE  "\033[36m"
#define GREY  "\033[90m"
#define END   "\033[0m"

int main(int ac, char **av);

enum e_spec { S_C, S_S, S_P, S_D, S_U, S_X, S_XX, S_PCT, S_MIX, S_COUNT };
enum e_kind { K_MAND, K_BONUS, K_OPT, K_COUNT };

static const char   *g_spec_name[S_COUNT] =
    {"c", "s", "p", "d / i", "u", "x", "X", "%", "mix"};
static const char   *g_kind_name[K_COUNT] = {"MANDATORY", "BONUS", "OPTIONAL"};

typedef struct s_res
{
    int     ret;
    int     len;
    char    buf[BUFSZ];
}   t_res;

static int  g_stat[S_COUNT][K_COUNT][2];   /* [spec][kind][0=pass, 1=fail] */
static int  g_spec;
static int  g_kind;
static int  g_quiet;
static int  g_mand_only;
static int  g_saved;
static int  g_fd;
static char *g_filter;

static int      g_x = 42;
static char     g_c = 'a';
static char     *g_heap;
static char     *g_null = NULL;
static void     *g_nullp = NULL;
static char     g_long[300];

/* ------------------------------------------------------------------ */
/*  capture of stdout                                                  */
/* ------------------------------------------------------------------ */

static void start_capture(void)
{
    fflush(stdout);
    g_saved = dup(1);
    g_fd = open("/tmp/ft_capture", O_RDWR | O_CREAT | O_TRUNC, 0644);
    dup2(g_fd, 1);
}

static int  end_capture(char *buf)
{
    ssize_t n;

    fflush(stdout);
    dup2(g_saved, 1);
    close(g_saved);
    lseek(g_fd, 0, SEEK_SET);
    n = read(g_fd, buf, BUFSZ - 1);
    if (n < 0)
        n = 0;
    buf[n] = '\0';
    close(g_fd);
    return ((int)n);
}

/* ------------------------------------------------------------------ */
/*  report                                                             */
/* ------------------------------------------------------------------ */

static void show(const char *label, const char *buf, int len, int ret)
{
    int i;

    printf("        %s [", label);
    i = 0;
    while (i < len)
    {
        if (buf[i] == '\0')
            printf("\\0");
        else if (buf[i] == '\n')
            printf("\\n");
        else
            putchar(buf[i]);
        i++;
    }
    printf("]  return=%d\n", ret);
}


/* ------------------------------------------------------------------ */
/*  explain the difference between printf and ft_printf                */
/* ------------------------------------------------------------------ */

#define MAX_FAILED 1000
static const char       *g_fdesc[MAX_FAILED];
static unsigned char    g_fspec[MAX_FAILED];
static unsigned char    g_fkind[MAX_FAILED];
static int              g_nfailed;

static int  vis_width(char c)
{
    return ((c == '\0' || c == '\n') ? 2 : 1);
}

static void put_char_desc(unsigned char c)
{
    if (c == '\0')
        printf("'\\0'");
    else if (c == '\n')
        printf("'\\n'");
    else if (c >= 32 && c < 127)
        printf("'%c'", c);
    else
        printf("0x%02x", c);
}

static void explain(t_res *e, t_res *g)
{
    int i;
    int j;
    int col;
    int n;

    n = (e->len < g->len) ? e->len : g->len;
    i = 0;
    while (i < n && e->buf[i] == g->buf[i])
        i++;
    if (i < n || e->len != g->len)
    {
        col = 0;
        j = 0;
        while (j < i)
            col += vis_width(e->buf[j++]);
        printf("%*s^ first difference at position %d: ", 30 + col, "", i);
        if (i < e->len && i < g->len)
        {
            printf("expected ");
            put_char_desc((unsigned char)e->buf[i]);
            printf(" but yours has ");
            put_char_desc((unsigned char)g->buf[i]);
            printf("\n");
        }
        else if (g->len < e->len)
            printf("yours stops too early (%d char(s) missing)\n",
                e->len - g->len);
        else
            printf("yours has %d extra char(s)\n", g->len - e->len);
    }
    if (e->ret != g->ret)
        printf("        return value: expected %d, yours %d\n", e->ret, g->ret);
}

static void list_failed(void)
{
    int i;

    if (g_nfailed == 0)
        return ;
    printf("\n=====================================================\n");
    printf("  ALL FAILED TESTS (mandatory and bonus): %d\n", g_nfailed);
    printf("-----------------------------------------------------\n");
    i = 0;
    while (i < g_nfailed)
    {
        printf("  [%-5s] %-9s  %s\n", g_spec_name[g_fspec[i]],
            g_kind_name[g_fkind[i]], g_fdesc[i]);
        i++;
    }
}

static void report(const char *desc, t_res *e, t_res *g, int status)
{
    int ok;

    ok = (!WIFSIGNALED(status) && e->ret == g->ret && e->len == g->len
            && memcmp(e->buf, g->buf, e->len) == 0);
    g_stat[g_spec][g_kind][ok ? 0 : 1]++;
    if (ok)
    {
        if (!g_quiet)
            printf(GREEN "  PASS" END "  %s\n", desc);
        return ;
    }
    printf(RED "  FAIL" END "  %s\n", desc);
    if (g_kind != K_OPT && g_nfailed < MAX_FAILED)
    {
        g_fdesc[g_nfailed] = desc;
        g_fspec[g_nfailed] = (unsigned char)g_spec;
        g_fkind[g_nfailed] = (unsigned char)g_kind;
        g_nfailed++;
    }
    show("expected (printf)  :", e->buf, e->len, e->ret);
    if (WIFSIGNALED(status))
        printf("        ft_printf CRASHED (signal %d)\n", WTERMSIG(status));
    else
    {
        show("yours    (ft_printf):", g->buf, g->len, g->ret);
        explain(e, g);
    }
}

#define TEST(...) do { \
    if (g_mand_only && g_kind != K_MAND) \
        break ; \
    t_res   exp_; \
    t_res   got_; \
    int     pipe_[2]; \
    int     status_ = 0; \
    pid_t   pid_; \
    memset(&exp_, 0, sizeof(exp_)); \
    memset(&got_, 0, sizeof(got_)); \
    start_capture(); \
    exp_.ret = printf(__VA_ARGS__); \
    exp_.len = end_capture(exp_.buf); \
    pipe(pipe_); \
    fflush(stdout); \
    pid_ = fork(); \
    if (pid_ == 0) \
    { \
        close(pipe_[0]); \
        start_capture(); \
        got_.ret = ft_printf(__VA_ARGS__); \
        got_.len = end_capture(got_.buf); \
        write(pipe_[1], &got_, sizeof(got_)); \
        _exit(0); \
    } \
    close(pipe_[1]); \
    waitpid(pid_, &status_, 0); \
    if (!WIFSIGNALED(status_)) \
        read(pipe_[0], &got_, sizeof(got_)); \
    close(pipe_[0]); \
    report(#__VA_ARGS__, &exp_, &got_, status_); \
} while (0)

/* sec(): start a group of tests (spec + level + what it checks) */
static void sec(int spec, int kind, const char *text)
{
    const char  *color;

    g_spec = spec;
    g_kind = kind;
    if (g_mand_only && kind != K_MAND)
        return ;
    color = (kind == K_MAND) ? YEL : (kind == K_BONUS) ? BLUE : GREY;
    printf("\n%s== [%s] %s: %s ==" END "\n", color, g_spec_name[spec],
        g_kind_name[kind], text);
}

static int  want(const char *name)
{
    return (g_filter == NULL || strcmp(g_filter, name) == 0);
}

/* ------------------------------------------------------------------ */
/*  %c                                                                 */
/* ------------------------------------------------------------------ */

static void test_c(void)
{
    sec(S_C, K_MAND, "basic characters");
    TEST("%c", 'a');
    TEST("%c", 'Z');
    TEST("%c", ' ');
    TEST("%c", '0');
    TEST("%c", '%');
    TEST("%c", '\n');
    TEST("%c", 0);
    TEST("%c", 127);
    TEST("%c", 128);
    TEST("%c", 255);
    TEST("%c", 256);
    TEST("%c", -1);
    TEST("%c%c%c", 'a', 'b', 'c');
    TEST("a%cb", 'X');
    TEST("%c%c", 0, 'a');
    TEST("%c %c %c", 'a', 'b', 'c');

    sec(S_C, K_BONUS, "width (spaces on the left)");
    TEST("[%1c]", 'x');
    TEST("[%2c]", 'x');
    TEST("[%3c]", 'x');
    TEST("[%10c]", 'x');
    TEST("[%100c]", 'x');
    TEST("[%5c]", 0);
    TEST("[%5c]", 255);
    TEST("[%5c|%5c]", 'a', 'b');

    sec(S_C, K_BONUS, "flag - (spaces on the right)");
    TEST("[%-1c]", 'x');
    TEST("[%-3c]", 'x');
    TEST("[%-10c]", 'x');
    TEST("[%-5c]", 0);
    TEST("[%-5c]", 255);
    TEST("[%--5c]", 'x');
    TEST("[%-5c|%5c]", 'a', 'b');

    sec(S_C, K_OPT, "flags that %c ignores (glibc)");
    TEST("[%.c]", 'x');
    TEST("[%.3c]", 'x');
    TEST("[%5.3c]", 'x');
    TEST("[%+c]", 'x');
    TEST("[%+5c]", 'x');
    TEST("[% c]", 'x');
    TEST("[%#c]", 'x');
    TEST("[%05c]", 'x');
}

/* ------------------------------------------------------------------ */
/*  %s                                                                 */
/* ------------------------------------------------------------------ */

static void test_s(void)
{
    sec(S_S, K_MAND, "basic strings and NULL");
    TEST("%s", "hello");
    TEST("%s", "a");
    TEST("%s", "");
    TEST("%s", "hello world");
    TEST("%s", "100%");
    TEST("%s", "a\nb");
    TEST("%s", g_long);
    TEST("%s", g_null);
    TEST("%s%s%s", "a", "b", "c");
    TEST("%s %s", g_null, "ok");
    TEST("a%sb", "-");
    TEST("[%s]", "");

    sec(S_S, K_BONUS, "width (spaces on the left)");
    TEST("[%1s]", "ab");
    TEST("[%2s]", "ab");
    TEST("[%5s]", "ab");
    TEST("[%10s]", "hello");
    TEST("[%5s]", "hello world");
    TEST("[%5s]", "");
    TEST("[%5s]", g_null);
    TEST("[%6s]", g_null);
    TEST("[%10s]", g_null);

    sec(S_S, K_BONUS, "flag - (spaces on the right)");
    TEST("[%-1s]", "ab");
    TEST("[%-5s]", "ab");
    TEST("[%-10s]", "hello");
    TEST("[%-5s]", "hello world");
    TEST("[%-5s]", "");
    TEST("[%-6s]", g_null);
    TEST("[%-10s]", g_null);
    TEST("[%--5s]", "ab");

    sec(S_S, K_BONUS, "precision (max number of chars)");
    TEST("[%.0s]", "hello");
    TEST("[%.s]", "hello");
    TEST("[%.1s]", "hello");
    TEST("[%.3s]", "hello");
    TEST("[%.5s]", "hello");
    TEST("[%.10s]", "hello");
    TEST("[%.3s]", "");
    TEST("[%.3s]", "a");
    TEST("[%.100s]", g_long);

    sec(S_S, K_BONUS, "precision with NULL (glibc: < 6 -> empty)");
    TEST("[%.0s]", g_null);
    TEST("[%.3s]", g_null);
    TEST("[%.5s]", g_null);
    TEST("[%.6s]", g_null);
    TEST("[%.7s]", g_null);
    TEST("[%.10s]", g_null);

    sec(S_S, K_BONUS, "width + precision");
    TEST("[%5.2s]", "hello");
    TEST("[%-5.2s]", "hello");
    TEST("[%2.5s]", "hello");
    TEST("[%10.3s]", "hello");
    TEST("[%5.0s]", "hello");
    TEST("[%-5.s]", "hello");
    TEST("[%10.3s]", g_null);
    TEST("[%-10.6s]", g_null);
    TEST("[%3.6s]", g_null);

    sec(S_S, K_OPT, "flags that %s ignores (glibc)");
    TEST("[%05s]", "ab");
    TEST("[%+s]", "ab");
    TEST("[% s]", "ab");
    TEST("[%#s]", "ab");
    TEST("[%+5s]", "ab");
}

/* ------------------------------------------------------------------ */
/*  %p                                                                 */
/* ------------------------------------------------------------------ */

static void test_p(void)
{
    sec(S_P, K_MAND, "basic pointers and NULL");
    TEST("%p", &g_x);
    TEST("%p", &g_c);
    TEST("%p", g_heap);
    TEST("%p", (void *)main);
    TEST("%p", g_nullp);
    TEST("%p", (void *)1);
    TEST("%p", (void *)0xf);
    TEST("%p", (void *)0x10);
    TEST("%p", (void *)0xff);
    TEST("%p", (void *)0xdeadbeef);
    TEST("%p", (void *)0x7fffffffffff);
    TEST("%p", (void *)ULONG_MAX);
    TEST("%p", (void *)LONG_MAX);
    TEST("%p", (void *)LONG_MIN);
    TEST("%p", (void *)-1);
    TEST("%p %p", &g_x, g_nullp);
    TEST("ptr=[%p]!", &g_x);

    sec(S_P, K_BONUS, "width (spaces on the left)");
    TEST("[%1p]", (void *)1);
    TEST("[%3p]", (void *)1);
    TEST("[%10p]", (void *)1);
    TEST("[%10p]", (void *)0xff);
    TEST("[%20p]", &g_x);
    TEST("[%5p]", g_nullp);
    TEST("[%6p]", g_nullp);
    TEST("[%10p]", g_nullp);
    TEST("[%18p]", (void *)ULONG_MAX);
    TEST("[%25p]", (void *)ULONG_MAX);

    sec(S_P, K_BONUS, "flag - (spaces on the right)");
    TEST("[%-1p]", (void *)1);
    TEST("[%-10p]", (void *)1);
    TEST("[%-20p]", &g_x);
    TEST("[%-5p]", g_nullp);
    TEST("[%-10p]", g_nullp);
    TEST("[%-25p]", (void *)ULONG_MAX);
    TEST("[%--10p]", (void *)1);

    sec(S_P, K_OPT, "flags undefined for %p (glibc)");
    TEST("[%+p]", (void *)1);
    TEST("[% p]", (void *)1);
    TEST("[%#p]", (void *)1);
    TEST("[%010p]", (void *)1);
    TEST("[%.5p]", (void *)1);
    TEST("[%.0p]", g_nullp);
}

/* ------------------------------------------------------------------ */
/*  %d and %i                                                          */
/* ------------------------------------------------------------------ */

static void test_d(void)
{
    sec(S_D, K_MAND, "basic numbers");
    TEST("%d", 0);
    TEST("%d", 1);
    TEST("%d", -1);
    TEST("%d", 42);
    TEST("%d", -42);
    TEST("%d", 123456);
    TEST("%d", INT_MAX);
    TEST("%d", INT_MIN);
    TEST("%d", INT_MAX - 1);
    TEST("%d", INT_MIN + 1);
    TEST("%i", 0);
    TEST("%i", 42);
    TEST("%i", -42);
    TEST("%i", INT_MAX);
    TEST("%i", INT_MIN);
    TEST("%d%d%d", 1, -2, 3);
    TEST("%d %i", 1, 2);

    sec(S_D, K_BONUS, "width (spaces on the left)");
    TEST("[%5d]", 42);
    TEST("[%5d]", -42);
    TEST("[%5d]", 0);
    TEST("[%1d]", 42);
    TEST("[%2d]", 12345);
    TEST("[%12d]", INT_MIN);
    TEST("[%20d]", INT_MAX);
    TEST("[%5i]", 42);

    sec(S_D, K_BONUS, "flag - (spaces on the right)");
    TEST("[%-5d]", 42);
    TEST("[%-5d]", -42);
    TEST("[%-5d]", 0);
    TEST("[%-1d]", 42);
    TEST("[%-3d]", 12345);
    TEST("[%-12d]", INT_MIN);
    TEST("[%-5i]", 42);

    sec(S_D, K_BONUS, "flag 0 (zeros, the sign comes first)");
    TEST("[%05d]", 42);
    TEST("[%05d]", -42);
    TEST("[%05d]", 0);
    TEST("[%02d]", 12345);
    TEST("[%010d]", INT_MIN);
    TEST("[%010d]", INT_MAX);
    TEST("[%05i]", -42);

    sec(S_D, K_BONUS, "- and 0 together ('-' wins)");
    TEST("[%-05d]", 42);
    TEST("[%0-5d]", 42);
    TEST("[%-05d]", -42);

    sec(S_D, K_BONUS, "precision (min number of digits)");
    TEST("[%.3d]", 7);
    TEST("[%.3d]", -7);
    TEST("[%.3d]", 12345);
    TEST("[%.1d]", 0);
    TEST("[%.0d]", 0);
    TEST("[%.d]", 0);
    TEST("[%.d]", 5);
    TEST("[%.d]", -5);
    TEST("[%.15d]", INT_MIN);
    TEST("[%.3i]", 7);

    sec(S_D, K_BONUS, "width + precision");
    TEST("[%8.3d]", 7);
    TEST("[%8.3d]", -7);
    TEST("[%-8.3d]", 7);
    TEST("[%-8.3d]", -7);
    TEST("[%3.5d]", 42);
    TEST("[%5.0d]", 0);
    TEST("[%5.d]", 0);
    TEST("[%20.15d]", INT_MIN);
    TEST("[%-20.15d]", INT_MIN);
    TEST("[%30.25d]", INT_MIN);

    sec(S_D, K_BONUS, "0 + precision (the 0 is ignored)");
    TEST("[%08.3d]", 7);
    TEST("[%08.3d]", -7);
    TEST("[%05.3d]", 7);
    TEST("[%05.0d]", 0);
    TEST("[%010.5d]", INT_MIN);

    sec(S_D, K_BONUS, "flag +");
    TEST("[%+d]", 42);
    TEST("[%+d]", -42);
    TEST("[%+d]", 0);
    TEST("[%+5d]", 42);
    TEST("[%+5d]", -42);
    TEST("[%+-5d]", 42);
    TEST("[%+05d]", 42);
    TEST("[%+05d]", -42);
    TEST("[%+.3d]", 7);
    TEST("[%+8.3d]", 7);
    TEST("[%+-8.3d]", 7);
    TEST("[%+08.3d]", 7);
    TEST("[%+.0d]", 0);
    TEST("[%+.d]", 0);
    TEST("[%+d]", INT_MAX);
    TEST("[%+d]", INT_MIN);
    TEST("[%+i]", 42);

    sec(S_D, K_BONUS, "flag space");
    TEST("[% d]", 42);
    TEST("[% d]", -42);
    TEST("[% d]", 0);
    TEST("[% 5d]", 42);
    TEST("[% -5d]", 42);
    TEST("[% 05d]", 42);
    TEST("[% 05d]", -42);
    TEST("[% .3d]", 7);
    TEST("[% 8.3d]", 7);
    TEST("[% .0d]", 0);
    TEST("[% d]", INT_MAX);
    TEST("[% i]", 42);

    sec(S_D, K_BONUS, "+ and space together ('+' wins)");
    TEST("[%+ d]", 42);
    TEST("[% +d]", 42);
    TEST("[% +5d]", 42);
    TEST("[%+ 05d]", -42);
    TEST("[% +-8.3d]", 7);

    sec(S_D, K_OPT, "flag # on %d (glibc ignores it)");
    TEST("[%#d]", 42);
    TEST("[%#5d]", 42);
}

/* ------------------------------------------------------------------ */
/*  %u                                                                 */
/* ------------------------------------------------------------------ */

static void test_u(void)
{
    sec(S_U, K_MAND, "basic unsigned numbers");
    TEST("%u", 0);
    TEST("%u", 1);
    TEST("%u", 42);
    TEST("%u", INT_MAX);
    TEST("%u", 2147483648u);
    TEST("%u", UINT_MAX);
    TEST("%u", -1);
    TEST("%u", -42);
    TEST("%u %u", 1, 2);

    sec(S_U, K_BONUS, "width and flag -");
    TEST("[%5u]", 42);
    TEST("[%1u]", 42);
    TEST("[%12u]", UINT_MAX);
    TEST("[%-5u]", 42);
    TEST("[%-12u]", UINT_MAX);

    sec(S_U, K_BONUS, "flag 0");
    TEST("[%05u]", 42);
    TEST("[%05u]", 0);
    TEST("[%012u]", UINT_MAX);
    TEST("[%-05u]", 42);

    sec(S_U, K_BONUS, "precision, and width + precision");
    TEST("[%.3u]", 7);
    TEST("[%.0u]", 0);
    TEST("[%.u]", 0);
    TEST("[%.u]", 5);
    TEST("[%8.5u]", 42);
    TEST("[%-8.5u]", 42);
    TEST("[%5.0u]", 0);
    TEST("[%08.3u]", 7);

    sec(S_U, K_OPT, "flags + space # on %u (glibc ignores them)");
    TEST("[%+u]", 42);
    TEST("[% u]", 42);
    TEST("[%#u]", 42);
    TEST("[%+5u]", 42);
}

/* ------------------------------------------------------------------ */
/*  %x                                                                 */
/* ------------------------------------------------------------------ */

static void test_x(void)
{
    sec(S_X, K_MAND, "basic hexadecimal (lowercase)");
    TEST("%x", 0);
    TEST("%x", 1);
    TEST("%x", 15);
    TEST("%x", 16);
    TEST("%x", 255);
    TEST("%x", 0xabcdef);
    TEST("%x", UINT_MAX);
    TEST("%x", -1);
    TEST("%x", LONG_MAX);
    TEST("%x %x", 1, 255);

    sec(S_X, K_BONUS, "width and flag -");
    TEST("[%5x]", 255);
    TEST("[%1x]", 255);
    TEST("[%12x]", UINT_MAX);
    TEST("[%-5x]", 255);
    TEST("[%-12x]", UINT_MAX);

    sec(S_X, K_BONUS, "flag 0");
    TEST("[%05x]", 255);
    TEST("[%05x]", 0);
    TEST("[%012x]", UINT_MAX);
    TEST("[%-05x]", 255);

    sec(S_X, K_BONUS, "precision, and width + precision");
    TEST("[%.5x]", 255);
    TEST("[%.0x]", 0);
    TEST("[%.x]", 0);
    TEST("[%.1x]", 0);
    TEST("[%8.5x]", 255);
    TEST("[%-8.5x]", 255);
    TEST("[%5.0x]", 0);
    TEST("[%08.3x]", 255);

    sec(S_X, K_BONUS, "flag # (prefix 0x, not for 0)");
    TEST("[%#x]", 255);
    TEST("[%#x]", 0);
    TEST("[%#x]", 1);
    TEST("[%#8x]", 255);
    TEST("[%#-8x]", 255);
    TEST("[%#08x]", 255);
    TEST("[%#.5x]", 255);
    TEST("[%#10.5x]", 255);
    TEST("[%#-10.5x]", 255);
    TEST("[%#2x]", 255);
    TEST("[%#.0x]", 0);
    TEST("[%#x]", UINT_MAX);
    TEST("[%#20x]", UINT_MAX);

    sec(S_X, K_OPT, "flags + space on %x (glibc ignores them)");
    TEST("[%+x]", 255);
    TEST("[% x]", 255);
}

/* ------------------------------------------------------------------ */
/*  %X                                                                 */
/* ------------------------------------------------------------------ */

static void test_xx(void)
{
    sec(S_XX, K_MAND, "basic hexadecimal (UPPERCASE)");
    TEST("%X", 0);
    TEST("%X", 1);
    TEST("%X", 15);
    TEST("%X", 16);
    TEST("%X", 255);
    TEST("%X", 0xabcdef);
    TEST("%X", UINT_MAX);
    TEST("%X", -1);
    TEST("%X", LONG_MAX);
    TEST("%x%c%X", 255, 'z', 255);
    TEST("%X %X", 1, 255);

    sec(S_XX, K_BONUS, "width and flag -");
    TEST("[%5X]", 255);
    TEST("[%-5X]", 255);
    TEST("[%12X]", UINT_MAX);

    sec(S_XX, K_BONUS, "flag 0");
    TEST("[%05X]", 255);
    TEST("[%05X]", 0);
    TEST("[%-05X]", 255);

    sec(S_XX, K_BONUS, "precision, and width + precision");
    TEST("[%.5X]", 255);
    TEST("[%.0X]", 0);
    TEST("[%.X]", 0);
    TEST("[%8.5X]", 255);
    TEST("[%-8.5X]", 255);
    TEST("[%08.3X]", 255);

    sec(S_XX, K_BONUS, "flag # (prefix 0X, not for 0)");
    TEST("[%#X]", 255);
    TEST("[%#X]", 0);
    TEST("[%#8X]", 255);
    TEST("[%#-8X]", 255);
    TEST("[%#08X]", 255);
    TEST("[%#.5X]", 255);
    TEST("[%#10.5X]", 255);
    TEST("[%#X]", UINT_MAX);
}

/* ------------------------------------------------------------------ */
/*  mix: flags reset, many specifiers in one call                      */
/* ------------------------------------------------------------------ */

static void test_mix(void)
{
    sec(S_MIX, K_MAND, "several specifiers in one call");
    TEST("%c %s %d %i %u %x %X", 'a', "bc", -1, 2, 3, 255, 255);
    TEST("%c %s %p %d %u %x %X", 'a', "b", (void *)0xabc, 1, 2, 255, 255);
    TEST("text %d text %s text", 42, "str");
    TEST("%s%c%s", "ab", ':', "cd");
    TEST("%d%c%d", 1, '-', 2);
    TEST("%p %s %p", &g_x, "mid", g_nullp);

    sec(S_MIX, K_BONUS, "flags must be reset between two %");
    TEST("[%-5d] [%5d]", 1, 2);
    TEST("[%+d] [%d]", 1, 2);
    TEST("[%+5d] [%5d]", 1, 2);
    TEST("[%05d] [%d]", 1, 2);
    TEST("[%.3d] [%d]", 1, 2);
    TEST("[%-5c] [%c]", 'a', 'b');
    TEST("[%-5s] [%s]", "ab", "cd");
    TEST("[%.1s] [%s]", "ab", "cd");
    TEST("[%-10p] [%p]", (void *)1, (void *)2);
    TEST("[%#x] [%x]", 255, 255);
    TEST("[%-5d] [%5c]", 1, 'b');
    TEST("[%+d] [%c]", 1, 'b');
    TEST("[%.3d] [%c]", 1, 'b');

    sec(S_MIX, K_BONUS, "different flags on each specifier");
    TEST("[%-+8.3d|%08.3d|%#10x|% 5d]", 7, 7, 255, 42);
    TEST("[%5c|%-5c|%5s|%-5s]", 'a', 'b', "cd", "ef");
    TEST("[%05d|%-5d|%+5d|% 5d]", 42, 42, 42, 42);
    TEST("[%.3d|%.3u|%.3x|%.3s]", 1, 2, 3, "four");
    TEST("[%10s|%-10s|%.2s]", "abc", "abc", "abc");
    TEST("[%+d|%5c|% d]", 1, 'x', 2);
    TEST("[%5d|%-5d|%05d]", 1, 2, 3);
    TEST("[%10p|%-10p]", (void *)1, (void *)2);
    TEST("[%8x|%-8x|%#8x|%08x]", 255, 255, 255, 255);
}

/* ------------------------------------------------------------------ */
/*  summary table                                                      */
/* ------------------------------------------------------------------ */

static void cell(int spec, int kind)
{
    int pass;
    int fail;

    pass = g_stat[spec][kind][0];
    fail = g_stat[spec][kind][1];
    if (pass + fail == 0)
        printf(GREY "     -        " END);
    else if (fail == 0)
        printf(GREEN "%3d/%-3d OK    " END, pass, pass + fail);
    else
        printf(RED "%3d/%-3d KO    " END, pass, pass + fail);
}

static int  summary(void)
{
    int spec;
    int mand_fail;
    int bonus_fail;

    mand_fail = 0;
    bonus_fail = 0;
    printf("\n=====================================================\n");
    printf("  SPEC      MANDATORY     BONUS         OPTIONAL\n");
    printf("-----------------------------------------------------\n");
    spec = 0;
    while (spec < S_COUNT)
    {
        if (g_stat[spec][0][0] + g_stat[spec][0][1] + g_stat[spec][1][0]
            + g_stat[spec][1][1] + g_stat[spec][2][0] + g_stat[spec][2][1])
        {
            printf("  %-8s  ", g_spec_name[spec]);
            cell(spec, K_MAND);
            cell(spec, K_BONUS);
            cell(spec, K_OPT);
            printf("\n");
            mand_fail += g_stat[spec][K_MAND][1];
            bonus_fail += g_stat[spec][K_BONUS][1];
        }
        spec++;
    }
    printf("=====================================================\n");
    if (mand_fail == 0 && bonus_fail == 0 && g_mand_only)
        printf(GREEN "  Mandatory is correct." END "\n");
    else if (mand_fail == 0 && bonus_fail == 0)
        printf(GREEN "  Mandatory and bonus are correct." END "\n");
    else if (mand_fail)
        printf(RED "  Fix MANDATORY first (%d KO): the bonus is graded only "
            "if the mandatory is perfect." END "\n", mand_fail);
    else
        printf(YEL "  Mandatory is OK. %d KO left in the bonus." END "\n",
            bonus_fail);
    if (g_mand_only)
        printf(YEL "  Only the MANDATORY tests were run (option -m)." END "\n");
    printf("  OPTIONAL tests can be ignored.\n\n");
    return (mand_fail + bonus_fail);
}

int main(int ac, char **av)
{
    int i;
    int fails;

    memset(g_long, 'z', 299);
    g_long[299] = '\0';
    g_heap = malloc(16);
    i = 1;
    while (i < ac)
    {
        if (strcmp(av[i], "-q") == 0)
            g_quiet = 1;
        else if (strcmp(av[i], "-m") == 0)
            g_mand_only = 1;
        else
            g_filter = av[i];
        i++;
    }
    if (want("c"))
        test_c();
    if (want("s"))
        test_s();
    if (want("p"))
        test_p();
    if (want("d"))
        test_d();
    if (want("u"))
        test_u();
    if (want("x"))
        test_x();
    if (want("X"))
        test_xx();
    if (want("mix"))
        test_mix();
    list_failed();
    fails = summary();
    free(g_heap);
    return (fails != 0);
}
