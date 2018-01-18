#include<stdio.h>
int fibonacci (int n){    
    const int cona = -1;    
    char testc;
    if(n==1) return (1);    
    if(n!=2) return (+fibonacci(n+-1)+fibonacci(n-2)+0); 
    return (1);
}
int main(){
	int i;
	scanf("%d", &i);
	printf("%d\n", fibonacci(i));
}
