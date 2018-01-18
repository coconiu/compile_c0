#include <stdio.h>
int getsqr(int n){
    int _i;
    
    for(_i = 1;_i < n / _i;_i =  _i+1) ;
    if(-n+_i*_i < n-(-1+_i)*(_i-1)) return (_i);    
    return (_i-1);
}
int main(){
	int i;
	while(1){
		scanf("%d", &i);
		printf("%d\n", getsqr(i));
	}
}
