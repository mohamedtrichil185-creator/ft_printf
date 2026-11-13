/*
** test_p.c - tests ONLY %p (mandatory + bonus flags)
**
** Every test prints one line:   PASS  or  FAIL   + the format used.
** On FAIL it also shows what printf gave (expected) and what yours gave.
** Your ft_printf runs in a child process: a segfault = FAIL, not a stop.
**
** Run with the script:   sh run_p.sh
** Only the failures:     sh run_p.sh -q
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
#define END   "\033[0m"

typedef struct s_res
{
    int     ret;
    int     len;
    char    buf[BUFSZ];
}   t_res;

static int  g_pass;
static int  g_fail;
static int  g_opt_pass;
static int  g_opt_fail;
static int  g_quiet;
static int  g_optional;
static int  g_saved;
static int  g_fd;

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

static void show(const char *label, const char *buf, int len, int ret)
{
    int i;

    printf("        %s [", label);
    i = 0;
    while (i < len)
    {
        if (buf[i] == '\0')
            printf("\\0");
        else
            putchar(buf[i]);
        i++;
    }
    printf("]  return=%d\n", ret);
}

static void report(const char *desc, t_res *e, t_res *g, int status)
{
    int ok;

    ok = (!WIFSIGNALED(status) && e->ret == g->ret && e->len == g->len
            && memcmp(e->buf, g->buf, e->len) == 0);
    if (ok)
    {
        if (g_optional)
            g_opt_pass++;
        else
            g_pass++;
        if (!g_quiet)
            printf(GREEN "  PASS" END "  %s\n", desc);
        return ;
    }
    if (g_optional)
        g_opt_fail++;
    else
        g_fail++;
    printf(RED "  FAIL" END "  %s\n", desc);
    show("expected (printf)  :", e->buf, e->len, e->ret);
    if (WIFSIGNALED(status))
        printf("        ft_printf CRASHED (signal %d)\n", WTERMSIG(status));
    else
        show("yours    (ft_printf):", g->buf, g->len, g->ret);
}

/* TEST("what I test", format, args...) */
#define TEST(desc, ...) do { \
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
    report(desc, &exp_, &got_, status_); \
} while (0)

static void title(const char *s)
{
    printf("\n" YEL "== %s ==" END "\n", s);
}

int main(int ac, char **av)
{
    int     x = 42;
    char    c = 'a';
    char    *heap;
    void    *null_ptr = NULL;

    g_quiet = (ac > 1 && strcmp(av[1], "-q") == 0);
    heap = malloc(16);

    title("1. BASIC %p");
    TEST("address of a local int", "%p", &x);
    TEST("address of a local char", "%p", &c);
    TEST("address from malloc", "%p", heap);
    TEST("address of main", "%p", (void *)main);
    TEST("address of a string literal", "%p", "hello");
    TEST("NULL pointer (Linux: (nil))", "%p", null_ptr);
    TEST("pointer 0x1", "%p", (void *)1);
    TEST("pointer 0xf", "%p", (void *)0xf);
    TEST("pointer 0x10", "%p", (void *)0x10);
    TEST("pointer 0xff", "%p", (void *)0xff);
    TEST("pointer 0x1000", "%p", (void *)0x1000);
    TEST("pointer 0xdeadbeef", "%p", (void *)0xdeadbeef);
    TEST("pointer 0x7fffffffffff", "%p", (void *)0x7fffffffffff);
    TEST("pointer ULONG_MAX (all f)", "%p", (void *)ULONG_MAX);
    TEST("pointer LONG_MAX", "%p", (void *)LONG_MAX);
    TEST("pointer LONG_MIN", "%p", (void *)LONG_MIN);
    TEST("pointer -1", "%p", (void *)-1);

    title("2. TEXT AROUND %p");
    TEST("text before and after", "ptr=[%p]!", &x);
    TEST("NULL with text", "ptr=[%p]!", null_ptr);
    TEST("with newline", "%p\n", &x);
    TEST("two pointers", "%p %p", &x, &c);
    TEST("pointer and NULL", "%p %p", &x, null_ptr);
    TEST("NULL and pointer", "%p %p", null_ptr, &x);
    TEST("same pointer twice", "%p|%p", &x, &x);
    TEST("three pointers", "%p%p%p", (void *)1, (void *)2, (void *)3);

    title("3. WIDTH (spaces on the left)");
    TEST("%1p, pointer 0x1", "[%1p]", (void *)1);
    TEST("%3p, pointer 0x1 (same size as 0x1)", "[%3p]", (void *)1);
    TEST("%4p, pointer 0x1", "[%4p]", (void *)1);
    TEST("%10p, pointer 0x1", "[%10p]", (void *)1);
    TEST("%10p, pointer 0xff", "[%10p]", (void *)0xff);
    TEST("%20p, local int", "[%20p]", &x);
    TEST("%30p, local int", "[%30p]", &x);
    TEST("%5p, NULL", "[%5p]", null_ptr);
    TEST("%6p, NULL (same size as (nil))", "[%6p]", null_ptr);
    TEST("%10p, NULL", "[%10p]", null_ptr);
    TEST("%20p, NULL", "[%20p]", null_ptr);
    TEST("%25p, ULONG_MAX", "[%25p]", (void *)ULONG_MAX);
    TEST("%18p, ULONG_MAX (exactly 18 chars)", "[%18p]", (void *)ULONG_MAX);
    TEST("%17p, ULONG_MAX (width too small)", "[%17p]", (void *)ULONG_MAX);
    TEST("two widths", "[%10p|%20p]", (void *)1, (void *)2);

    title("4. FLAG - (spaces on the right)");
    TEST("%-1p, pointer 0x1", "[%-1p]", (void *)1);
    TEST("%-3p, pointer 0x1", "[%-3p]", (void *)1);
    TEST("%-10p, pointer 0x1", "[%-10p]", (void *)1);
    TEST("%-10p, pointer 0xff", "[%-10p]", (void *)0xff);
    TEST("%-20p, local int", "[%-20p]", &x);
    TEST("%-30p, local int", "[%-30p]", &x);
    TEST("%-5p, NULL", "[%-5p]", null_ptr);
    TEST("%-6p, NULL", "[%-6p]", null_ptr);
    TEST("%-10p, NULL", "[%-10p]", null_ptr);
    TEST("%-20p, NULL", "[%-20p]", null_ptr);
    TEST("%-25p, ULONG_MAX", "[%-25p]", (void *)ULONG_MAX);
    TEST("%-17p, ULONG_MAX (width too small)", "[%-17p]", (void *)ULONG_MAX);
    TEST("%-10p then %10p", "[%-10p|%10p]", (void *)1, (void *)2);
    TEST("%10p then %-10p", "[%10p|%-10p]", (void *)1, (void *)2);
    TEST("repeated flag %--10p", "[%--10p]", (void *)1);

    title("5. FLAGS MUST BE RESET BETWEEN TWO %");
    TEST("%-10p then %p", "[%-10p] [%p]", (void *)1, (void *)2);
    TEST("%10p then %p", "[%10p] [%p]", (void *)1, (void *)2);
    TEST("%-10d then %p", "[%-10d] [%p]", 42, (void *)2);
    TEST("%+d then %p", "[%+d] [%p]", 42, (void *)2);
    TEST("%p then %-5c", "[%p] [%-5c]", (void *)2, 'x');

    title("6. MIXED WITH OTHER SPECIFIERS");
    TEST("%p %s %d", "%p %s %d", &x, "str", 42);
    TEST("%d %p %c", "%d %p %c", 42, &x, 'z');
    TEST("%x %p %X", "%x %p %X", 255, (void *)255, 255);
    TEST("%10p and %-10s", "[%10p|%-10s]", (void *)1, "ab");
    TEST("%%, %p, %%", "%%[%p]%%", (void *)1);
    TEST("all specifiers", "%c %s %p %d %i %u %x %X %%",
        'a', "b", (void *)0xabc, 1, 2, 3, 255, 255);

    title("7. OPTIONAL: undefined in C, glibc may differ (not required)");
    g_optional = 1;
    TEST("[optional] %+p", "[%+p]", (void *)1);
    TEST("[optional] %+p NULL", "[%+p]", null_ptr);
    TEST("[optional] % p", "[% p]", (void *)1);
    TEST("[optional] % p NULL", "[% p]", null_ptr);
    TEST("[optional] %#p", "[%#p]", (void *)1);
    TEST("[optional] %010p", "[%010p]", (void *)1);
    TEST("[optional] %010p NULL", "[%010p]", null_ptr);
    TEST("[optional] %.5p", "[%.5p]", (void *)1);
    TEST("[optional] %.0p NULL", "[%.0p]", null_ptr);
    TEST("[optional] %20.5p", "[%20.5p]", (void *)1);
    g_optional = 0;

    free(heap);

    printf("\n==============================================\n");
    printf("  REQUIRED : " GREEN "%d PASS" END "  /  " RED "%d FAIL" END "\n",
        g_pass, g_fail);
    printf("  OPTIONAL : %d pass  /  %d fail  (can be ignored)\n",
        g_opt_pass, g_opt_fail);
    if (g_fail == 0)
        printf(GREEN "  >>> %%p IS CORRECT <<<" END "\n");
    else
        printf(RED "  >>> %%p has %d problem(s), fix the first FAIL <<<" END
            "\n", g_fail);
    printf("==============================================\n");
    return (g_fail != 0);
}
