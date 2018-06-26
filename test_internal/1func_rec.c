int f(int x){
    if (x !=0){
			return f(x-1);
	}
	else {
		return 5;
	}
}


int main(){
    
    int x;
	x = f(3);
    return x;
}