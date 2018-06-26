int IF_ELSE_F(); /* expect 0 to be returned */

int main(){
    int val = IF_ELSE_F();
    
    /* if IF_ELSE_F 'failed' and returned 1, ternary operator makes main return 1 */
    return val ? 1:0;    
}