int FOR_N(int n);
/* MUST pass in a number >=0
   tests spec condition when n >=2
   expect return val 0
   dependancy - sub for int
   dependancy - function parameters working on compiler being tested
   reason for this:
   - could have used internal variable n , and set to >=2 (hard code)
   - this limits the amount of times the driver can test FOR_N to one value of N
   - what if compiler works for X loops of while, but not Y loops?
*/

/* test FOR_N loop for N = 2 to N = 100 */
#define START 2
#define END 100
/* ASIZE = END-START + 1 */
#define ASIZE 99 /* array size to match start,end */

int main(){
    
    int N [ASIZE]; /* store results from FOR_N */

	int i;
    for(i = START; i <= END; i++){
        N[i-START] = FOR_N(i);
    }

    int sum = 0;
    /* sum array N - if FOR_N works correctly, sum == 0 */
    
	int j;
    for(j = 0; j < ASIZE; j++){
        sum += N[j];
    }

    /* if sum == 0, return 0 */
    return sum ? 1:0;
}