int IF_F(); /* expect 0 to be returned */

int main(){
    int val = IF_F();
    
    /* if IF_F 'failed' and returned 1, ternary operator makes main return 1 */
    return val ? 1:0;    
}