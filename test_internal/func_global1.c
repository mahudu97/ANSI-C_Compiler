int gcd(int n1, int n2);


int n1 = 366;
int n2 = 60;



int main(){
        
        return gcd(n1,n2);
}

int gcd(int n1, int n2){
        
        if(n2 != 0) return gcd(n2, n1%n2);
        
        else return n1;
}
