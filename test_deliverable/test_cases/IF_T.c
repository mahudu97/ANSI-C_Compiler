int IF_T(){
    /* should always evaluate to true */
    if(1) return 0;
    return 1;  /* 'if' broken on compiler */
}