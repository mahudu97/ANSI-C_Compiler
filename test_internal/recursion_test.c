int f(int x);


int main(){
    int x;
    int y;
    x = f(5);
    return x;
    
}



int f(int x){
    
    if(x !=0){
         return  f(x-1);
    }
         return x;
}
    
