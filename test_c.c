/*
** test_c.c - tests ONLY %c (mandatory + bonus flags)
**
** Compares the real printf with your ft_printf (text byte by byte, \0
** included, + return value). Your ft_printf runs in a child process, so a
** segfault does not stop the other tests.
**
** Compile:
**   make
**   cc -Wall -Wextra -Wno-format -Wno-format-security -Wno-format-extra-args \
**      test_c.c libftprintf.a -I. -Ilibft -o test_c
**   (if undefined reference to a libft function, add: -Llibft -lft)
**
** Run:
**   ./test_c        -> show only KO
**   ./test_c -v     -> also show OK
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include "ft_printf.h"

#define BUFSZ 4096

typedef struct s_res
{
    int     ret;
    int     len;
    char    buf[BUFSZ];
}   t_res;

static int  g_ok;
static int  g_ko;
static int  g_verbose;
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

    printf("   %s [", label);
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
    printf("] len=%d ret=%d\n", len, ret);
}

static void report(int line, const char *desc, t_res *e, t_res *g, int status)
{
    int same;

    if (WIFSIGNALED(status))
    {
        g_ko++;
        printf("\033[31mKO\033[0m line %d: %s\n", line, desc);
        printf("   ft_printf CRASHED (signal %d)%s\n", WTERMSIG(status),
            WTERMSIG(status) == SIGSEGV ? " = segfault" : "");
        return ;
    }
    same = (e->ret == g->ret && e->len == g->len
            && memcmp(e->buf, g->buf, e->len) == 0);
    if (same)
    {
        g_ok++;
        if (g_verbose)
            printf("\033[32mOK\033[0m line %d: %s\n", line, desc);
        return ;
    }
    g_ko++;
    printf("\033[31mKO\033[0m line %d: %s\n", line, desc);
    show("printf   :", e->buf, e->len, e->ret);
    show("ft_printf:", g->buf, g->len, g->ret);
}

#define TEST(...) do { \
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
    report(__LINE__, #__VA_ARGS__, &exp_, &got_, status_); \
} while (0)

static void title(const char *s)
{
    printf("\n--- %s ---\n", s);
}

int main(int ac, char **av)
{
    g_verbose = (ac > 1 && strcmp(av[1], "-v") == 0);

    title("1. BASIC %c (mandatory)");
    TEST("%c", 'a');
    TEST("%c", 'Z');
    TEST("%c", '0');
    TEST("%c", ' ');
    TEST("%c", '%');
    TEST("%c", '\n');
    TEST("%c", '\t');
    TEST("[%c]", 'x');
    TEST("%c%c%c", 'a', 'b', 'c');
    TEST("a%cb", 'X');
    TEST("%c %c %c", 'a', 'b', 'c');
    TEST("%c%%%c", 'a', 'b');

    title("2. SPECIAL VALUES (\\0 and high bytes)");
    TEST("%c", 0);
    TEST("[%c]", 0);
    TEST("%c%c", 0, 'a');
    TEST("%c%c", 'a', 0);
    TEST("%c%c%c", 0, 0, 0);
    TEST("a%cb", 0);
    TEST("%c", 127);
    TEST("%c", 128);
    TEST("%c", 255);
    TEST("%c", 256);
    TEST("%c", 'a' + 256);
    TEST("%c", -1);

    title("3. WIDTH (spaces on the left)");
    TEST("[%1c]", 'x');
    TEST("[%2c]", 'x');
    TEST("[%3c]", 'x');
    TEST("[%5c]", 'x');
    TEST("[%10c]", 'x');
    TEST("[%20c]", 'x');
    TEST("[%5c]", 0);
    TEST("[%5c]", 255);
    TEST("[%5c]", ' ');
    TEST("[%5c|%5c]", 'a', 'b');
    TEST("[%100c]", 'x');

    title("4. FLAG - (spaces on the right)");
    TEST("[%-1c]", 'x');
    TEST("[%-2c]", 'x');
    TEST("[%-3c]", 'x');
    TEST("[%-5c]", 'x');
    TEST("[%-10c]", 'x');
    TEST("[%-20c]", 'x');
    TEST("[%-5c]", 0);
    TEST("[%-5c]", 255);
    TEST("[%-5c|%-5c]", 'a', 'b');
    TEST("[%-100c]", 'x');
    TEST("[%-5c|%5c]", 'a', 'b');
    TEST("[%5c|%-5c]", 'a', 'b');

    title("5. REPEATED FLAG -");
    TEST("[%--5c]", 'x');
    TEST("[%---5c]", 'x');

    title("6. PRECISION (ignored by glibc for %c)");
    TEST("[%.c]", 'x');
    TEST("[%.0c]", 'x');
    TEST("[%.1c]", 'x');
    TEST("[%.5c]", 'x');
    TEST("[%5.c]", 'x');
    TEST("[%5.0c]", 'x');
    TEST("[%5.3c]", 'x');
    TEST("[%-5.3c]", 'x');
    TEST("[%-5.c]", 'x');
    TEST("[%.3c]", 0);

    title("7. FLAGS + space # (ignored by glibc for %c)");
    TEST("[%+c]", 'x');
    TEST("[%+5c]", 'x');
    TEST("[%+-5c]", 'x');
    TEST("[% c]", 'x');
    TEST("[% 5c]", 'x');
    TEST("[% -5c]", 'x');
    TEST("[%#c]", 'x');
    TEST("[%#5c]", 'x');
    TEST("[%#-5c]", 'x');
    TEST("[%+ #5c]", 'x');
    TEST("[%+ #-5c]", 'x');

    title("8. FLAG 0 (undefined in C: optional, glibc may differ)");
    TEST("[%05c]", 'x');
    TEST("[%0-5c]", 'x');
    TEST("[%-05c]", 'x');
    TEST("[%03c]", 'x');
    TEST("[%01c]", 'x');

    title("9. MANY %c IN ONE CALL");
    TEST("[%3c|%-3c|%c]", 'a', 'b', 'c');
    TEST("%c%c%c%c%c", 'h', 'e', 'l', 'l', 'o');
    TEST("[%5c][%-5c][%5c][%-5c]", 'a', 'b', 'c', 'd');
    TEST("%c-%c-%c", 0, 'x', 0);
    TEST("[%10c]\n", 'x');
    TEST("[%-10c]\n", 'x');

    title("10. MIXED WITH OTHER SPECIFIERS");
    TEST("%c %s %d", 'a', "bc", 42);
    TEST("[%5c|%-5s|%5d]", 'a', "bc", 42);
    TEST("%d%c%d", 1, '-', 2);
    TEST("%s%c%s", "ab", ':', "cd");
    TEST("[%+d|%5c|% d]", 1, 'x', 2);
    TEST("[%c|%c|%c|%c]", 'a', 0, 'b', 255);
    TEST("%%[%5c]%%", 'x');
    TEST("%x%c%X", 255, 'z', 255);

    title("11. FLAGS MUST BE RESET BETWEEN TWO %");
    TEST("[%-5c] [%5c]", 'a', 'b');
    TEST("[%-5d] [%5c]", 1, 'b');
    TEST("[%5c] [%c]", 'a', 'b');
    TEST("[%-5c] [%c]", 'a', 'b');
    TEST("[%+d] [%c]", 1, 'b');
    TEST("[%.3d] [%c]", 1, 'b');

    printf("\n=== %%c: %d OK / %d KO ===\n", g_ok, g_ko);
    return (g_ko != 0);
}
