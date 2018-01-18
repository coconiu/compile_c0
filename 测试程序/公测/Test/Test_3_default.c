#include<stdio.h>
int NsquareSum(int temp,int times){
    int noneUse[10];
    int Temp;
    Temp=temp;
    if(times==1)
        return (temp);
    Temp=Temp*NsquareSum(temp,times-1);
    return (((Temp)));
}
int a3[10];
int main(){
    int temp1,temp2;
    int num;
    int i;
    scanf("%d",&num);
	if(num>0){
				temp2=0;
				for(i=0;i<num;i=i+1){
					scanf("%d", &temp1);
					temp2=temp2+NsquareSum(temp1,num);
				}
				a3[0]=temp2;
				printf("result:%d",a3[0]);
			}
} 
