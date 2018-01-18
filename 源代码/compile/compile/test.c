const int a = 1;
const int b = 2,int c = 3;
const char c1 = 'a';
const char d = 'z';
int i,j;
int x,y,z;
int x1[2],x2[2];

int sqrt(){
	return (1);
}


void plus(int x,int y){
	x = 1;
	y = 1;
	x = x+y;
	y = x;
}

void main(){
	scanf(x,y);
	switch (x){
		case 1: x = x+1;
		case 2: x = x+6;
		case 3: x = x+21;
		case 4: x = x+16;
		case 5: x = x+19;
		case 6: x = x-2;
		case 7: x = x-6;
		case 8: x = x+7;
		case 9: x = x+22;
		case 10:x = x-17;
		default:y = 1;
	}
	if(x>=20){
		y = 2;
	}else{
		y = 0;
	}
	for(i=0;i<10;i=i+1){
		y = y+2;
	}
	y = y+sqrt(x);
	plus(x,y);
	z = 110;
	printf("a = ",x);
	printf("Hello World!");
	printf(x+y);
}
