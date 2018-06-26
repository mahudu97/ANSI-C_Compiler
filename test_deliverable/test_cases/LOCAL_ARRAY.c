int local_array(int index){ /* 0 <= index <= 9 */

    int val; /* not used until set */

    /* local array - also tests array initialisation */
    int la [10] = {0,1,2,3,4,5,6,7,8,9};
    /* array elements equal to the index value - allows a return value of 0 for function */

    val = la[index]; /* tests read from local array, val = index */

    /* tests write to local array */
    la[0] = 0;
    la[1] = 0;
    la[2] = 0;
    la[3] = 0;
    la[4] = 0;
    la[5] = 0;
    la[6] = 0;
    la[7] = 0;
    la[8] = 0;
    la[9] = 0;
    /* add la[index] to sum (should have no effect if write working) */

    val = val + la[index]; /* value should be equal to index */
    val = val - index;     /* value should be equal to 0 */

    return val; /* return should be 0 */
    
}