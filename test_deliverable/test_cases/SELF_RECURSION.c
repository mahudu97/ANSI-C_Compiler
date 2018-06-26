int self_recursion(int base){
    /* MUST have base >=0 to terminate */

    /* base > 0 */
    if(base) return self_recursion(base-1);

    /* base = 0 */
    return 0; /* value 0 propogates back down stack to driver */
}