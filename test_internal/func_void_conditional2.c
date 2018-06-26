int a(int n);

void b(int n);


int f = 2;

int main(){
        
        int n = 9;
        int k = 4;
        int r = 4;
        
        if(a(n)<a(r)) {
                b(f) ;
                k++;
        }
       
        return f+k;
}

                
int a(int n){
        
        return n+5;

}


void b(int n){
        
        n++;

}
