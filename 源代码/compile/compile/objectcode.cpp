#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "grammer.h"

///////////////////////////////////////全局变量
FILE *fpMips = fopen("mips.txt","wt");
//typedef struct consttype{
//	char constID[256];
//	int constValue;
//	int addr;
//}constType;
//
//typedef struct vartype{
//	char varID[256];
//	int addr;
//}varType;
///////////////////////////////////////////////
typedef struct Stack{
	/*constType constArea[128];
	varType varArea[128];
	char stackID[256];
	int constNum ;
	int varNum;
	int size;
	int offset;*/
	char stackFunc[256];
	char name[256][256];
	int addr[256];
	int num;
}runStack;

char nowFuncID[256];

runStack runstack[256];
int runStackNum = 0;
///////////////////////////////////////////先建的符号表

////////////////////////////////////////////////////
///////////////////这部分无用
int constNum;
int varNum;
int size;
int offset;
char constID[256];
char varID[256];
char stackID[256];
int addr;
int constValue;
////////////////////////////////////////
int p = 0;
int q = 0;
int doneNum = 0;
int pushNum = 0;
int paraNum = 0;
char funcName[256];
int funcNum;
int callStack[256];
int callStackNum = 0;
int stackOffset1[256];
int stackOffset1Num = 0;

int stackOffset = 8;

int findResult = 0;
int isMain = 0;
typedef struct stackVar{
	char name[256];
	int addr;
}stackvar;
stackvar conVar[512];
int areaNum = 0;

char glo[256][256];
int gloNum = 0;

char paraStack[256][256];///////////////////////////////////////////////////////////////////////////////////////////
int paraStackNum;/////////////////////////////////////////////////////////////////////////////////////////////
int paraType[256];
///////////////////////////////////////函数声明

///////////////////////////////////////函数实现
int isNum(char m){
	return ( (m>='0') && (m<='9') );
}
int findAddr(char *name){
	int lkj = runStackNum;
	int mn = 0;
	//for(lkj=1;lkj<=runStackNum;lkj++){
	//	if(strcmp(nowFuncID,runstack[lkj].stackFunc) == 0){
	//		break;	
	//	}else{
	//	
	//	}
	//}
	if(1){
		for(mn=0;mn<=runstack[lkj].num;mn++){
			if(strcmp(name,runstack[lkj].name[mn]) == 0){
				break;
			}else{
			
			}
		}
		if(mn<=runstack[lkj].num){
			return runstack[lkj].addr[mn];
		}else{
			return -1;
		}
	}else{
		return -1;
	}
	return -1;
}
void int_mips(){
	
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].mc);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
	
}
void char_mips(){
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].mc);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
}
void inta_mips(){


	stackOffset += 4*(atoi(midcode[doneNum].mc));
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].mb);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset-(4*(atoi(midcode[doneNum].mc)-1));
	/*fprintf(fpMips,"subi $sp $sp %d\n",4*(atoi(midcode[doneNum].mc)));*/
	runstack[runStackNum].num++;
	
}
void chara_mips(){
	stackOffset += 4*(atoi(midcode[doneNum].mc));
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].mb);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset-(4*(atoi(midcode[doneNum].mc)-1));
	/*fprintf(fpMips,"subi $sp $sp %d\n",4*(atoi(midcode[doneNum].mc)));*/
	runstack[runStackNum].num++;
	
}

void add_mips(){
	int addr1;
	int addr2;
	int addr3;
	//////////////操作数1
	if(isNum(midcode[doneNum].mb[0]) || midcode[doneNum].mb[0] == '-'){
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
	}else{
		int zxh = 0;
		/*for(zxh=0;zxh<gloNum;zxh++){
			if(strcmp(glo[zxh],midcode[doneNum].mb) == 0){
				break;
			}else{
				
			}
		}*/
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){//////全局变量
			//fprintf(fpMips,"la $t0 %s",midcode[doneNum].mb);
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
		}	
	}
	///////////////操作数2
	if(isNum(midcode[doneNum].mc[0]) || midcode[doneNum].mc[0] == '-'){
		fprintf(fpMips,"li $t1 %d\n",atoi(midcode[doneNum].mc));
	}else{
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){///////全局变量
			fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t1 %d($fp)\n",addr2);
		}
	}
	///////////////add
	fprintf(fpMips,"add $t0 $t0 $t1\n");
	///////////////result
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
	
	fprintf(fpMips,"sw $t0 %d($fp)\n",(-1)*stackOffset);
	
	
	//addr3 = (-1) * findAddr(midcode[doneNum].ma);
	//if(addr3 == 1){
	//	fprintf(fpMips,"sw $t0 %s\n",midcode[doneNum].ma);
	//}else{
	//	fprintf(fpMips,"sw $t0 %d($fp)\n",addr3);
	//}
}
void minus_mips(){
	int addr1;
	int addr2;
	int addr3;
	//////////////操作数1
	if(isNum(midcode[doneNum].mb[0]) || midcode[doneNum].mb[0] == '-'){
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
	}else{
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){//////全局变量
			//fprintf(fpMips,"la $t0 %s",midcode[doneNum].mb);
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
		}
	}
	///////////////操作数2
	if(isNum(midcode[doneNum].mc[0]) || midcode[doneNum].mc[0] == '-'){
		fprintf(fpMips,"li $t1 %d\n",atoi(midcode[doneNum].mc));
	}else{
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){///////全局变量
			fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t1 %d($fp)\n",addr2);
		}
	}
	///////////////sub
	fprintf(fpMips,"sub $t0 $t0 $t1\n");
	///////////////result
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
	
	fprintf(fpMips,"sw $t0 %d($fp)\n",(-1)*stackOffset);
	
}
void mul_mips(){
	int addr1;
	int addr2;
	int addr3;
	//////////////操作数1
	if(isNum(midcode[doneNum].mb[0]) || midcode[doneNum].mb[0] == '-'){
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
	}else{
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){//////全局变量
			//fprintf(fpMips,"la $t0 %s",midcode[doneNum].mb);
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
		}
	}
	///////////////操作数2
	if(isNum(midcode[doneNum].mc[0]) || midcode[doneNum].mc[0] == '-'){
		fprintf(fpMips,"li $t1 %d\n",atoi(midcode[doneNum].mc));
	}else{
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){///////全局变量
			fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t1 %d($fp)\n",addr2);
		}
	}
	/////////////mul
	fprintf(fpMips,"mul $t0 $t0 $t1\n");
	//////////////result
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	fprintf(fpMips,"subi $sp $sp 4\n");
	runstack[runStackNum].num++;
	
	fprintf(fpMips,"sw $t0 %d($fp)\n",(-1)*stackOffset);
	
}
void div_mips(){
	int addr1;
	int addr2;
	int addr3;
	//////////////操作数1
	if(isNum(midcode[doneNum].mb[0]) || midcode[doneNum].mb[0] == '-'){
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
	}else{
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){//////全局变量
			//fprintf(fpMips,"la $t0 %s",midcode[doneNum].mb);
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
		}
	}
	///////////////操作数2
	if(isNum(midcode[doneNum].mc[0]) || midcode[doneNum].mc[0] == '-'){
		fprintf(fpMips,"li $t1 %d\n",atoi(midcode[doneNum].mc));
	}else{
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){///////全局变量
			fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t1 %d($fp)\n",addr2);
		}
	}
	/////////////div
	fprintf(fpMips,"div $t0 $t0 $t1\n");
	///////////result
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
	
	fprintf(fpMips,"sw $t0 %d($fp)\n",(-1)*stackOffset);
	
}
void compare_mips(){
	int compare1 = 0;
	int compare2 = 0;
	int addr1 = 0;
	int addr2 = 0;
	int zxh = 0;
	int yq = 0;
	/*compare1 = atoi(midcode[doneNum].mb);
	compare2 = atoi(midcode[doneNum].mc);*/
	/////////////判断语句中的操作数mb
	if(isNum(midcode[doneNum].mb[0]) || (midcode[doneNum].mb[0] == '-')){
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
	}else{
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){////////全局变量
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
		}
	}
	//for(zxh=0;zxh<areaNum;zxh++){
	//	if(strcmp(midcode[doneNum].mb,conVar[zxh].name) == 0){
	//		break;
	//	}else{
	//	
	//	}
	//}
	//if(zxh!=areaNum){
	//	fprintf(fpMips,"lw $t0 %d($fp)\n",(-1)*(conVar[zxh].addr));
	//}else{
	//	for(yq=0;yq<gloNum;yq++){
	//		if(strcmp(midcode[doneNum].mb,glo[yq]) == 0){
	//			break;
	//		}else{
	//			
	//		}
	//	}
	//	if(yq != gloNum){
	//		fprintf(fpMips,"lw $t0 %s\n",glo[yq]);
	//	}else{
	//		////error;
	//	}
	//}
	/////////////判断语句中的操作数mc
	if(isNum(midcode[doneNum].mc[0]) || (midcode[doneNum].mc[0] == '-')){
		fprintf(fpMips,"li $t1 %d\n",atoi(midcode[doneNum].mc));
	}else{
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){////全局变量
			fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t1 %d($fp)\n",addr2);
		}
	}
	//for(zxh=0;zxh<areaNum;zxh++){
	//	if(strcmp(midcode[doneNum].mc,conVar[zxh].name) == 0){
	//		break;
	//	}else{
	//	
	//	}
	//}
	//if(zxh!=areaNum){
	//	fprintf(fpMips,"lw $t1 %d($fp)\n",(-1)*(conVar[zxh].addr));
	//}else{
	//	for(yq=0;yq<gloNum;yq++){
	//		if(strcmp(midcode[doneNum].mc,glo[yq]) == 0){
	//			break;
	//		}else{
	//			
	//		}
	//	}
	//	if(yq != gloNum){
	//		fprintf(fpMips,"lw $t1 %s\n",glo[yq]);
	//	}else{
	//		////error;
	//	}
	//}
	//////////////操作数mb,mc已经确定,操作数ma即比较的结果一定是自己定义的新变量，同上面add,minus,mul,div

	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
	

	if(midcode[doneNum].mType == 20){
		fprintf(fpMips,"slt $t0 $t0 $t1\n");
		/*fprintf(fpMips,"li $t0 %d\n",(compare1 < compare2));	*/
	}else if(midcode[doneNum].mType == 21){
		fprintf(fpMips,"sle $t0 $t0 $t1\n");
		/*fprintf(fpMips,"li $t0 %d\n",(compare1 <= compare2));*/	
	}else if(midcode[doneNum].mType == 22){
		fprintf(fpMips,"slt $t0 $t1 $t0\n");
		/*fprintf(fpMips,"li $t0 %d\n",(compare1 > compare2));*/	
	}else if(midcode[doneNum].mType == 23){
		fprintf(fpMips,"sle $t0 $t1 $t0\n");
		/*fprintf(fpMips,"li $t0 %d\n",(compare1 >= compare2));*/
	}else if(midcode[doneNum].mType == 24){
		fprintf(fpMips,"sne $t0 $t0 $t1\n");
		/*fprintf(fpMips,"li $t0 %d\n",(compare1 != compare2));*/
	}else if(midcode[doneNum].mType == 25){
		fprintf(fpMips,"seq $t0 $t0 $t1\n");
		/*fprintf(fpMips,"li $t0 %d\n",(compare1 == compare2));*/
	}
	
	fprintf(fpMips,"sw $t0 %d($fp)\n",(-1)*stackOffset);
	
}


void pri_mips(){
	int addr1;
	if(midcode[doneNum].mType == 42){
		fprintf(fpMips,"la $a0 %s\n",midcode[doneNum].mb);
		fprintf(fpMips,"li $v0 4\n");
		fprintf(fpMips,"syscall\n");
	}else if(midcode[doneNum].mType == 41){
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){
			fprintf(fpMips,"lw $a0 %s\n",midcode[doneNum].mb);
			fprintf(fpMips,"li $v0 1\n");
			fprintf(fpMips,"syscall\n");
		}else{
			fprintf(fpMips,"lw $a0 %d($fp)\n",addr1);
			fprintf(fpMips,"li $v0 1\n");
			fprintf(fpMips,"syscall\n");
		}
	}else if(midcode[doneNum].mType == 45){
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){
			fprintf(fpMips,"lw $a0 %s\n",midcode[doneNum].mb);
			fprintf(fpMips,"li $v0 11\n");
			fprintf(fpMips,"syscall\n");
		}else{
			fprintf(fpMips,"lw $a0 %d($fp)\n",addr1);
			fprintf(fpMips,"li $v0 11\n");
			fprintf(fpMips,"syscall\n");
		}
	}
}
void sca_mips(){
	int addr1;
	addr1 = (-1)*findAddr(midcode[doneNum].mb);
	if(midcode[doneNum].mType == 39){
		if(addr1 == 1){
			fprintf(fpMips,"li $v0 5\n");
			fprintf(fpMips,"syscall\n");
			fprintf(fpMips,"sw $v0 %s\n",midcode[doneNum].mb);
		}else {
			fprintf(fpMips,"li $v0 5\n");
			fprintf(fpMips,"syscall\n");
			fprintf(fpMips,"sw $v0 %d($fp)\n",addr1);
		}
	}else if(midcode[doneNum].mType == 46){
		if(addr1 == 1){
			fprintf(fpMips,"li $v0 12\n");
			fprintf(fpMips,"syscall\n");
			fprintf(fpMips,"sw $v0 %s\n",midcode[doneNum].mb);
		}else {
			fprintf(fpMips,"li $v0 12\n");
			fprintf(fpMips,"syscall\n");
			fprintf(fpMips,"sw $v0 %d($fp)\n",addr1);
		}
	}
}
void label_mips(){
	fprintf(fpMips,"%s:\n",midcode[doneNum].ma);
}
void jmp_mips(){//////////goto
	fprintf(fpMips,"j %s\n",midcode[doneNum].mb);
}
void bnz_mips(){
	int addr1;
	addr1 = (-1)*findAddr(midcode[doneNum].mb);
	if(addr1 == 1){
		fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
		fprintf(fpMips,"bgtz $t0 %s\n",midcode[doneNum].mc);
	}else {
		fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
		fprintf(fpMips,"bgtz $t0 %s\n",midcode[doneNum].mc);
	}
}
void bz_mips(){
	int addr1;
	addr1 = (-1)*findAddr(midcode[doneNum].mb);
	if(addr1 == 1){
		fprintf(fpMips,"li $t0 1\n");
		fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mb);
		fprintf(fpMips,"bne $t0 $t1 %s\n",midcode[doneNum].mc);
	}else{
		fprintf(fpMips,"li $t0 1\n");
		fprintf(fpMips,"lw $t1 %d($fp)\n",addr1);
		fprintf(fpMips,"bne $t0 $t1 %s\n",midcode[doneNum].mc);
	}
	
}
void ass_mips(){
	int addr1 = 0;
	int addr2 = 0;
	int addr3 = 0;
	int yq = 0;

	if(midcode[doneNum].mType == 29){
		//////////a equal b
		addr1 = (-1)*findAddr(midcode[doneNum].ma);
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr2);
		}
		if(addr1 == 1){
			for(yq=0;yq<=gloNum;yq++){
				if(strcmp(midcode[doneNum].ma,glo[yq]) == 0){
					break;
				}else{
			
				}
			}
			if(yq > gloNum){/////////新变量
				stackOffset += 4;
				strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
				runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
				/*fprintf(fpMips,"subi $sp $sp 4\n");*/
				runstack[runStackNum].num++;
		
				addr1 = (-1)*stackOffset;
				fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
			}else{///////////全局变量
				fprintf(fpMips,"sw $t0 %s\n",midcode[doneNum].ma);
			}
		}else{
			fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
		}
	}else if(midcode[doneNum].mType == 30){
		////////a=b[i]
		/////// a b i
		addr1 = (-1)*findAddr(midcode[doneNum].ma);
		addr2 = (-1)*findAddr(midcode[doneNum].mb);
		addr3 = (-1)*findAddr(midcode[doneNum].mc);
		if(addr2 == 1){//////全局数组变量
			fprintf(fpMips,"la $t0 %s\n",midcode[doneNum].mb);
			if(addr3 == 1){
				fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
			}else{
				fprintf(fpMips,"lw $t1 %d($fp)\n",addr3);
			}
			
			fprintf(fpMips,"li $t2 4\n");
			fprintf(fpMips,"mul $t1 $t1 $t2\n");

			fprintf(fpMips,"add $t0 $t0 $t1\n");
			fprintf(fpMips,"lw $t0 0($t0)\n");
		}else{////////局部数组变量
			fprintf(fpMips,"li $t0 %d\n",addr2);
			if(addr3 == 1){
				fprintf(fpMips,"lw $t1 %s\n",midcode[doneNum].mc);
			}else{
				fprintf(fpMips,"lw $t1 %d($fp)\n",addr3);
			}
			fprintf(fpMips,"li $t2 4\n");
			fprintf(fpMips,"mul $t1 $t1 $t2\n");

			fprintf(fpMips,"sub $t0 $t0 $t1\n");
			fprintf(fpMips,"add $t0 $t0 $fp\n");
			fprintf(fpMips,"lw $t0 0($t0)\n");
		}
		if(addr1 == 1){
			for(yq=0;yq<=gloNum;yq++){
				if(strcmp(midcode[doneNum].ma,glo[yq]) == 0){
					break;
				}else{
			
				}
			}
			if(yq > gloNum){/////////新变量
				stackOffset += 4;
				strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
				runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
				/*fprintf(fpMips,"subi $sp $sp 4\n");*/
				runstack[runStackNum].num++;
		
				addr1 = (-1)*stackOffset;
				fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
			}else{///////////全局变量
				fprintf(fpMips,"sw $t0 %s\n",midcode[doneNum].ma);
			}
		}else{
			fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
		}
	}else if(midcode[doneNum].mType == 31){
		//////////a[i] = b;
		//////////a i b
		addr1 = (-1)*findAddr(midcode[doneNum].ma);
		addr2 = (-1)*findAddr(midcode[doneNum].mc);
		addr3 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr2 == 1){
			fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mc);
		}else{
			fprintf(fpMips,"lw $t0 %d($fp)\n",addr2);
		}
		if(addr1 == 1){
			fprintf(fpMips,"la $t1 %s\n",midcode[doneNum].ma);
			if(addr3 == 1){
				fprintf(fpMips,"lw $t2 %s\n",midcode[doneNum].mb);
			}else{
				fprintf(fpMips,"lw $t2 %d($fp)\n",addr3);
			}
			
			fprintf(fpMips,"li $t3 4\n");
			fprintf(fpMips,"mul $t2 $t2 $t3\n");
			fprintf(fpMips,"add $t1 $t1 $t2\n");
			fprintf(fpMips,"sw $t0 0($t1)\n");
		}else{
			fprintf(fpMips,"li $t1 %d\n",addr1);
			if(addr3 == 1){
				fprintf(fpMips,"lw $t2 %s\n",midcode[doneNum].mb);
			}else{
				fprintf(fpMips,"lw $t2 %d($fp)\n",addr3);
			}
			fprintf(fpMips,"li $t3 4\n");
			fprintf(fpMips,"mul $t2 $t2 $t3\n");
			fprintf(fpMips,"sub $t1 $t1 $t2\n");

			
			fprintf(fpMips,"add $t1 $t1 $fp\n");
			fprintf(fpMips,"sw $t0 0($t1)\n");
		}
	}else if(midcode[doneNum].mType == 40){
		////a = i
		////a equal i
		int yq = 0;
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mc));
		addr1 = (-1)*findAddr(midcode[doneNum].ma);
		if(addr1 == 1){
			for(yq=0;yq<=gloNum;yq++){
				if(strcmp(midcode[doneNum].ma,glo[yq]) == 0){
					break;
				}else{
			
				}
			}
			if(yq > gloNum){/////////新变量
				stackOffset += 4;
				strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
				runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
				/*fprintf(fpMips,"subi $sp $sp 4\n");*/
				runstack[runStackNum].num++;
		
				addr1 = (-1)*stackOffset;
				fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
			}else{///////////全局变量
				fprintf(fpMips,"sw $t0 %s\n",midcode[doneNum].ma);
			}
		}else{/////////局部变量
			fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
		}
	}else{
		////不需要处理
	}
}


void retCall_mips(){/////i equal RET
	/*int lt = 0;
	for(lt=callStack[callStackNum];lt<=areaNum;lt++){
		memset(conVar[lt].name,0,256);
		conVar[lt].addr = 0;
	}
	areaNum = callStack[callStackNum];
	callStack[callStackNum] = 0;
	callStackNum--;
	stackOffset = stackOffset1[stackOffset1Num];
	stackOffset1Num--;*/


	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].ma);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;

	/*fprintf(fpMips,"lw $t0 -4($sp)\n");*/
	fprintf(fpMips,"sw $v0 %d($fp)\n",(-1)*stackOffset);

	//for(i=callStack;i<=areaNum;i++){
	//	memset(conVar[i].name,0,256);
	//	conVar[i].addr = 0;
	//}
	//areaNum = callStack;
}
void ret_mips(){///////return (x);
	if(isMain == 0){
		int addr1 = 0;
		char place[256];
		strcpy(place,midcode[doneNum].mb);
		if((midcode[doneNum].mb[0] == '-') || isNum(midcode[doneNum].mb[0])){
			fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
			fprintf(fpMips,"move $v0 $t0\n");
		}else if(midcode[doneNum].mb[0] == ' '){
			
		}else{
			addr1 = (-1)*findAddr(midcode[doneNum].mb);
			if(addr1 == 1){
				fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
				fprintf(fpMips,"move $v0 $t0\n");
			}else{
				fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
				fprintf(fpMips,"move $v0 $t0\n");
			}
		}
		fprintf(fpMips,"lw $ra -4($fp)\n");
		fprintf(fpMips,"move $sp $fp\n");
		fprintf(fpMips,"lw $fp -8($fp)\n");
		fprintf(fpMips,"jr $ra\n");

		

		
	}else if(isMain == 1){
		fprintf(fpMips,"j %s\n",mainEndLabel);
		
		
		
	}
}

void func_mips(){
	runStackNum++;
	strcpy(runstack[runStackNum].stackFunc,midcode[doneNum].mb);
	strcpy(nowFuncID,midcode[doneNum].mb);
	
	stackOffset = 8;
	fprintf(fpMips,"sw $ra -4($fp)\n");
}
void funcCall_mips(){
	int i = 0;
	int lkj = 0;
	int callSize = 0;
	int paraSize = 0;
	
	searchSymTab(midcode[doneNum].mb,0,0,-1);///////得到searchParaNum
	callSize = searchParaNum;

	for(lkj=1;lkj<=callSize;lkj++){
		if(paraStack[paraStackNum-callSize+lkj][0] == '+'){
			paraSize = atoi(paraStack[paraStackNum-callSize+lkj]);
			fprintf(fpMips,"sw %d %d($sp)\n",paraSize,(-1)*((lkj)*4+8));
		}else if(isNum(paraStack[paraStackNum-callSize+lkj][0]) || (paraStack[paraStackNum-callSize+lkj][0] == '-')){
			fprintf(fpMips,"lw $t0 %s($fp)\n",paraStack[paraStackNum-callSize+lkj]);
			fprintf(fpMips,"sw $t0 %d($sp)\n",(-1)*((lkj)*4+8));
		}else{
			fprintf(fpMips,"lw $t0 %s\n",paraStack[paraStackNum-callSize+lkj]);
			fprintf(fpMips,"sw $t0 %d($sp)\n",(-1)*((lkj)*4+8));
		}
	}
	for(lkj=0;lkj<callSize;lkj++){
		memset(paraStack[paraStackNum-lkj],0,256);
	}

	paraStackNum = paraStackNum - callSize;

	fprintf(fpMips,"sw $fp -8($sp)\n");
	fprintf(fpMips,"add $fp $sp $0\n");
	fprintf(fpMips,"subi $sp $sp 4000\n");

	fprintf(fpMips,"jal %s\n",midcode[doneNum].mc);
	
	

}
//void para_mips(){
//	int addr1;
//	addr1 = (-1)*findAddr(midcode[doneNum].mc);
//	if(addr1 == 1{
//		fprintf(fpMips,"lw $t0 %s",midcode[doneNum].mc);
//	}else{
//		fprintf(fpMips,"lw $t0 %d($fp)",addr1);
//	}
//}
void paraPush_mips(){
	//int addr1;
	//char place1[256];
	//
	//pushNum+=4;
	//paraStackNum++;
	//if(isNum(midcode[doneNum].mb[0]) == 1 || (midcode[doneNum].mb[0] == '-')){
	//	fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mb));
	//	fprintf(fpMips,"sw $t0 %d($sp)\n",(-1)*(pushNum+8));
	//}else {
	//	addr1 = (-1)*findAddr(midcode[doneNum].mb);
	//	if(addr1 == 1){
	//		fprintf(fpMips,"lw $t0 %s\n",midcode[doneNum].mb);
	//		fprintf(fpMips,"sw $t0 %d($sp)\n",(-1)*(pushNum+8));
	//	}else{
	//		fprintf(fpMips,"lw $t0 %d($fp)\n",addr1);
	//		fprintf(fpMips,"sw $t0 %d($sp)\n",(-1)*(pushNum+8));
	//	}
	//	
	//}
	int addr1;
	
	paraStackNum++;
	
	if(isNum(midcode[doneNum].mb[0]) || (midcode[doneNum].mb[0] == '-')){
		sprintf(paraStack[paraStackNum],"+%s",midcode[doneNum].mb);
	}else{
		addr1 = (-1)*findAddr(midcode[doneNum].mb);
		if(addr1 == 1){
			strcpy(paraStack[paraStackNum],midcode[doneNum].mb);
		}else{
			sprintf(paraStack[paraStackNum],"%d",addr1);
		}
	}
	
}
void para_mips(){///////已知searchParaNum
	//int addr1;
	//addr1 = (-1)*findAddr(midcode[doneNum].mc);
	//if(addr1 != 1){
	//	stackOffset += 4;
	//	strcpy(conVar[areaNum].name,midcode[doneNum].mc);
	//	conVar[areaNum].addr = stackOffset;
	//	fprintf(fpMips,"subi $sp $sp 4\n");
	//	areaNum++;
	//}else{}
	int addr1;
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].mc);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;
	
}
void funcConst_mips(){
	///////const int a = 1;
	///////int a 1 14
	///////const char a = 'z';
	///////char a z 15
	int addr1;
	
	stackOffset += 4;
	strcpy(runstack[runStackNum].name[runstack[runStackNum].num],midcode[doneNum].mb);
	runstack[runStackNum].addr[runstack[runStackNum].num] = stackOffset;
	/*fprintf(fpMips,"subi $sp $sp 4\n");*/
	runstack[runStackNum].num++;

	addr1 = (-1)*stackOffset;
	if(midcode[doneNum].mType == 14){
		fprintf(fpMips,"li $t0 %d\n",atoi(midcode[doneNum].mc));	
	}else{
		
		fprintf(fpMips,"li $t0 %s\n",midcode[doneNum].mc);
	}
	
	fprintf(fpMips,"sw $t0 %d($fp)\n",addr1);
}
////////////////////////////////////////////////主函数
void main(){///////////////////////////先处理全局变量放在.data里面，然后就开始处理函数定义以及函数调用没别的情况
	grammer();
	p = 0;
	fprintf(fpMips,".data\n");
	while((midcode[p].mType != 1) && (midcode[p].mType != 2) && (midcode[p].mType != 3) && (midcode[p].mType != 4)){
		int x = midcode[p].mType;
		if(midcode[p].mType == 14){
			searchSymTab(midcode[p].mb,1,0,-1);////得到searchValue
			fprintf(fpMips,"%s: .word %d\n",midcode[p].mb,searchValue);
			strcpy(glo[gloNum],midcode[p].mb);
			gloNum++;
		}else if(midcode[p].mType == 15){
			searchSymTab(midcode[p].mb,1,0,-1);////得到searchValue
			fprintf(fpMips,"%s: .word %d\n",midcode[p].mb,searchValue);
			strcpy(glo[gloNum],midcode[p].mb);
			gloNum++;
		}else if(midcode[p].mType == 10){
			//searchSymTab(midcode[i].mc,1,0,-1);
			fprintf(fpMips,"%s: .word 0\n",midcode[p].mc);
			strcpy(glo[gloNum],midcode[p].mc);
			gloNum++;
		}else if(midcode[p].mType == 11){
			fprintf(fpMips,"%s: .word 0\n",midcode[p].mc);
			strcpy(glo[gloNum],midcode[p].mc);
			gloNum++;
		}else if(midcode[p].mType == 12){
			fprintf(fpMips,"%s: .word 0:%d\n",midcode[p].mb,atoi(midcode[p].mc));
			strcpy(glo[gloNum],midcode[p].mc);
			gloNum++;
		}else if(midcode[p].mType == 13){
			fprintf(fpMips,"%s: .word 0:%d\n",midcode[p].mb,atoi(midcode[p].mc));
			strcpy(glo[gloNum],midcode[p].mc);
			gloNum++;
		}
		p++;
	}
	for(j=p;j<midCodeNum;j++){
		if(midcode[j].mType == 48){
			fprintf(fpMips,"%s: .asciiz \"%s\"\n",midcode[j].ma,midcode[j].mc);
			strcpy(glo[gloNum],midcode[p].mc);
			gloNum++;
		}else{
			
		}
	}
	doneNum = p-1;

	fprintf(fpMips,".text\n");
	fprintf(fpMips,"j %s\n",mainLabel);
	while(doneNum < midCodeNum){
		fprintf(fpMips,"#%d\n",doneNum+1);
		int sdk = midcode[doneNum].mType;
		if((midcode[doneNum].mType >=1) && (midcode[doneNum].mType <= 3)){
			func_mips();
			
		}else if(midcode[doneNum].mType == 4){
			isMain = 1;
			func_mips();
			fprintf(fpMips,"add $fp $sp $0\n");
			fprintf(fpMips,"subi $sp $sp 8000\n");
			/*strcpy(runstack[runStackNum].stackFunc,"main");
			strcpy(nowFuncID,"main");*/
			/*fprintf(fpMips,"add $fp $sp $0\n");*/
		}else if(midcode[doneNum].mType == 5 || midcode[doneNum].mType == 6){
			para_mips();
		}else if(midcode[doneNum].mType == 7){
			funcCall_mips();
		}else if(midcode[doneNum].mType == 8){
			paraPush_mips();
		}else if(midcode[doneNum].mType == 9){
			ret_mips();
		}else if(midcode[doneNum].mType == 10){
			int_mips();
		}else if(midcode[doneNum].mType == 11){
			char_mips();
		}else if(midcode[doneNum].mType == 12){
			inta_mips();
		}else if(midcode[doneNum].mType == 13){
			chara_mips();
		}else if((midcode[doneNum].mType == 14) || (midcode[doneNum].mType == 15)){
			funcConst_mips();
		}else if(midcode[doneNum].mType == 16){
			add_mips();
		}else if(midcode[doneNum].mType == 17){
			minus_mips();
		}else if(midcode[doneNum].mType == 18){
			mul_mips();
		}else if(midcode[doneNum].mType == 19){
			div_mips();
		}else if((midcode[doneNum].mType >= 20) && (midcode[doneNum].mType <=25)){
			compare_mips();
		}else if(midcode[doneNum].mType == 26){
			jmp_mips();
		}else if(midcode[doneNum].mType == 27){
			bnz_mips();
		}else if(midcode[doneNum].mType == 28){
			bz_mips();
		}else if((midcode[doneNum].mType >= 29) && (midcode[doneNum].mType <= 31) || (midcode[doneNum].mType ==40)){
			ass_mips();
		}else if(midcode[doneNum].mType == 34 || midcode[doneNum].mType == 35 || midcode[doneNum].mType == 43){
			fprintf(fpMips,"%s:\n",midcode[doneNum].ma);
		}else if(midcode[doneNum].mType == 36 || midcode[doneNum].mType == 37 || midcode[doneNum].mType == 38){
			fprintf(fpMips,"%s:\n",midcode[doneNum].mc);
		}else if(midcode[doneNum].mType == 39 || midcode[doneNum].mType == 46){
			sca_mips();
		}else if(midcode[doneNum].mType == 41 || midcode[doneNum].mType == 42 || midcode[doneNum].mType == 45){
			pri_mips();
		}else if(midcode[doneNum].mType == 44){
			retCall_mips();
		}else if(midcode[doneNum].mType == 47){
			funcCall_mips();
		}
		doneNum++;
	}
	

}