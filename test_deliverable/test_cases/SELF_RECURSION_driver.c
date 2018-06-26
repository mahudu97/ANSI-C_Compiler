int self_recursion(int base);
/*  
    Expect return to be 0
    MUST have base >=0 to terminate
    dependancy - if can take true branch when true, and not when false
    //// unless another way to conditionally call funciton?
*/

#define RUNS 1000
/* amount of times to test self_recursion */

int main(){
    int sum = 0;

    /* tests self_recursion with base = 0 to 99 */
	int i;
    for(i = 0; i < RUNS; i++){
        sum += self_recursion(i); /* should be adding 0 to sum */
    }

    /* final sum value should be 0 */
    return sum ? 1:0;
}