int f(int x,int y){
    x=5;
    y=6;
    return y;
}


int main(){
    
    int x = 2;
    int y = 4;
    x = f(x,y);
    return x;
}