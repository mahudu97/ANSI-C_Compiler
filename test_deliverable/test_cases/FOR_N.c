/* MUST have n >= 0 */
int FOR_N(int n){
    while(n){ /* should stop when a == 0 */
        n = n - 1;
    }
    return n; /* expect this to be 0 */
}