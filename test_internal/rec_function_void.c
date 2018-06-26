    

void count_to_ten(int count);

int main(){
       int n;
       n=0;
       count_to_ten(n);
       return 0;
        
    
}



void count_to_ten(int count){
        
        if(count<10){
                count_to_ten(count+1);
        }
}



