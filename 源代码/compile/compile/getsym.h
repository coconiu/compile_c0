#ifndef GETSYM
#define GETSYM
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "error.h"
/*#define VOIDSYM		void
#define MAINSYM		main
#define INTSYM		int
#define CHARSYM		char
#define CONSTSYM	const
#define IFSYM		if
#define ELSESYM		else
#define FORSYM		for
#define SWITCHSYM	switch
#define CASESYM		case
#define DEFAULTSYM	default
#define SCANFSYM	scanf
#define PRINTFSYM	printf
#define RETURNSYM	return*/


#define symLength 20
#define tokenLength 1024
#define strLength 512
char *sym[symLength] ={
					"void",		//1
					"main",		//2
					"int",		//3
					"char",		//4
					"const",	//5
					"if",		//6
					"else",		//7
					"for",		//8
					"switch",	//9
					"case",		//10
					"default",	//11
					"scanf",	//12
					"printf",	//13
					"return"	//14
				};
char *symbol[symLength] = {
							"VOIDSYM",		//1
							"MAINSYM",		//2
							"INTSYM",		//3
							"CHARSYM",		//4
							"CONSTSYM",		//5
							"IFSYM",		//6
							"ELSESYM",		//7
							"FORSYM",		//8
							"SWITCHSYM",	//9
							"CASESYM",		//10
							"DEFAULTSYM",	//11
							"SCANFSYM",		//12
							"PRINTFSYM",	//13
							"RETURNSYM"		//14
						};
char token[tokenLength] = {'\0'};
char str[strLength] = {'\0'};
char ch;
char *symid;
int length = 0;
int i =0,j=0;
int row;					//row用以记录词法分析结果的行数
int errorRow=0;				//errorRow用以记录出错的行数
int errorCol=0;				//errorCol用以记录出错的列数
FILE *fp;

int isSpace(char ch){
	if(ch==' '){
		return 1;
	}
	return 0;
}
int isTab(char ch){
	if(ch=='	'){
		return 1;
	}
	return 0;
}
int isLetter(char ch){
	if((ch>=65)&&(ch<=90) || (ch>=97)&&(ch<=122) || ch=='_'){
		return 1;
	}
	return 0;
}
int isSpeLetter(char ch){
	if(ch == 32 || ch == 33 || (ch>=35)&&(ch<=126)){
		return 1;
	}
	return 0;
}
int isDigit(char ch){
	if((ch>=48)&&(ch<=57)){
		return 1; 
	}
	return 0;
}
int isPlus(char ch){
	if(ch == '+'){
		return 1;
	}
	return 0;
}
int isMinus(char ch){
	if(ch == '-'){
		return 1;
	}
	return 0;
}
int isMul(char ch){
	if(ch == '*'){
		return 1;
	}
	return 0;
}
int isDiv(char ch){
	if(ch == '/'){
		return 1;
	}
	return 0;
}
int isExc(char ch){
	if(ch=='!'){
		return 1;
	}
	return 0;
}
int isLess(char ch){
	if(ch == '<'){
		return 1;
	}
	return 0;
}
int isMore(char ch){
	if(ch == '>'){
		return 1;
	}
	return 0;
}
int isEqual(char ch){
	if(ch == '='){
		return 1;
	}
	return 0;
}
int isSinQuo(char ch){
	if(ch == '\''){
		return 1;
	}
	return 0;
}
int isDouQuo(char ch){
	if(ch == '\"'){
		return 1;
	}
	return 0;
}
int isSemi(char ch){
	if(ch == ';'){
		return 1;
	}
	return 0;
}
int isLBRA1(char ch){
	if(ch == '('){
		return 1;
	}
	return 0;
}
int isRBRA1(char ch){
	if(ch == ')'){
		return 1;
	}
	return 0;
}
int isLBRA2(char ch){
	if(ch == '['){
		return 1;
	}
	return 0;
}
int isRBRA2(char ch){
	if(ch == ']'){
		return 1;
	}
	return 0;
}
int isLBRA3(char ch){
	if(ch == '{'){
		return 1;
	}
	return 0;
}
int isRBRA3(char ch){
	if(ch == '}'){
		return 1;
	}
	return 0;
}
int isColon(char ch){
	if(ch == ':'){
		return 1;
	}
	return 0;
}
int isComma(char ch){
	if(ch == ','){
		return 1;
	}
	return 0;
}
int reserver(){
	for(i=0;i<14;i++){
		if(strcmp(token,sym[i]) == 0){
			return i;
		}
	}
	return -1;
}
int isNewline(char ch){
	return ch=='\n';
}
void catToken(char ch){
	length = strlen(token);
	token[length] = ch;
}
void clearToken(){
	memset(token,0,tokenLength);
}
void getsym(){
	//FILE *fp;
	int resultValue = 0;
	i = 0;
	clearToken();
	ch = fgetc(fp);
			
		/*while(isNewline(ch)){
			ch = fgetc(fp);
			errorCol = 0;
			errorRow++;
		}
		while(isSpace(ch)||isTab(ch)){
			ch = fgetc(fp);
		}*/
	while(isNewline(ch) || isSpace(ch) || isTab(ch)){
		if(isNewline(ch)){
			errorCol = 0;
			errorRow++;
		}
		ch = fgetc(fp);
	}
	if(isLetter(ch)){
		while(isLetter(ch)||isDigit(ch)){
			catToken(ch);
			ch = fgetc(fp);
		}
		if(ch != EOF){
			fseek(fp,-1,1);
		}
		resultValue = reserver();
		if(resultValue == -1) {
			errorCol++;
			symid = "IDEN";
		}else{
			errorCol++;
			symid = symbol[resultValue];
		}
	}else if(isDigit(ch)){
		while(isDigit(ch)){
			catToken(ch);
			ch = fgetc(fp);
		}
		if(isLetter(ch)){
			error(0,errorRow,errorCol);////////////////////数字开头的标识符
			while(isLetter(ch) || isDigit(ch)){
				ch = fgetc(fp);
			}
		}else{
			if(ch != EOF){
				fseek(fp,-1,1);
			}
			//if(token[0] == '0'){
			//	error(1,errorRow,errorCol);////////////////
			errorCol++;
			symid = "UNINT";
		}		
	}else if(isSinQuo(ch)){
		char zifu;
		ch = fgetc(fp);
		catToken(ch);
		zifu = ch;
		if(isPlus(ch) || isMinus(ch) || isMul(ch) || isDiv(ch) || isDigit(ch) || isLetter(ch)){
			ch = fgetc(fp);
			if(isSinQuo(ch)){
				errorCol++;
				symid = "ZIFU";
			}else{////单引号不匹配
				error(6,errorRow,errorCol);
				fseek(fp,-1,1);
			}
		}
	}else if(isDouQuo(ch)){
		ch = fgetc(fp);
		while(isSpeLetter(ch)){
			catToken(ch);
			ch = fgetc(fp);
					
		}
		if(isDouQuo(ch)){
			errorCol++;
			symid = "STRING";
		}else{///////双引号不匹配
			error(7,errorRow,errorCol);
			fseek(fp,-1,1);
		}
	}
	else if(isPlus(ch)){
		errorCol++;
		symid = "PLUS";
	}else if(isMinus(ch)){
		errorCol++;
		symid = "MINUS";
	}else if(isMul(ch)){
		errorCol++;
		symid = "MUL";
	}else if(isDiv(ch)){
		errorCol++;
		symid = "DIV";
	}else if(isLess(ch)){
		ch = fgetc(fp);
		if(ch=='='){
			errorCol++;
			symid = "LESSOREQUAL";
		}else{
			fseek(fp,-1,1);
			errorCol++;
			symid = "LESS";
		}
	}else if(isMore(ch)){
		ch = fgetc(fp);
		if(ch == '='){
			errorCol++;
			symid = "MOREOREQUAL";
		}else{
			fseek(fp,-1,1);
			errorCol++;
			symid = "MORE";
		}
	}else if(isEqual(ch)){
		ch = fgetc(fp);
		if(ch=='='){
			errorCol++;
			symid = "ALEQUAL";
		}else {
			fseek(fp,-1,1);
			errorCol++;
			symid = "EQUAL";
		}
	}else if(isExc(ch)){
		ch = fgetc(fp);
		if(ch=='='){
			errorCol++;
			symid = "NOTEQUAL";
		}else{
			fseek(fp,-1,1);
			error(2,errorRow,errorCol);///////////////////////////
		}
	}else if(isSemi(ch)){
		errorCol++;
		symid = "SEMI";
	}else if(isLBRA1(ch)){
		errorCol++;
		symid = "LBRA1";
	}else if(isRBRA1(ch)){
		errorCol++;
		symid = "RBRA1";
	}else if(isLBRA2(ch)){
		errorCol++;
		symid = "LBRA2";
	}else if(isRBRA2(ch)){
		errorCol++;
		symid = "RBRA2";
	}else if(isLBRA3(ch)){
		errorCol++;
		symid = "LBRA3";
	}else if(isRBRA3(ch)){
		errorCol++;
		symid = "RBRA3";
	}else if(isColon(ch)){
		errorCol++;
		symid = "COLON";
	}else if(isComma(ch)){
		errorCol++;
		symid = "COMMA";
	}else if(ch == EOF){
		symid = "END";
	}
	else{
		error(3,errorRow,errorCol);//////////////////////////错误字符
	}
}
#endif

