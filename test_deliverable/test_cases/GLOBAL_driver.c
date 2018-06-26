extern int a; /* global variable from 'global.c' */
int b = 1;

int g(); /* should return a+b */

#define SUM 3 /* SUM = a + a + b */

int main(){

    int sum = a + g();

    return (sum - SUM) ? 1:0;
}