#include "../include/test.h"
#include <time.h>
#include <stdio.h>

clock_t t;

// Starts time test
void test_start()
{
	t = clock();
}

// Displays time elapsed since test started
void test_stop()
{
    t = clock() - t;
    printf ("Elapsed: %f seconds\n", (double) t / CLOCKS_PER_SEC);
    getch();
}