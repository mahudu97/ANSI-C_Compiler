int addNumbers(int n);


int main(){
        
        int num;
        num = 20;
        return addNumbers(num);



}


int addNumbers(int n){
        
        if(n != 0) return n + addNumbers(n-1);
        
        else return n;
}
