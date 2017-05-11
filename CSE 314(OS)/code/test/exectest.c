/*
 * exectest.c
 *
 * Exec a simple program.  Return the result of Exec.
 */

#include "syscall.h"

int
main()
{
    int result = 1000;
    char buffer[13];
    Write("Write Something: from nachos\n",29,0);
    result = Exec("../test/matmult");
    Exit(result);
}
