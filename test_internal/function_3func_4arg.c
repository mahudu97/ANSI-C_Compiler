int a (int x,int y,int z,int zz);

int b (int x,int y,int z,int zz);

int c (int x,int y,int z,int zz);



int fib_rec(int n);

int main(){
        
        int x,y,z,zz;
        x=1;
        y=2;
        z=3;
        zz=7;
        
       return  a(x,y,z,zz);
        
}

int a (int x,int y,int z,int zz){


        return x+y+z+zz+b(x,y,z,zz);
}

int b (int x,int y,int z,int zz){
        
        return fib_rec(x) + fib_rec(y) + fib_rec(z) + fib_rec(zz) + c(1,1,1,1);
}

int c (int x,int y,int z,int zz){

        return x+y+z+zz;
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



