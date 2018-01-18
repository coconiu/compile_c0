#ifndef MID
#define MID
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define maxMidCode 1024

typedef struct midcode{
	char ma[256];
	char mb[256];
	char mc[256];
	int mType;
}fourVarCode;
fourVarCode midcode[maxMidCode];
int midCodeNum = 0;
int varNum = 0;
int labelNum = 0;
FILE *fpMid = fopen("midcode.txt","w");
void genMidCode(char *ma,char *mb,char *mc,int mType){
	strcpy(midcode[midCodeNum].ma,ma);
	strcpy(midcode[midCodeNum].mb,mb);
	strcpy(midcode[midCodeNum].mc,mc);
	midcode[midCodeNum].mType = mType;
	if(fpMid != NULL){
		fprintf(fpMid,"%s,",ma);
		fprintf(fpMid,"%s,",mb);
		fprintf(fpMid,"%s,",mc);
		fprintf(fpMid,"%d\n",mType);
	}
}

char *genVar(){
	char *var = (char *)malloc(sizeof(char)*16);
	sprintf(var,"tempVar%d",varNum);
	varNum++;
	return var;
}

char *genLabel(){
	char *label = (char *)malloc(sizeof(char)*16);
	sprintf(label,"label%d :",labelNum);
	labelNum++;
	return label;
}
#endif