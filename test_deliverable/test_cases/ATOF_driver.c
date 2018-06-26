#include <stdlib.h> /* required for atof.c */

double atof_test();
/* should return 12.34 (hard-coded) */
#define ATOF_TEST_RES 12.34

int main(){
    /* only 1 test case - only testing call to atof */
    if( atof_test() == ATOF_TEST_RES ) return 0;
    return 1;
}