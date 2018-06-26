int x = 4;
int y = 5;

int main(){
	x = y;
	int i;	
	for(i=0; i<2; ++i){
		int z = y;		
		x = 5;
	}
	return x;
}