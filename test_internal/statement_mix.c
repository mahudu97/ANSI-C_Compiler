int main(){
	int x = 0;
	int y = 2;
	int z = 0;
	do{
		x++;
		while(y < 5){
			y++;
			for(z=1;z<3;z++){
				if(x==4 && z == 2){
					y = 7;				
				}
			}	
		}
		y = 1;
	}while(x != 10);
	
	return x;
}