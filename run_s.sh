#!/bin/sh
# run_s.sh - build your library, compile test_s.c, run it
#
# Put test_s.c and run_s.sh in your ft_printf folder (next to ft_printf.h).
#   sh run_s.sh        -> every test shows PASS or FAIL
#   sh run_s.sh -q     -> only the FAIL are shown

FLAGS="-Wall -Wextra -Wno-format -Wno-format-security -Wno-format-extra-args"

echo "1) make"
make > /dev/null 2>&1 || { echo "make failed, run 'make' to see the error"; exit 1; }

echo "2) compile test_s.c"
if ! cc $FLAGS test_s.c libftprintf.a -I. -Ilibft -o test_s 2> /tmp/test_s_err; then
    # maybe libft is not inside libftprintf.a
    if ! cc $FLAGS test_s.c libftprintf.a -I. -Ilibft -Llibft -lft -o test_s 2> /tmp/test_s_err; then
        echo "compile error:"
        cat /tmp/test_s_err
        exit 1
    fi
fi

echo "3) run"
./test_s "$@"
