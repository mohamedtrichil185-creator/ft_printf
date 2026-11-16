/* ************************************************************************** */
/*                                                                            */
/*   ft_printf_test.c                                                         */
/*                                                                            */
/*   Full test suite for the 1337 / 42 "ft_printf" project.                   */
/*                                                                            */
/*   Covers:                                                                  */
/*     - Mandatory conversions: %c %s %p %d %i %u %x %X %%                    */
/*     - Bonus flags:        '-' '0' '.' '*' '#' ' ' '+'                       */
/*     - Field width & precision (literals and '*' dynamic values)            */
/*     - Combinations of multiple flags                                       */
/*     - Edge cases (NULL string, INT_MIN, 0, negative width, etc.)           */
/*                                                                            */
/*   How to use:                                                              */
/*     1) Compile your libftprintf.a (or .so) as usual.                        */
/*     2) gcc -Wall -Wextra ft_printf_test.c -L. -lftprintf -o test          */
/*        (adjust include path / lib name as needed)                          */
/*                                                                            */
/*        NOTE: the real printf throws -Wformat warnings on a few valid       */
/*        runtime constructs used here (e.g. "%-5%", "%s" with NULL,          */
/*        "%+08.3d").  Do NOT compile this test with -Werror -Wformat.        */
/*     3) ./test            -> runs every test, prints OK / KO per case        */
/*     4) ./test --diff     -> also prints the diff when a case fails         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*                                                                            */
/*  ======  REPLACE THESE TWO LINES WITH YOUR OWN HEADER  ===================  */
/*                                                                            */
/*  If your header is named differently (e.g. "ft_printf.h"), change here.   */
/*                                                                            */
#include "ft_printf.h"

/*                                                                            */
/* ========================================================================= */
/*                                                                            */

/* -------------------------------------------------------------------------- */
/*  Internal: capture stdout of a printf-like function                        */
/* -------------------------------------------------------------------------- */

#define TMP_FILE "/tmp/ft_printf_test_out.txt"

/*
 * Capture the output of `fn` (a function taking a FILE* and writing through it)
 * into a malloc'd string.  Returns NULL on failure.
 *
 * We redirect the real FILE* stdout to a temp file so that BOTH the standard
 * printf AND your ft_printf can be captured with the same mechanism.
 */
static char *capture_output(void (*fn)(void))
{
    FILE *fp = fopen(TMP_FILE, "w+b");
    if (!fp)
        return NULL;

    int saved_stdout = dup(STDOUT_FILENO);
    fflush(stdout);
    dup2(fileno(fp), STDOUT_FILENO);

    fn();

    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buf = (char *)malloc(size + 1);
    if (!buf)
    {
        fclose(fp);
        return NULL;
    }
    fread(buf, 1, size, fp);
    buf[size] = '\0';
    fclose(fp);
    remove(TMP_FILE);
    return buf;
}

/* -------------------------------------------------------------------------- */
/*  Global test counters                                                      */
/* -------------------------------------------------------------------------- */

static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;
static int g_show_diff = 0;

/*                                                                            */
/*  Each test case is stored as a pair of callbacks:                          */
/*    - real_fn   -> calls the standard `printf`                              */
/*    - ft_fn     -> calls your `ft_printf`                                   */
/*                                                                            */
/*  Capturing both outputs and comparing them gives the verdict.              */
/*                                                                            */

typedef struct s_test
{
    const char *name;
    void (*real_fn)(void);
    void (*ft_fn)(void);
} t_test;

#define MAX_TESTS 4096
static t_test g_tests[MAX_TESTS];
static int g_test_count = 0;

static void add_test(const char *name, void (*real_fn)(void), void (*ft_fn)(void))
{
    if (g_test_count >= MAX_TESTS)
        return;
    g_tests[g_test_count].name = name;
    g_tests[g_test_count].real_fn = real_fn;
    g_tests[g_test_count].ft_fn = ft_fn;
    g_test_count++;
}

/*                                                                            */
/*  Tiny helper macros so the test bodies stay readable.                      */
/*  Each test function declares `int r;` so we store the return value        */
/*  there (and silence unused warnings with `(void)r;`).                      */
/*                                                                            */
#define REAL_PRINTF(...)  r = printf(__VA_ARGS__)
#define FT_PRINTF(...)    r = ft_printf(__VA_ARGS__)

/* -------------------------------------------------------------------------- */
/*  %c  -  single character                                                   */
/* -------------------------------------------------------------------------- */

/* ---------- basic %c ---------- */
static void t_c_real(void) { int r; REAL_PRINTF("%c", 'A'); (void)r; }
static void t_c_ft  (void) { int r; FT_PRINTF ("%c", 'A'); (void)r; }

static void t_c_zero_real(void) { int r; REAL_PRINTF("%c", '\0'); (void)r; }
static void t_c_zero_ft  (void) { int r; FT_PRINTF ("%c", '\0'); (void)r; }

static void t_c_num_real(void) { int r; REAL_PRINTF("%c%c%c", '4', '2', '!'); (void)r; }
static void t_c_num_ft  (void) { int r; FT_PRINTF ("%c%c%c", '4', '2', '!'); (void)r; }

/* ---------- %c with width / flags (bonus) ---------- */
static void t_c_w_real(void) { int r; REAL_PRINTF("%5c", 'X'); (void)r; }
static void t_c_w_ft  (void) { int r; FT_PRINTF ("%5c", 'X'); (void)r; }

static void t_c_minus_real(void) { int r; REAL_PRINTF("%-5c|", 'X'); (void)r; }
static void t_c_minus_ft  (void) { int r; FT_PRINTF ("%-5c|", 'X'); (void)r; }

static void t_c_star_real(void) { int r; REAL_PRINTF("%*c", 7, 'Z'); (void)r; }
static void t_c_star_ft  (void) { int r; FT_PRINTF ("%*c", 7, 'Z'); (void)r; }

/* -------------------------------------------------------------------------- */
/*  %s  -  string                                                             */
/* -------------------------------------------------------------------------- */

static void t_s_real(void) { int r; REAL_PRINTF("%s", "hello"); (void)r; }
static void t_s_ft  (void) { int r; FT_PRINTF ("%s", "hello"); (void)r; }

static void t_s_empty_real(void) { int r; REAL_PRINTF("%s", ""); (void)r; }
static void t_s_empty_ft  (void) { int r; FT_PRINTF ("%s", ""); (void)r; }

static void t_s_null_real(void) { int r; REAL_PRINTF("%s", (char *)NULL); (void)r; }
static void t_s_null_ft  (void) { int r; FT_PRINTF ("%s", (char *)NULL); (void)r; }

static void t_s_multi_real(void) { int r; REAL_PRINTF("%s %s %s", "42", "is", "cool"); (void)r; }
static void t_s_multi_ft  (void) { int r; FT_PRINTF ("%s %s %s", "42", "is", "cool"); (void)r; }

/* ---------- %s with bonus ---------- */
static void t_s_w_real(void) { int r; REAL_PRINTF("%10s", "hi"); (void)r; }
static void t_s_w_ft  (void) { int r; FT_PRINTF ("%10s", "hi"); (void)r; }

static void t_s_minus_real(void) { int r; REAL_PRINTF("%-10s|", "hi"); (void)r; }
static void t_s_minus_ft  (void) { int r; FT_PRINTF ("%-10s|", "hi"); (void)r; }

static void t_s_prec_real(void) { int r; REAL_PRINTF("%.3s", "hello"); (void)r; }
static void t_s_prec_ft  (void) { int r; FT_PRINTF ("%.3s", "hello"); (void)r; }

static void t_s_w_prec_real(void) { int r; REAL_PRINTF("%10.3s|", "hello"); (void)r; }
static void t_s_w_prec_ft  (void) { int r; FT_PRINTF ("%10.3s|", "hello"); (void)r; }

static void t_s_star_w_p_real(void) { int r; REAL_PRINTF("%*.*s", 8, 3, "hello"); (void)r; }
static void t_s_star_w_p_ft  (void) { int r; FT_PRINTF ("%*.*s", 8, 3, "hello"); (void)r; }

static void t_s_minus_prec_real(void) { int r; REAL_PRINTF("%-8.3s|", "hello"); (void)r; }
static void t_s_minus_prec_ft  (void) { int r; FT_PRINTF ("%-8.3s|", "hello"); (void)r; }

static void t_s_prec_zero_real(void) { int r; REAL_PRINTF("%.0s", "hello"); (void)r; }
static void t_s_prec_zero_ft  (void) { int r; FT_PRINTF ("%.0s", "hello"); (void)r; }

/* -------------------------------------------------------------------------- */
/*  %p  -  pointer                                                            */
/* -------------------------------------------------------------------------- */

static void t_p_real(void) { int r; REAL_PRINTF("%p", (void *)0xdeadbeef); (void)r; }
static void t_p_ft  (void) { int r; FT_PRINTF ("%p", (void *)0xdeadbeef); (void)r; }

static void t_p_null_real(void) { int r; REAL_PRINTF("%p", (void *)NULL); (void)r; }
static void t_p_null_ft  (void) { int r; FT_PRINTF ("%p", (void *)NULL); (void)r; }

static void t_p_zero_real(void) { int r; REAL_PRINTF("%p", (void *)0); (void)r; }
static void t_p_zero_ft  (void) { int r; FT_PRINTF ("%p", (void *)0); (void)r; }

static void t_p_w_real(void) { int r; REAL_PRINTF("%20p", (void *)0x1234); (void)r; }
static void t_p_w_ft  (void) { int r; FT_PRINTF ("%20p", (void *)0x1234); (void)r; }

static void t_p_minus_real(void) { int r; REAL_PRINTF("%-20p|", (void *)0x1234); (void)r; }
static void t_p_minus_ft  (void) { int r; FT_PRINTF ("%-20p|", (void *)0x1234); (void)r; }

/* -------------------------------------------------------------------------- */
/*  %d / %i  -  signed decimal                                                */
/* -------------------------------------------------------------------------- */

static void t_d_real(void) { int r; REAL_PRINTF("%d", 42); (void)r; }
static void t_d_ft  (void) { int r; FT_PRINTF ("%d", 42); (void)r; }

static void t_d_neg_real(void) { int r; REAL_PRINTF("%d", -42); (void)r; }
static void t_d_neg_ft  (void) { int r; FT_PRINTF ("%d", -42); (void)r; }

static void t_d_zero_real(void) { int r; REAL_PRINTF("%d", 0); (void)r; }
static void t_d_zero_ft  (void) { int r; FT_PRINTF ("%d", 0); (void)r; }

static void t_d_intmax_real(void) { int r; REAL_PRINTF("%d", INT_MAX); (void)r; }
static void t_d_intmax_ft  (void) { int r; FT_PRINTF ("%d", INT_MAX); (void)r; }

static void t_d_intmin_real(void) { int r; REAL_PRINTF("%d", INT_MIN); (void)r; }
static void t_d_intmin_ft  (void) { int r; FT_PRINTF ("%d", INT_MIN); (void)r; }

static void t_i_real(void) { int r; REAL_PRINTF("%i", -12345); (void)r; }
static void t_i_ft  (void) { int r; FT_PRINTF ("%i", -12345); (void)r; }

/* ---------- %d with bonus ---------- */
static void t_d_w_real(void) { int r; REAL_PRINTF("%8d", 42); (void)r; }
static void t_d_w_ft  (void) { int r; FT_PRINTF ("%8d", 42); (void)r; }

static void t_d_zero_real2(void) { int r; REAL_PRINTF("%08d", 42); (void)r; }
static void t_d_zero_ft2  (void) { int r; FT_PRINTF ("%08d", 42); (void)r; }

static void t_d_minus_real(void) { int r; REAL_PRINTF("%-8d|", 42); (void)r; }
static void t_d_minus_ft  (void) { int r; FT_PRINTF ("%-8d|", 42); (void)r; }

static void t_d_prec_real(void) { int r; REAL_PRINTF("%.5d", 42); (void)r; }
static void t_d_prec_ft  (void) { int r; FT_PRINTF ("%.5d", 42); (void)r; }

static void t_d_prec_zero_real(void) { int r; REAL_PRINTF("%.0d", 0); (void)r; }
static void t_d_prec_zero_ft  (void) { int r; FT_PRINTF ("%.0d", 0); (void)r; }

static void t_d_plus_real(void) { int r; REAL_PRINTF("%+d", 42); (void)r; }
static void t_d_plus_ft  (void) { int r; FT_PRINTF ("%+d", 42); (void)r; }

static void t_d_space_real(void) { int r; REAL_PRINTF("% d", 42); (void)r; }
static void t_d_space_ft  (void) { int r; FT_PRINTF ("% d", 42); (void)r; }

static void t_d_plus_neg_real(void) { int r; REAL_PRINTF("%+d", -42); (void)r; }
static void t_d_plus_neg_ft  (void) { int r; FT_PRINTF ("%+d", -42); (void)r; }

static void t_d_star_w_real(void) { int r; REAL_PRINTF("%*d", 10, 7); (void)r; }
static void t_d_star_w_ft  (void) { int r; FT_PRINTF ("%*d", 10, 7); (void)r; }

static void t_d_star_p_real(void) { int r; REAL_PRINTF("%.*d", 5, 7); (void)r; }
static void t_d_star_p_ft  (void) { int r; FT_PRINTF ("%.*d", 5, 7); (void)r; }

static void t_d_star_neg_w_real(void) { int r; REAL_PRINTF("%*d", -10, 7); (void)r; }
static void t_d_star_neg_w_ft  (void) { int r; FT_PRINTF ("%*d", -10, 7); (void)r; }

/* combination: 0 + precision (precision wins) */
static void t_d_zero_prec_real(void) { int r; REAL_PRINTF("%08.3d", 42); (void)r; }
static void t_d_zero_prec_ft  (void) { int r; FT_PRINTF ("%08.3d", 42); (void)r; }

/* combination: + 0 width */
static void t_d_plus_zero_real(void) { int r; REAL_PRINTF("%+08d", 42); (void)r; }
static void t_d_plus_zero_ft  (void) { int r; FT_PRINTF ("%+08d", 42); (void)r; }

/* combination: space + width */
static void t_d_space_w_real(void) { int r; REAL_PRINTF("% 8d", 42); (void)r; }
static void t_d_space_w_ft  (void) { int r; FT_PRINTF ("% 8d", 42); (void)r; }

/* -------------------------------------------------------------------------- */
/*  %u  -  unsigned decimal                                                   */
/* -------------------------------------------------------------------------- */

static void t_u_real(void) { int r; REAL_PRINTF("%u", 42u); (void)r; }
static void t_u_ft  (void) { int r; FT_PRINTF ("%u", 42u); (void)r; }

static void t_u_zero_real(void) { int r; REAL_PRINTF("%u", 0u); (void)r; }
static void t_u_zero_ft  (void) { int r; FT_PRINTF ("%u", 0u); (void)r; }

static void t_u_max_real(void) { int r; REAL_PRINTF("%u", UINT_MAX); (void)r; }
static void t_u_max_ft  (void) { int r; FT_PRINTF ("%u", UINT_MAX); (void)r; }

static void t_u_w_real(void) { int r; REAL_PRINTF("%10u", 42u); (void)r; }
static void t_u_w_ft  (void) { int r; FT_PRINTF ("%10u", 42u); (void)r; }

static void t_u_zero_pad_real(void) { int r; REAL_PRINTF("%010u", 42u); (void)r; }
static void t_u_zero_pad_ft  (void) { int r; FT_PRINTF ("%010u", 42u); (void)r; }

static void t_u_prec_real(void) { int r; REAL_PRINTF("%.5u", 42u); (void)r; }
static void t_u_prec_ft  (void) { int r; FT_PRINTF ("%.5u", 42u); (void)r; }

static void t_u_minus_real(void) { int r; REAL_PRINTF("%-10u|", 42u); (void)r; }
static void t_u_minus_ft  (void) { int r; FT_PRINTF ("%-10u|", 42u); (void)r; }

/* -------------------------------------------------------------------------- */
/*  %x / %X  -  hexadecimal                                                   */
/* -------------------------------------------------------------------------- */

static void t_x_real(void) { int r; REAL_PRINTF("%x", 0xdeadbeef); (void)r; }
static void t_x_ft  (void) { int r; FT_PRINTF ("%x", 0xdeadbeef); (void)r; }

static void t_X_real(void) { int r; REAL_PRINTF("%X", 0xdeadbeef); (void)r; }
static void t_X_ft  (void) { int r; FT_PRINTF ("%X", 0xdeadbeef); (void)r; }

static void t_x_zero_real(void) { int r; REAL_PRINTF("%x", 0); (void)r; }
static void t_x_zero_ft  (void) { int r; FT_PRINTF ("%x", 0); (void)r; }

static void t_x_max_real(void) { int r; REAL_PRINTF("%x", UINT_MAX); (void)r; }
static void t_x_max_ft  (void) { int r; FT_PRINTF ("%x", UINT_MAX); (void)r; }

/* ---------- %x with bonus ---------- */
static void t_x_w_real(void) { int r; REAL_PRINTF("%10x", 0xab); (void)r; }
static void t_x_w_ft  (void) { int r; FT_PRINTF ("%10x", 0xab); (void)r; }

static void t_x_zero_pad_real(void) { int r; REAL_PRINTF("%010x", 0xab); (void)r; }
static void t_x_zero_pad_ft  (void) { int r; FT_PRINTF ("%010x", 0xab); (void)r; }

static void t_x_prec_real(void) { int r; REAL_PRINTF("%.5x", 0xab); (void)r; }
static void t_x_prec_ft  (void) { int r; FT_PRINTF ("%.5x", 0xab); (void)r; }

static void t_x_minus_real(void) { int r; REAL_PRINTF("%-10x|", 0xab); (void)r; }
static void t_x_minus_ft  (void) { int r; FT_PRINTF ("%-10x|", 0xab); (void)r; }

/* # flag: prefix 0x / 0X */
static void t_x_hash_real(void) { int r; REAL_PRINTF("%#x", 0xab); (void)r; }
static void t_x_hash_ft  (void) { int r; FT_PRINTF ("%#x", 0xab); (void)r; }

static void t_X_hash_real(void) { int r; REAL_PRINTF("%#X", 0xab); (void)r; }
static void t_X_hash_ft  (void) { int r; FT_PRINTF ("%#X", 0xab); (void)r; }

static void t_x_hash_zero_real(void) { int r; REAL_PRINTF("%#x", 0); (void)r; }
static void t_x_hash_zero_ft  (void) { int r; FT_PRINTF ("%#x", 0); (void)r; }

static void t_x_hash_zero_pad_real(void) { int r; REAL_PRINTF("%#010x", 0xab); (void)r; }
static void t_x_hash_zero_pad_ft  (void) { int r; FT_PRINTF ("%#010x", 0xab); (void)r; }

static void t_x_hash_prec_real(void) { int r; REAL_PRINTF("%#.5x", 0xab); (void)r; }
static void t_x_hash_prec_ft  (void) { int r; FT_PRINTF ("%#.5x", 0xab); (void)r; }

/* -------------------------------------------------------------------------- */
/*  %%  -  literal percent                                                    */
/* -------------------------------------------------------------------------- */

static void t_pct_real(void) { int r; REAL_PRINTF("%%"); (void)r; }
static void t_pct_ft  (void) { int r; FT_PRINTF ("%%"); (void)r; }

static void t_pct_w_real(void) { int r; REAL_PRINTF("%5%"); (void)r; }
static void t_pct_w_ft  (void) { int r; FT_PRINTF ("%5%"); (void)r; }

static void t_pct_minus_real(void) { int r; REAL_PRINTF("%-5%|"); (void)r; }
static void t_pct_minus_ft  (void) { int r; FT_PRINTF ("%-5%|"); (void)r; }

/* -------------------------------------------------------------------------- */
/*  Mixed / complex combinations                                              */
/* -------------------------------------------------------------------------- */

static void t_mix1_real(void) { int r; REAL_PRINTF("Hello %s, score=%d%%\n", "World", 99); (void)r; }
static void t_mix1_ft  (void) { int r; FT_PRINTF ("Hello %s, score=%d%%\n", "World", 99); (void)r; }

static void t_mix2_real(void) { int r; REAL_PRINTF("%+08.3d|%-5s|%#x", 42, "hi", 0xff); (void)r; }
static void t_mix2_ft  (void) { int r; FT_PRINTF ("%+08.3d|%-5s|%#x", 42, "hi", 0xff); (void)r; }

static void t_mix3_real(void) { int r; REAL_PRINTF("[%*d][%.*s][%#X]", 8, 7, 3, "abcdef", 0xa); (void)r; }
static void t_mix3_ft  (void) { int r; FT_PRINTF ("[%*d][%.*s][%#X]", 8, 7, 3, "abcdef", 0xa); (void)r; }

static void t_mix4_real(void) { int r; REAL_PRINTF("%c%c%c%c%c", '4','2',' ','!','\n'); (void)r; }
static void t_mix4_ft  (void) { int r; FT_PRINTF ("%c%c%c%c%c", '4','2',' ','!','\n'); (void)r; }

/* -------------------------------------------------------------------------- */
/*  Register all tests                                                        */
/* -------------------------------------------------------------------------- */

static void register_tests(void)
{
    /* %c */
    add_test("basic %c 'A'",                t_c_real,        t_c_ft);
    add_test("%c null char",                t_c_zero_real,   t_c_zero_ft);
    add_test("%c multi '4','2','!'",        t_c_num_real,    t_c_num_ft);
    add_test("%c width %5c",                t_c_w_real,      t_c_w_ft);
    add_test("%c minus %-5c|",              t_c_minus_real,  t_c_minus_ft);
    add_test("%c star %*c",                 t_c_star_real,   t_c_star_ft);

    /* %s */
    add_test("basic %s",                    t_s_real,        t_s_ft);
    add_test("%s empty",                    t_s_empty_real,  t_s_empty_ft);
    add_test("%s NULL",                     t_s_null_real,   t_s_null_ft);
    add_test("%s multi",                    t_s_multi_real,  t_s_multi_ft);
    add_test("%s width %10s",               t_s_w_real,      t_s_w_ft);
    add_test("%s minus %-10s|",             t_s_minus_real,  t_s_minus_ft);
    add_test("%s prec %.3s",                t_s_prec_real,   t_s_prec_ft);
    add_test("%s w+prec %10.3s|",           t_s_w_prec_real, t_s_w_prec_ft);
    add_test("%s star w+p %*.*s",           t_s_star_w_p_real, t_s_star_w_p_ft);
    add_test("%s minus+prec %-8.3s|",       t_s_minus_prec_real, t_s_minus_prec_ft);
    add_test("%s prec zero %.0s",           t_s_prec_zero_real, t_s_prec_zero_ft);

    /* %p */
    add_test("basic %p",                    t_p_real,        t_p_ft);
    add_test("%p NULL",                     t_p_null_real,   t_p_null_ft);
    add_test("%p zero",                     t_p_zero_real,   t_p_zero_ft);
    add_test("%p width %20p",               t_p_w_real,      t_p_w_ft);
    add_test("%p minus %-20p|",             t_p_minus_real,  t_p_minus_ft);

    /* %d / %i */
    add_test("basic %d 42",                 t_d_real,        t_d_ft);
    add_test("%d -42",                      t_d_neg_real,    t_d_neg_ft);
    add_test("%d 0",                        t_d_zero_real,   t_d_zero_ft);
    add_test("%d INT_MAX",                  t_d_intmax_real, t_d_intmax_ft);
    add_test("%d INT_MIN",                  t_d_intmin_real, t_d_intmin_ft);
    add_test("%i -12345",                   t_i_real,        t_i_ft);
    add_test("%d width %8d",                t_d_w_real,      t_d_w_ft);
    add_test("%d zero pad %08d",            t_d_zero_real2,  t_d_zero_ft2);
    add_test("%d minus %-8d|",              t_d_minus_real,  t_d_minus_ft);
    add_test("%d prec %.5d",                t_d_prec_real,   t_d_prec_ft);
    add_test("%d prec zero %.0d 0",         t_d_prec_zero_real, t_d_prec_zero_ft);
    add_test("%d plus %+d",                 t_d_plus_real,   t_d_plus_ft);
    add_test("%d space % d",                t_d_space_real,  t_d_space_ft);
    add_test("%d plus neg %+d -42",         t_d_plus_neg_real, t_d_plus_neg_ft);
    add_test("%d star width %*d",           t_d_star_w_real, t_d_star_w_ft);
    add_test("%d star prec %.*d",           t_d_star_p_real, t_d_star_p_ft);
    add_test("%d star neg width %*d",       t_d_star_neg_w_real, t_d_star_neg_w_ft);
    add_test("%d zero+prec %08.3d",         t_d_zero_prec_real, t_d_zero_prec_ft);
    add_test("%d plus+zero %+08d",          t_d_plus_zero_real, t_d_plus_zero_ft);
    add_test("%d space+width % 8d",         t_d_space_w_real, t_d_space_w_ft);

    /* %u */
    add_test("basic %u 42",                 t_u_real,        t_u_ft);
    add_test("%u 0",                        t_u_zero_real,   t_u_zero_ft);
    add_test("%u UINT_MAX",                 t_u_max_real,    t_u_max_ft);
    add_test("%u width %10u",               t_u_w_real,      t_u_w_ft);
    add_test("%u zero pad %010u",           t_u_zero_pad_real, t_u_zero_pad_ft);
    add_test("%u prec %.5u",                t_u_prec_real,   t_u_prec_ft);
    add_test("%u minus %-10u|",             t_u_minus_real,  t_u_minus_ft);

    /* %x / %X */
    add_test("basic %x deadbeef",           t_x_real,        t_x_ft);
    add_test("basic %X deadbeef",           t_X_real,        t_X_ft);
    add_test("%x 0",                        t_x_zero_real,   t_x_zero_ft);
    add_test("%x UINT_MAX",                 t_x_max_real,    t_x_max_ft);
    add_test("%x width %10x",               t_x_w_real,      t_x_w_ft);
    add_test("%x zero pad %010x",           t_x_zero_pad_real, t_x_zero_pad_ft);
    add_test("%x prec %.5x",                t_x_prec_real,   t_x_prec_ft);
    add_test("%x minus %-10x|",             t_x_minus_real,  t_x_minus_ft);
    add_test("%x hash %#x",                 t_x_hash_real,   t_x_hash_ft);
    add_test("%X hash %#X",                  t_X_hash_real,   t_X_hash_ft);
    add_test("%x hash 0",                   t_x_hash_zero_real, t_x_hash_zero_ft);
    add_test("%x hash+zero pad %#010x",     t_x_hash_zero_pad_real, t_x_hash_zero_pad_ft);
    add_test("%x hash+prec %#.5x",          t_x_hash_prec_real, t_x_hash_prec_ft);

    /* %% */
    add_test("basic %%",                    t_pct_real,      t_pct_ft);
    add_test("%% width %5%",                t_pct_w_real,    t_pct_w_ft);
    add_test("%% minus %-5%|",              t_pct_minus_real, t_pct_minus_ft);

    /* mixed */
    add_test("mix: Hello %s score=%d%%",    t_mix1_real,     t_mix1_ft);
    add_test("mix: %+08.3d|%-5s|%#x",      t_mix2_real,     t_mix2_ft);
    add_test("mix: [%*d][%.*s][%#X]",      t_mix3_real,     t_mix3_ft);
    add_test("mix: 5 chars",                t_mix4_real,     t_mix4_ft);
}

/* -------------------------------------------------------------------------- */
/*  Pretty color output                                                       */
/* -------------------------------------------------------------------------- */

#define C_GREEN  "\033[32m"
#define C_RED    "\033[31m"
#define C_YELLOW "\033[33m"
#define C_RESET  "\033[0m"
#define C_BOLD   "\033[1m"
#define C_CYAN   "\033[36m"

/* -------------------------------------------------------------------------- */
/*  Main test runner                                                          */
/* -------------------------------------------------------------------------- */

static void print_visible(const char *s)
{
    if (!s)
    {
        printf("(null)");
        return;
    }
    for (const char *p = s; *p; p++)
    {
        if (*p == '\n')      printf("\\n");
        else if (*p == '\t') printf("\\t");
        else if (*p == '\r') printf("\\r");
        else if (*p == '\0') printf("\\0");
        else if (*p >= 32 && *p < 127) putchar(*p);
        else printf("\\x%02x", (unsigned char)*p);
    }
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--diff") == 0)
        g_show_diff = 1;

    register_tests();

    printf(C_BOLD C_CYAN "\n========================================\n");
    printf("    ft_printf  -  test suite  (%d cases)\n", g_test_count);
    printf("========================================\n" C_RESET "\n");

    for (int i = 0; i < g_test_count; i++)
    {
        g_total++;
        char *real_out = capture_output(g_tests[i].real_fn);
        char *ft_out   = capture_output(g_tests[i].ft_fn);

        int ok = (real_out && ft_out && strcmp(real_out, ft_out) == 0);

        if (ok)
        {
            g_passed++;
            printf(C_GREEN "  [OK] " C_RESET " %s\n", g_tests[i].name);
        }
        else
        {
            g_failed++;
            printf(C_RED    "  [KO] " C_RESET " %s\n", g_tests[i].name);
            if (g_show_diff)
            {
                printf("        expected : \"");
                print_visible(real_out);
                printf("\"\n");
                printf("        got      : \"");
                print_visible(ft_out);
                printf("\"\n");
            }
        }
        free(real_out);
        free(ft_out);
    }

    printf(C_BOLD "\n----------------------------------------\n" C_RESET);
    printf("Total : %d    ", g_total);
    printf(C_GREEN "Passed : %d   " C_RESET, g_passed);
    printf(C_RED    "Failed : %d\n" C_RESET, g_failed);
    if (g_failed == 0)
        printf(C_BOLD C_GREEN "\n  *** ALL TESTS PASSED ***\n" C_RESET);
    else
        printf(C_BOLD C_YELLOW "\n  Run with --diff to see expected vs actual.\n" C_RESET);
    printf("\n");

    return (g_failed == 0) ? 0 : 1;
}
