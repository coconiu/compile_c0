#include<stdio.h>
int SquareSum(int _a,int b){
    const int unuse=0;
    int temp;
    temp=-_a*-1;
    temp=+_a*0;
    temp=_a*unuse;
    temp=_a*_a;
    temp=-(-_a*_a)+(b*b);
    return (temp);
}
int main(){
	int a,b;
	scanf("%d %d", &a, &b);
	printf("%d", SquareSum(a,b));
}
