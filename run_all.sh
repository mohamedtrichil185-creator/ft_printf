#!/bin/sh
# run_all.sh - build your library, compile test_all.c, run it
#
# Put test_all.c and run_all.sh in your ft_printf folder (next to the Makefile).
#
#   sh run_all.sh --man        builds the MANDATORY library (make) and runs
#                              only the mandatory tests (no flags)
#   sh run_all.sh --bonus      builds the BONUS library (make bonus) and runs
#                              ALL the tests (mandatory + bonus flags)
#   sh run_all.sh --nomake     does not touch your library, runs all the tests
#
# You can add the other options after it:
#   -q         only the FAIL
#   c s p d u x X mix     only one specifier
# Examples:
#   sh run_all.sh --man
#   sh run_all.sh --bonus -q
#   sh run_all.sh --bonus d

FLAGS="-Wall -Wextra -Wno-format -Wno-format-security -Wno-format-extra-args"
INCS="-Iincludes -I. -Ilibft"
MODE=""
ARGS=""

for a in "$@"; do
    case "$a" in
        --man)    MODE="man" ;;
        --bonus)  MODE="bonus" ;;
        --nomake) MODE="nomake" ;;
        *)        ARGS="$ARGS $a" ;;
    esac
done

if [ -z "$MODE" ]; then
    echo "Choose a mode:  sh run_all.sh --man   or   sh run_all.sh --bonus"
    echo "(--man = mandatory only, --bonus = mandatory + flags)"
    exit 1
fi

case "$MODE" in
    man)
        echo "1) make fclean && make   (mandatory library)"
        make fclean > /dev/null 2>&1
        make > /dev/null 2>&1 || { echo "make failed, run 'make' to see the error"; exit 1; }
        ARGS="$ARGS -m"
        ;;
    bonus)
        echo "1) make fclean && make bonus   (bonus library)"
        make fclean > /dev/null 2>&1
        make bonus > /dev/null 2>&1 || { echo "make bonus failed, run 'make bonus' to see the error"; exit 1; }
        ;;
    nomake)
        echo "1) library not rebuilt (--nomake)"
        ;;
esac

echo "2) compile test_all.c"
if ! cc $FLAGS test_all.c libftprintf.a $INCS -o test_all 2> /tmp/test_all_err; then
    if ! cc $FLAGS test_all.c libftprintf.a $INCS -Llibft -lft -o test_all 2> /tmp/test_all_err; then
        echo "compile error:"
        cat /tmp/test_all_err
        exit 1
    fi
fi

echo "3) run"
./test_all $ARGS
