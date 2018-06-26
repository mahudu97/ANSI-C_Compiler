int a = 1; /* global variable for driver */

extern int b; /* global variable from driver */

int g(){
    return a+b;
}