int a(int n);

int b(int n);


int main(){
        
        int n = 9;
        int k;
        int r = 0;
        for(k = 0 ; k < a(n) ; k++){
                r = r + b(r);
        }
        
        
        
        return r;
}

                
int a(int n){
        
        return n+5;

}


int b(int n){
        
        return a(n)+5;

}
