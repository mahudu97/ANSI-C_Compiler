int a(int n);

int b(int n);


int main(){
        
        int n = 9;

	int x = 4;
        
        if(a(n)<12){
                
                x = b(n);
        }
        
        return x;
}

                
int a(int n){
        
        return n+5;

}

int b(int n){
        
        return a(n)+5;

}
