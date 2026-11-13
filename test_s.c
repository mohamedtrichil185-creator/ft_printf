/*
** test_s.c - tests ONLY %s (mandatory + bonus flags)
**
** Every test prints one line:   PASS  or  FAIL   + what is tested.
** On FAIL it also shows what printf gave (expected) and what yours gave.
** Your ft_printf runs in a child process: a segfault = FAIL, not a stop.
**
** Run with the script:   sh run_s.sh
** Only the failures:     sh run_s.sh -q
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
        else if (buf[i] == '\n')
            printf("\\n");
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
    char    *null_str = NULL;
    char    long_str[300];

    g_quiet = (ac > 1 && strcmp(av[1], "-q") == 0);
    memset(long_str, 'z', 299);
    long_str[299] = '\0';

    title("1. BASIC %s (mandatory)");
    TEST("simple word", "%s", "hello");
    TEST("one char", "%s", "a");
    TEST("empty string", "%s", "");
    TEST("string with spaces", "%s", "hello world");
    TEST("string with a newline inside", "%s", "a\nb");
    TEST("string with a percent inside", "%s", "100%");
    TEST("string with digits", "%s", "42 is the answer");
    TEST("string with symbols", "%s", "!@#$^&*()");
    TEST("string with a tab", "%s", "a\tb");
    TEST("long string (299 chars)", "%s", long_str);
    TEST("NULL string -> (null)", "%s", null_str);

    title("2. TEXT AROUND AND SEVERAL %s");
    TEST("text before and after", "before [%s] after", "mid");
    TEST("empty string between text", "[%s]", "");
    TEST("NULL between text", "[%s]", null_str);
    TEST("two strings", "%s %s", "ab", "cd");
    TEST("three strings stuck together", "%s%s%s", "a", "b", "c");
    TEST("empty, text, empty", "%s%s%s", "", "mid", "");
    TEST("NULL and a string", "%s %s", null_str, "ok");
    TEST("a string and NULL", "%s %s", "ok", null_str);
    TEST("two NULL", "%s|%s", null_str, null_str);
    TEST("same string twice", "%s|%s", "same", "same");
    TEST("with newline at the end", "%s\n", "hello");

    title("3. WIDTH (spaces on the left)");
    TEST("%1s, 'ab' (width too small)", "[%1s]", "ab");
    TEST("%2s, 'ab' (same size)", "[%2s]", "ab");
    TEST("%3s, 'ab'", "[%3s]", "ab");
    TEST("%5s, 'ab'", "[%5s]", "ab");
    TEST("%10s, 'ab'", "[%10s]", "ab");
    TEST("%20s, 'hello'", "[%20s]", "hello");
    TEST("%5s, 'hello world' (longer than width)", "[%5s]", "hello world");
    TEST("%5s, empty string", "[%5s]", "");
    TEST("%1s, empty string", "[%1s]", "");
    TEST("%0s is width 0 (no padding)", "[%0s]", "ab");
    TEST("%100s, 'x'", "[%100s]", "x");
    TEST("%5s, NULL (6 chars > 5)", "[%5s]", null_str);
    TEST("%6s, NULL (exactly 6)", "[%6s]", null_str);
    TEST("%7s, NULL", "[%7s]", null_str);
    TEST("%10s, NULL", "[%10s]", null_str);
    TEST("two widths", "[%5s|%8s]", "ab", "cd");

    title("4. FLAG - (spaces on the right)");
    TEST("%-1s, 'ab'", "[%-1s]", "ab");
    TEST("%-2s, 'ab'", "[%-2s]", "ab");
    TEST("%-3s, 'ab'", "[%-3s]", "ab");
    TEST("%-5s, 'ab'", "[%-5s]", "ab");
    TEST("%-10s, 'ab'", "[%-10s]", "ab");
    TEST("%-20s, 'hello'", "[%-20s]", "hello");
    TEST("%-5s, 'hello world' (longer)", "[%-5s]", "hello world");
    TEST("%-5s, empty string", "[%-5s]", "");
    TEST("%-100s, 'x'", "[%-100s]", "x");
    TEST("%-5s, NULL", "[%-5s]", null_str);
    TEST("%-6s, NULL", "[%-6s]", null_str);
    TEST("%-10s, NULL", "[%-10s]", null_str);
    TEST("%-5s then %5s", "[%-5s|%5s]", "ab", "cd");
    TEST("%5s then %-5s", "[%5s|%-5s]", "ab", "cd");
    TEST("repeated flag %--5s", "[%--5s]", "ab");

    title("5. PRECISION (max number of chars)");
    TEST("%.1s, 'hello'", "[%.1s]", "hello");
    TEST("%.2s, 'hello'", "[%.2s]", "hello");
    TEST("%.3s, 'hello'", "[%.3s]", "hello");
    TEST("%.5s, 'hello' (same size)", "[%.5s]", "hello");
    TEST("%.10s, 'hello' (bigger than string)", "[%.10s]", "hello");
    TEST("%.0s, 'hello' (nothing)", "[%.0s]", "hello");
    TEST("%.s, 'hello' (. alone = 0)", "[%.s]", "hello");
    TEST("%.3s, empty string", "[%.3s]", "");
    TEST("%.0s, empty string", "[%.0s]", "");
    TEST("%.3s, 1 char string", "[%.3s]", "a");
    TEST("%.100s, long string", "[%.100s]", long_str);
    TEST("%.s then text", "[%.s] text", "abc");
    TEST("two precisions", "[%.2s|%.3s]", "hello", "world");

    title("6. PRECISION WITH NULL (glibc rule: precision < 6 -> empty)");
    TEST("%.0s, NULL", "[%.0s]", null_str);
    TEST("%.s, NULL", "[%.s]", null_str);
    TEST("%.1s, NULL", "[%.1s]", null_str);
    TEST("%.3s, NULL", "[%.3s]", null_str);
    TEST("%.5s, NULL", "[%.5s]", null_str);
    TEST("%.6s, NULL (exactly 6 -> (null))", "[%.6s]", null_str);
    TEST("%.7s, NULL", "[%.7s]", null_str);
    TEST("%.10s, NULL", "[%.10s]", null_str);

    title("7. WIDTH + PRECISION");
    TEST("%5.2s, 'hello'", "[%5.2s]", "hello");
    TEST("%-5.2s, 'hello'", "[%-5.2s]", "hello");
    TEST("%2.5s, 'hello' (width smaller)", "[%2.5s]", "hello");
    TEST("%-2.5s, 'hello'", "[%-2.5s]", "hello");
    TEST("%10.3s, 'hello'", "[%10.3s]", "hello");
    TEST("%-10.3s, 'hello'", "[%-10.3s]", "hello");
    TEST("%10.10s, 'hello'", "[%10.10s]", "hello");
    TEST("%5.0s, 'hello' (only spaces)", "[%5.0s]", "hello");
    TEST("%-5.0s, 'hello'", "[%-5.0s]", "hello");
    TEST("%5.s, 'hello'", "[%5.s]", "hello");
    TEST("%-5.s, 'hello'", "[%-5.s]", "hello");
    TEST("%1.1s, 'hello'", "[%1.1s]", "hello");
    TEST("%10.3s, NULL (precision < 6)", "[%10.3s]", null_str);
    TEST("%-10.3s, NULL", "[%-10.3s]", null_str);
    TEST("%10.6s, NULL", "[%10.6s]", null_str);
    TEST("%-10.6s, NULL", "[%-10.6s]", null_str);
    TEST("%10.10s, NULL", "[%10.10s]", null_str);
    TEST("%3.6s, NULL", "[%3.6s]", null_str);

    title("8. FLAGS MUST BE RESET BETWEEN TWO %");
    TEST("%-5s then %5s", "[%-5s] [%5s]", "ab", "cd");
    TEST("%5s then %s", "[%5s] [%s]", "ab", "cd");
    TEST("%.1s then %s", "[%.1s] [%s]", "ab", "cd");
    TEST("%-5d then %s", "[%-5d] [%s]", 42, "cd");
    TEST("%+d then %s", "[%+d] [%s]", 42, "cd");
    TEST("%.3d then %s", "[%.3d] [%s]", 7, "cd");
    TEST("%s then %-5c", "[%s] [%-5c]", "ab", 'x');
    TEST("%.2s then %5s", "[%.2s] [%5s]", "hello", "ab");

    title("9. MIXED WITH OTHER SPECIFIERS");
    TEST("%s %c %d", "%s %c %d", "str", 'z', 42);
    TEST("%d %s %x", "%d %s %x", 42, "str", 255);
    TEST("%10s and %-10s and %.2s", "[%10s|%-10s|%.2s]", "ab", "cd", "efgh");
    TEST("%%, %s, %%", "%%[%s]%%", "ab");
    TEST("%s with %p", "%s %p", "ab", (void *)0x1);
    TEST("all specifiers", "%c %s %p %d %i %u %x %X %%",
        'a', "b", (void *)0xabc, 1, 2, 3, 255, 255);

    title("10. OPTIONAL: undefined in C, glibc may differ (not required)");
    g_optional = 1;
    TEST("[optional] %05s", "[%05s]", "ab");
    TEST("[optional] %-05s", "[%-05s]", "ab");
    TEST("[optional] %+s", "[%+s]", "ab");
    TEST("[optional] %+5s", "[%+5s]", "ab");
    TEST("[optional] % s", "[% s]", "ab");
    TEST("[optional] % 5s", "[% 5s]", "ab");
    TEST("[optional] %#s", "[%#s]", "ab");
    TEST("[optional] %#5s", "[%#5s]", "ab");
    TEST("[optional] %+ #5.1s", "[%+ #5.1s]", "ab");
    g_optional = 0;

    printf("\n==============================================\n");
    printf("  REQUIRED : " GREEN "%d PASS" END "  /  " RED "%d FAIL" END "\n",
        g_pass, g_fail);
    printf("  OPTIONAL : %d pass  /  %d fail  (can be ignored)\n",
        g_opt_pass, g_opt_fail);
    if (g_fail == 0)
        printf(GREEN "  >>> %%s IS CORRECT <<<" END "\n");
    else
        printf(RED "  >>> %%s has %d problem(s), fix the first FAIL <<<" END
            "\n", g_fail);
    printf("==============================================\n");
    return (g_fail != 0);
}
