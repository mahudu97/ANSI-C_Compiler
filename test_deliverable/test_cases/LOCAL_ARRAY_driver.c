int local_array(int index);
/* MUST HAVE 0 <= index <= 9 - otherwise segmentation fault
   return should be 0
   dependancy - add and sub for int
   dependancy - function parameters working on compiler being tested
   - what if compiler works for element X, but not element Y?
*/

#define RUNS 10
/* equal to array size used in local_array */

int main(){

    int sum = 0;

    /* call local_array $RUNS times testing with index = 0 to 9 iteratively */
	int i;
    for(i= 0; i < RUNS; i++){
        sum += local_array(i);
        /* if local_array worked , sum should not change */
    }

    /* returns 0 if local_array returned 0 $RUNS times */
    return sum ? 1:0;
}