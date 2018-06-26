int fib_rec(int n);

int n=9;

     

int main() {
     int n;
     n=9;
     
     return fib_rec(n);
     
}

int fib_rec(int n) {
    if (n == 0) {
        return 0;
     } 
     if (n == 1) {
           return 1;
     }
     return fib_rec(n - 1) + fib_rec(n - 2);
}
