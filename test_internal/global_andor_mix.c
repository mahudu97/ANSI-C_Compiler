int x = 4;
int y = 12;

int main(){
	x = y | x && (y ^  y || x);
	return x;
}