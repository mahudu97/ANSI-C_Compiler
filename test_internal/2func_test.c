int f(int x,int y){
    x=5;
    return x;
}

int g(int y,int x){
    y = 7;
    return y;
}

int main(){
    
    int x = 2;
    x = f(x,5);
    int y = g(x,12);
    int z = (f(x,y) + 42) % 8;
    return x+y+z*g(z,x^y);
}