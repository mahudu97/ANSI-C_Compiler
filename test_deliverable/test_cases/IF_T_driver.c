int IF_T(); /* expect 0 to be returned */

int main(){
    int val = IF_T();
    
    /* if IF_T 'failed' and returned 1, ternary operator makes main return 1 */
    return val ? 1:0;    
}