#include <iostream>
using namespace std;
FILE *f;
void move(char a, char b){        
    const int testcon1 = 1;        
    const char testcon2 = '1';
    fprintf(f,"%c\n",'M');    
    fprintf(f,"OVE \n");
    fprintf(f,"%c\n",a);
    fprintf(f," TO %c\n",b);
    fprintf(f," \n");
} 
void hanoi(int hano, char l, char m, char r){    
    if(2>hano) {
        move(l,r);
        return;
        printf("RETURN ERROR"); 
    }
    hanoi(hano-1, l, r, m);     
    move(l,r);        
    hanoi(hano-1, m, l, r);
}
int main(){
	f = fopen("Test_4_hanoi.txt", "w");
	int i;
	cin >> i;
	hanoi(i,'A','B','C');
}
