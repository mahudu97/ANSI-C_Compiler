int x = 23;
int y = 5;

int main(){
	x = y + x % y;
	return x;
}