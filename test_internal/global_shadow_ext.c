int x = 4;

int main(){
	int x = 6;
	{
		extern x;
		x = 75;
		return x;
	}
}