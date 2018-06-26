int f(int x,int y){
    x=5;
    return x+y;
}

int g(int y,int x){
    y = 7;
    return y+x;
}

int main(){
    
    int x = 6;
	int y = 7;

    y = g(6,12);
    return x+y;
}