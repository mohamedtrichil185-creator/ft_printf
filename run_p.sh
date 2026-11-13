#!/bin/sh
# run_p.sh - build your library, compile test_p.c, run it
#
# Put test_p.c and run_p.sh in your ft_printf folder (next to ft_printf.h).
#   sh run_p.sh        -> every test shows PASS or FAIL
#   sh run_p.sh -q     -> only the FAIL are shown

FLAGS="-Wall -Wextra -Wno-format -Wno-format-security -Wno-format-extra-args"

echo "1) make"
make > /dev/null 2>&1 || { echo "make failed, run 'make' to see the error"; exit 1; }

echo "2) compile test_p.c"
if ! cc $FLAGS test_p.c libftprintf.a -I. -Ilibft -o test_p 2> /tmp/test_p_err; then
    # maybe libft is not inside libftprintf.a
    if ! cc $FLAGS test_p.c libftprintf.a -I. -Ilibft -Llibft -lft -o test_p 2> /tmp/test_p_err; then
        echo "compile error:"
        cat /tmp/test_p_err
        exit 1
    fi
fi

echo "3) run"
./test_p "$@"
