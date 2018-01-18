#ifndef GRAMMER
#define GRAMMER
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "getsym.h"
#include "midcode.h"

#define idLength 50
#define symtabLength 512
#define maxPre 512

typedef struct{
	char id[idLength];
	int type;		//0--const,1--var,2--func,3--para
	int value;		//常量的值，type为函数时，0表示void，1表示int，2表示char
	int address;	//标识符存储地址或地址偏移			
	int para;		//函数的参数个数 或 数组的长度
	char funcLabelGlo[256]; ////函数的地址（第一条语句）
}Symbol;

typedef struct{
	Symbol symtab[symtabLength];	//符号表
	int index;						//符号表栈顶指针，即符号表中的符号总数量 /////最开始为0
	int blocksnum;					//当前符号表拥有的分程序总数 /////////////////最开始为0
	int blocksindex[maxPre];		//分程序索引					////symTab.blocksindex[0] = 1;
}SymbolTab;

//////////////////////全局变量声明
char id[idLength];
int type;
int value;
int address;
int para;
char labelAdd[256];
Symbol symtab[symtabLength];
int index;
int blocksnum;
int blocksindex;
SymbolTab symTab;///////符号表



/////////////预读取的标识符串和标识符的种类
 char temp1[128],temp2[128];
 //////////char *symid1,*symid2;牢记少用指针，用指针要malloc，不然是野指针
 char tempID[128];///////////用于因子中向外传递的指针
 int isArr = 0;
 int funcOrIden;
 int subScript;
 int searchValue;
 int searchAdd;
 char searchLabel[256];
 int isFirstBlock;

 int arrNum = 0;
 int t;
 char nowTerm[128];
 int isConst;
 char tempCon[128];
 char tempFunc[128];
 
 char funcLabelGlo[256] = {0};
 int tempStep = 0;
 char condition01[256];
 char funcDefRet[256];
 char funcCallTemp[256];
 char senID[256];
 char constantNum[256];
 char switchExp[256];
 char endLabel[256];
////////////函数的声明
 void insertSymTab(char *id,int type,int value,int address,int para,char *labelAdd);
 void searchSymTab(char *id,int funcOrIden,int isArr,int subScript);
 void initSymTab();

void Exp();
void Sentence();
void Factor();
void SentenceList();
void Term();
void ConDef();
void ConExplain();

void VarDefChar();
void VarDefInt();
void VarExplain();

void Cycle();
void Condition();
void Assign();
void FuncCall1();
void FuncCall0();
void Scanf();
void Printf();
void Return();
void Main();
void FuncDef1();
void FuncDef0();
void Default();
void Integer();
void Constant();
void Case();
void Switch();
void ComSen();
void Step();
void CondiSen();
///////////函数定义
void insertSymTab(char *id,int type,int value,int address,int para,char *funcLabelGlo){
	if(type == 2){////////////函数名，只需查找各个分程序的入口即函数入口的名称是否重复即可
		for(i=1;i<symTab.blocksnum;i++){///////////不需要从0开始，因为第一层还没有任何函数 不可能重名
			if(strcmp(id,symTab.symtab[symTab.blocksindex[i]].id) == 0){
				//////error
				return;
			}
		}
		blocksnum++;
	}else {////////////////变量名，需查找该层符号表中是否有重复
		i = symTab.blocksindex[symTab.blocksnum];
		for(;i<symTab.index;i++){
			if(strcmp(id,symTab.symtab[i].id) == 0){
				////////error
				return;
			}
		}
	}
	////////////若无符号表重名 则开始填符号表
	strcpy(symTab.symtab[index-1].id,id);
	symTab.symtab[index-1].type = type;
	symTab.symtab[index-1].value = value;
	symTab.symtab[index-1].address = address;
	symTab.symtab[index-1].para = para;
	strcpy(symTab.symtab[index-1].funcLabelGlo,funcLabelGlo);
	symTab.index++;
}
void init(){
	symTab.blocksnum = 0;
	symTab.index = 0;
	symTab.blocksindex[0] = 1;
}
void searchSymTab(char *id,int funcOrIden,int isArr,int subScript){//////////subScript 是数组下标，若不是数组，则置为-1
	////////////////////////////////如果是int或char 就以int的类型返回回去，若是数组，就结合数组下标返回数组某一项的值,并返回是否是在第一层
	////////////////////////////////如果是数组名
	if(funcOrIden == 0){////func
		for(i=1;i<symTab.blocksnum;i++){
			if(strcmp(id,symTab.symtab[symTab.blocksindex[i]].id) == 0){
				strcpy(funcLabelGlo,symTab.symtab[symTab.blocksindex[i]].funcLabelGlo);
				strcpy(searchLabel,funcLabelGlo);
				searchAdd = symTab.symtab[symTab.blocksindex[i]].address;
				searchValue = -1;
				isFirstBlock = 0;
				break;
			}
		}
		if(i == symTab.blocksnum){
			//////////error();		///////未声明过的函数名
		}
		if(symTab.symtab[symTab.blocksindex[i]].para != para){
			//////////error();		//////声明过得函数，但是参数个数不一致
		}
		
	}else {////////////////////char or int//const or var
		i = symTab.blocksindex[blocksnum];
		for(;i<index;i++){//////////////在当前符号表查询
			if(strcmp(id,symtab[i].id) == 0){
				isConst = (symtab[i].type == 0);
				searchAdd = symtab[i].address;
				searchValue = symtab[i].value;
				isFirstBlock = (blocksnum == 0);
				break;
			}
		}
		if(i==index){/////////////////到全局符号表查询
			for(i=1;i<symTab.blocksindex[1];i++){
				if(strcmp(symtab[i].id,id) == 0){
					isConst = (symtab[i].type == 0);
					searchAdd = symtab[i].address;
					searchValue = symtab[i].value;
					isFirstBlock = 1;
					break;
				}
			}
			if(i==symTab.blocksindex[1]){
				///////////////////////未声明的标识符
				///////////////////////error;
			}
		}
	}
}

void Integer(){
	int integerNum = 0;
	if(strcmp(symid,"PLUS") == 0){
		getsym();
		if(strcmp(symid,"UNINT") == 0){
			integerNum = atoi(token);
			sprintf(nowTerm,"%d",integerNum);
			getsym();
		}
	}else if(strcmp(symid,"MINUS") == 0){
		getsym();
		if(strcmp(symid,"UNINT") == 0){
			integerNum = (-1)*atoi(token);
			sprintf(nowTerm,"%d",integerNum);
			getsym();
		}
	}else if(token[0] == '0' || strcmp(symid,"UNINT") == 0){
		integerNum = atoi(token);
		sprintf(nowTerm,"%d",integerNum);
		getsym();
	}
}
void Factor(){/////////////因子
	char place3[256] = {0};
	if(strcmp(symid,"IDEN") == 0){
		strcpy(tempID,token);
		getsym();
		if(strcmp(symid,"LBRA2") == 0){////////数组
			char tempExpTerm[256];
			funcOrIden = 1;
			isArr = 1;
			getsym();
			Exp();/////////
			strcpy(tempExpTerm,nowTerm);
			///////////////nowTerm中已经存储了相应的项
			searchSymTab(tempID,funcOrIden,isArr,atoi(tempExpTerm));

			if(strcmp(symid,"RBRA2") == 0){
				strcpy(nowTerm,genVar());
				genMidCode(nowTerm,"equal",tempExpTerm,29);
				
				getsym();
			}
		}else if(strcmp(symid,"LBRA1") == 0){////////函数调用
			funcOrIden = 0;
			FuncCall1();///////函数调用中完成参数和call的中间代码,且此时的nowTerm 已经是return的值了
			

			strcpy(place3,genVar());
			genMidCode(place3,"equal",nowTerm,29);
			strcpy(nowTerm,place3);///////////此时已经完成了中间临时变量的赋值，临时变量成为nowTerm
		}else{//////IDEN
			searchSymTab(tempID,1,0,-1);
			char tempCon[256];
			if(isConst){
				itoa(searchValue,tempCon,10);
				strcpy(nowTerm,tempCon);
				isConst = 1;
			}else{
				strcpy(nowTerm,tempID);
			}
		}
	}else if(strcmp(symid,"ZIFU") == 0){
		sprintf(nowTerm,"%d",token[0]);
		
		getsym();
	}else if(strcmp(symid,"LBRA1") == 0){
		getsym();
		Exp();////////////nowTerm已经更新
		
		if(strcmp(symid,"RBRA1") == 0){
			getsym();
		}
	}else if((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0) || token[0] == '0' || strcmp(symid,"UNINT") == 0){
		Integer();////////////nowTerm更新
		
	}
}
void Term(){//////项
	char ttt[256] = {0};
	char mmm[256] = {0};
	Factor();
	
	strcpy(ttt,nowTerm);
	while(1){
		if(strcmp(symid,"MUL") == 0){
			getsym();
			Factor();
			
			strcpy(mmm,nowTerm);
			genMidCode(ttt,"MUL",mmm,18);
			strcpy(nowTerm,genVar());
		}else if(strcmp(symid,"DIV") == 0){
			getsym();
			Factor();
			
			strcpy(mmm,nowTerm);
			genMidCode(ttt,"DIV",mmm,19);
			strcpy(nowTerm,genVar());
		}else{
			break;
		}
	}
}
void Exp(){//////表达式
	char expTemp[256];
	if(strcmp(symid,"MINUS") == 0){
		getsym();
		Term();
		
		genMidCode("0","MINUS",nowTerm,17);
		strcpy(nowTerm,genVar());
		strcpy(expTemp,nowTerm);
		while((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0)){
			if(strcmp(symid,"PLUS") == 0){
				getsym();
				Term();
				
				genMidCode(expTemp,"plus",nowTerm,16);
				strcpy(nowTerm,genVar());
			}else if(strcmp(symid,"MINUS") == 0){
				getsym();
				Term();
				
				genMidCode(expTemp,"minus",nowTerm,17);
				strcpy(nowTerm,genVar());
			}
		}
	}else if(strcmp(symid,"PLUS") == 0){
		getsym();
		Term();
		
		strcpy(expTemp,nowTerm);
		while((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0)){
			if(strcmp(symid,"PLUS") == 0){
				getsym();
				Term();
				
				genMidCode(expTemp,"plus",nowTerm,16);
				strcpy(nowTerm,genVar());
			}else if(strcmp(symid,"MINUS") == 0){
				getsym();
				Term();
				
				genMidCode(expTemp,"minus",nowTerm,17);
				strcpy(nowTerm,genVar());
			}
		}
	}else {
		Term();
		
		strcpy(expTemp,nowTerm);
		while((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0)){
			if(strcmp(symid,"PLUS") == 0){
				getsym();
				Term();
				
				genMidCode(expTemp,"plus",nowTerm,16);
				strcpy(nowTerm,genVar());
			}else if(strcmp(symid,"MINUS") == 0){
				getsym();
				Term();
				
				genMidCode(expTemp,"plus",nowTerm,17);
				strcpy(nowTerm,genVar());
			}
		}		
	}
}

void ConDef(){
	type = 0;
	getsym();
	while(1){
		if(strcmp(symid,"INTSYM") == 0){
			getsym();
			if(strcmp(symid,"IDEN") == 0){
				strcpy(id,token);
				getsym();
				if(strcmp(symid,"EQUAL") == 0){
					getsym();
					if(strcmp(symid,"PLUS") == 0){
						getsym();
						if(strcmp(symid,"UNINT") == 0){
							sprintf(tempCon,"%s",token);
							value = atoi(token);
							getsym();
							if(strcmp(symid,"COMMA") == 0){
								para = -1;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
								
								genMidCode("int",id,tempCon,14);
								getsym();
							}else{
								para = -1;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
								
								genMidCode("int",id,tempCon,14);

								break;
							}
						}
					}else if(strcmp(symid,"MINUS") == 0){
						if(strcmp(symid,"UNINT") == 0){
							value = 0-atoi(token);
							sprintf(tempCon,"%d",value);
							getsym();
							if(strcmp(symid,"COMMA") == 0){
								para = -1;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

								genMidCode("int",id,tempCon,14);

								getsym();
							}else{
								para = -1;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

								genMidCode("int",id,tempCon,14);
								break;
							}
						}
					}else if(strcmp(symid,"UNINT") == 0){
						value = atoi(token);
						sprintf(tempCon,"%d",value);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							para = -1;
							address++;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("int",id,tempCon,14);
							getsym();
						}else {
							para = -1;
							address++;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("int",id,tempCon,14);
							break;
						}
					}
				}
			}
		}else if(strcmp(symid,"CHARSYM") == 0){
			getsym();
			if(strcmp(symid,"IDEN") == 0){
				strcpy(id,token);
				getsym();
				if(strcmp(symid,"EQUAL") == 0){
					getsym();
					if(strcmp(symid,"ZIFU") == 0){
						value = token[0];
						sprintf(tempCon,"%s",token);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							address++;
							para = -1;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("char",id,tempCon,15);
							
						}else{
							address++;
							para = -1;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("char",id,tempCon,15);
							
							break;
						}
					}
				}
			}
		}
	}
}
void ConExplain(){
	while(strcmp(symid,"CONSTSYM") == 0){
		ConDef();
		getsym();
		printf("This is a ConExplain statement!\n");
	}
}

void VarDef(){
	char tempVarDef[256];
	if(strcmp(symid,"COMMA") == 0){
		address++;
		
		insertSymTab(id,type,value,address,para," ");
		printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
		
		if(strcmp(temp1,"INTSYM") == 0){
			genMidCode("var","int",id,10);
		}else if(strcmp(temp1,"CHARSYM") == 0){
			genMidCode("var","char",id,11);
		}else{
			////////error;
		}
		while(strcmp(symid,"COMMA") == 0){
			getsym();
			if(strcmp(symid,"IDEN") == 0){
				type = 1;
				strcpy(id,token);
				getsym();
				if(strcmp(symid,"LBRA2") == 0){
					getsym();
					if(strcmp(symid,"UNINT") == 0){
						strcpy(tempVarDef,token);
						para = atoi(token);
						getsym();
						if(strcmp(symid,"RBRA2") == 0){
							getsym();
							if(strcmp(symid,"COMMA") == 0){
								value = -1;
								address=address+para;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

								if(strcmp(temp1,"INTSYM") == 0){
									genMidCode("int",id,tempVarDef,12);
								}else if(strcmp(temp1,"CHARSYM") == 0){
									genMidCode("char",id,tempVarDef,13);
								}else{
								////////error;
								}
								getsym();
							}else if(strcmp(symid,"SEMI") == 0){
								value = -1;
								address = address + para;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

								if(strcmp(temp1,"INTSYM") == 0){
									genMidCode("int",id,tempVarDef,12);
								}else if(strcmp(temp1,"CHARSYM") == 0){
									genMidCode("char",id,tempVarDef,13);
								}else{
								////////error;
								}
								break;
							}
						}
					}
				}else if(strcmp(symid,"COMMA") == 0){
					value = -1;
					para = -1;
					address++;
					
					insertSymTab(id,type,value,address,para," ");
					printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

					if(strcmp(temp1,"INTSYM") == 0){
						genMidCode("var","int",id,10);
					}else if(strcmp(temp1,"CHARSYM") == 0){
						genMidCode("var","char",id,11);
					}else{
						////////error;
					}
					
				}else if(strcmp(symid,"SEMI") == 0){
					value = -1;
					para = -1;
					address++;
					
					insertSymTab(id,type,value,address,para," ");
					printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

					if(strcmp(temp1,"INTSYM") == 0){
						genMidCode("var","int",id,10);
					}else if(strcmp(temp1,"CHARSYM") == 0){
						genMidCode("var","char",id,11);
					}else{
						////////error;
					}
					break;
				}else{
					//////////error;
				}
			}
		}
	}else if(strcmp(symid,"LBRA2") == 0){
		type = 1;
		getsym();
		if(strcmp(symid,"UNINT") == 0){
			strcpy(tempVarDef,token);
			para = atoi(token);
			getsym();
			if(strcmp(symid,"RBRA2") == 0){
				getsym();
				if(strcmp(symid,"COMMA") == 0){
					value = -1;
					address = address+para;
					
					insertSymTab(id,type,value,address,para," ");
					printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
					
					if(strcmp(temp1,"INTSYM") == 0){
						genMidCode("int",id,tempVarDef,12);
					}else if(strcmp(temp1,"CHARSYM") == 0){
						genMidCode("char",id,tempVarDef,13);
					}else{
					////////error;
					}

					while(strcmp(symid,"COMMA") == 0){
						getsym();
						if(strcmp(symid,"IDEN") == 0){
							strcpy(id,token);
							type = 1;
							value = -1;
							getsym();
							if(strcmp(symid,"LBRA2") == 0){
								getsym();
								if(strcmp(symid,"UNINT") == 0){
									strcpy(tempVarDef,token);
									para = atoi(token);
									getsym();
									if(strcmp(symid,"RBRA2") == 0){
										getsym();
										if(strcmp(symid,"COMMA") == 0){
											address = address +para;
											insertSymTab(id,type,value,address,para," ");
											printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

											if(strcmp(temp1,"INTSYM") == 0){
												genMidCode("int",id,tempVarDef,12);
											}else if(strcmp(temp1,"CHARSYM") == 0){
												genMidCode("char",id,tempVarDef,13);
											}else{
											////////error;
											}

											getsym();
										}else if(strcmp(symid,"SEMI") == 0){
											address= address + para;
											
											insertSymTab(id,type,value,address,para," ");
											printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
											
											if(strcmp(temp1,"INTSYM") == 0){
												genMidCode("int",id,tempVarDef,12);
											}else if(strcmp(temp1,"CHARSYM") == 0){
												genMidCode("char",id,tempVarDef,13);
											}else{
											////////error;
											}
											break;
										}
									}
								}
							}else if(strcmp(symid,"COMMA") == 0){
								para = -1;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);


								if(strcmp(temp1,"INTSYM") == 0){
									genMidCode("var","int",id,10);
								}else if(strcmp(temp1,"CHARSYM") == 0){
									genMidCode("var","char",id,11);
								}else{
								////////error;
								}
								getsym();
							}else if(strcmp(symid,"SEMI") == 0){
								para = -1;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
								if(strcmp(temp1,"INTSYM") == 0){
									genMidCode("var","int",id,10);
								}else if(strcmp(temp1,"CHARSYM") == 0){
									genMidCode("var","char",id,11);
								}else{
								////////error;
								}
								break;
							}							
						}
					}
				}else if(strcmp(symid,"SEMI") == 0){
					value = -1;
					address = address + para;
					
					insertSymTab(id,type,value,address,para," ");
					printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

					if(strcmp(temp1,"INTSYM") == 0){
						genMidCode("var","int",id,10);
					}else if(strcmp(temp1,"CHARSYM") == 0){
						genMidCode("var","char",id,11);
					}else{
					////////error;
					}
				}
			}
		}
	}else if(strcmp(symid,"SEMI") == 0){
		para = -1;
		address++;
		strcpy(labelAdd," ");
		insertSymTab(id,type,value,address,para," ");
		printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

		if(strcmp(temp1,"INTSYM") == 0){
			genMidCode("var","int",id,10);
		}else if(strcmp(temp1,"CHARSYM") == 0){
			genMidCode("var","char",id,11);
		}else{
		////////error;
		}
	}
}
void VarExplain(){
	if(strcmp(symid,"COMMA") == 0 || strcmp(symid,"LBRA2") == 0){
		VarDef();
		if(strcmp(symid,"SEMI") == 0){
			getsym();
			printf("This a VarExplain statement!\n");
		}
	}else if(strcmp(symid,"SEMI") == 0){
		address++;
		insertSymTab(id,type,value,address,para," ");
		genMidCode("var",temp1,id,10);
		getsym();
		printf("This a VarExplain statement!\n");
	}
}

//int condiJudge(char conditionL,char *conditionOP,char conditionR){	
//	if(strcmp(conditionOP,"LESS") == 0){
//		return (conditionL < conditionR);
//	}else if(strcmp(conditionOP,"LESSOREQUAL") == 0){
//		return (conditionL <= conditionR);
//	}else if(strcmp(conditionOP,"MORE") == 0){
//		return (conditionL > conditionR);
//	}else if(strcmp(conditionOP,"MOREOREQUAL") == 0){
//		return (conditionL >= conditionR);
//	}else if(strcmp(conditionOP,"NOTEQUAL") == 0){
//		return (conditionL != conditionR);
//	}else if(strcmp(conditionOP,"ALEQUAL") == 0){
//		return (conditionL == conditionR);
//	}else {
//		///////error;
//	}
//}
//////////////////确定了tempStep
void Step(){ 
	tempStep = 0;
	if((strcmp(symid,"UNINT") == 0) && token[0] != '0'){
		tempStep = atoi(token);
		getsym();
	}
}
void Cycle(){
	char tempCycleLabel[256];
	char cycleID1[256];
	char cycleID2[256];
	char cycleID3[256];
	char cycleVar[256];
	if(strcmp(symid,"FORSYM") == 0){
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			getsym();
			if(strcmp(symid,"IDEN") == 0){
				strcpy(cycleID1,token);
				getsym();
				if(strcmp(symid,"EQUAL") == 0){
					getsym();
					Exp();
					genMidCode(cycleID1,"equal",nowTerm,29);
					if(strcmp(symid,"SEMI") == 0){
						
						strcpy(tempCycleLabel,genLabel());
						genMidCode(tempCycleLabel," "," ",43);

						getsym();
						Condition();/////////condition01确定

						if(strcmp(symid,"SEMI") == 0){
							getsym();
							if(strcmp(symid,"IDEN") == 0){
								strcpy(cycleID2,token);
								getsym();
								if(strcmp(symid,"EQUAL") == 0){
									getsym();
									if(strcmp(symid,"IDEN") == 0){
										strcpy(cycleID3,token);
										getsym();
										if(strcmp(symid,"PLUS") == 0){
											getsym();
											Step();/////////int tempStep已经确定
											char tempTempStep[256];
											strcpy(cycleVar,genVar());
											itoa(tempStep,tempTempStep,10);
											genMidCode(cycleVar,cycleID3,tempTempStep,16);
											genMidCode(cycleID2,"equal",cycleVar,29);
											
											if(strcmp(symid,"RBRA1") == 0){
												getsym();
												Sentence();
												
												genMidCode("BNZ",condition01,tempCycleLabel,27);
											}
										}else if(strcmp(symid,"MINUS") == 0){
											getsym();
											Step();/////////int tempStep已经确定
											char tempTempStep[256];
											strcpy(cycleVar,genVar());
											itoa(tempStep,tempTempStep,10);
											genMidCode(cycleVar,cycleID3,tempTempStep,17);
											genMidCode(cycleID2,"equal",cycleVar,29);

											if(strcmp(symid,"RBRA1") == 0){
												getsym();
												Sentence();
												
												genMidCode("BNZ",condition01,tempCycleLabel,27);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		printf("This is a cycle statement!\n");
	}
}				
//////////////确定了char condition01[256];记录条件成立与否的变量
void Condition(){
	char tempCondiExpNow[128];
	Exp();
	strcpy(tempCondiExpNow,nowTerm);
	if((strcmp(symid,"LESS") == 0) ||
		(strcmp(symid,"LESSOREQUAL") == 0) ||
		(strcmp(symid,"MORE") == 0) ||
		(strcmp(symid,"MOREOREQUAL") == 0) ||
		(strcmp(symid,"NOTEQUAL") == 0) ||
		(strcmp(symid,"ALEQUAL") == 0)
		){
		getsym();
		Exp();
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,nowTerm,20);
	}else if(strcmp(symid,"LESSOREQUAL") == 0){
		getsym();
		Exp();
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,nowTerm,21);
	}else if(strcmp(symid,"MORE") == 0){
		getsym();
		Exp();
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,nowTerm,22);
	}else if(strcmp(symid,"MOREOREQUAL") == 0){
		getsym();
		Exp();
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,nowTerm,23);
	}else if(strcmp(symid,"NOTEQUAL") == 0){
		getsym();
		Exp();
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,nowTerm,24);
	}else if(strcmp(symid,"ALEQUAL") == 0){
		getsym();
		Exp();
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,nowTerm,25);
	}
}
void CondiSen(){
	char condiSenL1[256];
	char condiSenL2[256];
	strcpy(condiSenL1,genLabel());//////////指向if后的语句
	strcpy(condiSenL2,genLabel());//////////指向else后的语句，若没有else就不用罢了

	if(strcmp(symid,"IFSYM") == 0){
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			getsym();
 			Condition();
			genMidCode("BZ",condition01,condiSenL1,28);
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
				if(strcmp(symid,"LBRA3") == 0){
					getsym();
					Sentence();
					genMidCode(condiSenL1," "," ",43);
					if(strcmp(symid,"RBRA3") == 0){
						getsym();
						if(strcmp(symid,"ELSESYM") == 0){
							getsym();
							if(strcmp(symid,"LBRA3") == 0){
								genMidCode("BZ",condition01,condiSenL2,28);
								getsym();
								Sentence();
								genMidCode(condiSenL2," "," ",43);
								if(strcmp(symid,"RBRA3") == 0){
									getsym();
								}
							}
						}
					}
				}
			}
		}
		printf("This is a condisen statement!\n");
	}
}

void Assign(){///////////////senID
	char assignArr[256];
	if(strcmp(symid,"EQUAL") == 0){
		getsym();
		Exp();
		genMidCode(senID,"equal",nowTerm,29);
		if(strcmp(symid,"SEMI") == 0){
		}
	}else if(strcmp(symid,"LBRA2") == 0){
		getsym();
		Exp();
		strcpy(assignArr,nowTerm);
		if(strcmp(symid,"LBRA2") == 0){
			getsym();
			if(strcmp(symid,"EQUAL") == 0){
				getsym();
				Exp();
				genMidCode(senID,assignArr,nowTerm,31);
				if(strcmp(symid,"SEMI") == 0){
				}
			}
		}
	}
	printf("This is an assign statement!\n");
}
//void ParaList(){
//	if(strcmp(symid,"RBRA1") != 0){
//		Exp();
//		while(strcmp(symid,"COMMA") == 0){
//			Exp();
//		}
//	}else{
//	
//	}
//}
void FuncCall1(){
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		while(strcmp(symid,"RBRA1") != 0){
			Exp();
			genMidCode("push",nowTerm," ",8);
			if(strcmp(symid,"COMMA") == 0){
				getsym();
			}
		}
		searchSymTab(tempID,0,0,-1);
		
		//genMidCode("GOTO",funcLabelGlo," ",26);////////产生无条件跳转指令
		if(strcmp(symid,"RBRA1") == 0){
			//strcpy();
			getsym();
		}
		genMidCode("call",tempID," ",7);
		strcpy(funcCallTemp,genVar());
		genMidCode(funcCallTemp,"equal","RET",44);
		strcpy(nowTerm,funcCallTemp);
	}
	printf("This is a funccall1 statement!\n");
}
void FuncCall0(){
		if(strcmp(symid,"LBRA1") == 0){
		getsym();
		while(strcmp(symid,"RBRA1") != 0){
			Exp();
			genMidCode("push",nowTerm," ",8);
			if(strcmp(symid,"COMMA") == 0){
				getsym();
			}
		}
		searchSymTab(senID,0,0,-1);
		
		//genMidCode("GOTO",funcLabelGlo," ",26);////////产生无条件跳转指令
		if(strcmp(symid,"RBRA1") == 0){
			//strcpy();
			getsym();
		}
		genMidCode("call",senID," ",7);
		strcpy(funcCallTemp,genVar());
		genMidCode(funcCallTemp,"equal","RET",44);
	}
	printf("This is a funccall0 statement!\n");
}

void Scanf(){
	char scanfID[256];
	getsym();
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		if(strcmp(symid,"IDEN") == 0){
			strcpy(scanfID,token);
			genMidCode("scanf",scanfID," ",39);
			getsym();
			while(strcmp(symid,"RBRA1") != 0){
				if(strcmp(symid,"COMMA") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						genMidCode("scanf",token," ",39);
						getsym();
					}
				}
			}
			getsym();
		}
	}
	printf("This is a read statement!\n");
}
void Printf(){
	getsym();
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		if(strcmp(symid,"STRING") == 0){
			genMidCode("printf",token," ",41);
			getsym();
			if(strcmp(symid,"COMMA") == 0){
				getsym();
				Exp();
				genMidCode("printf",nowTerm," ",41);
				if(strcmp(symid,"RBRA1") == 0){
					getsym();
				}
			}else if(strcmp(symid,"RBRA1") == 0){
				getsym();
			}
		}else{
			Exp();
			genMidCode("printf",nowTerm," ",41);
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
			}
		}
	}
	printf("This is a printf statement!\n");
}
void Return(){
	getsym();
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		Exp();
		genMidCode("ret",nowTerm," ",9);
		if(strcmp(symid,"RBRA1") == 0){
			getsym();
		}
	}else {
		genMidCode("ret"," "," ",41);
		getsym();
	}
	printf("This is a return statement!\n");
}

void Main(){
	getsym();
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		if(strcmp(symid,"RBRA1") == 0){
			getsym();
			if(strcmp(symid,"LBRA3") == 0){
				getsym();
				ComSen();
				if(strcmp(symid,"RBRA3") == 0){
					getsym();
				}
			}
		}
	}
	printf("This is a main statement!\n");
}
void FuncDef1(){////////////////无论何种函数都要返回一个funcDefRet(全局变量)void类型的返回一个'0'，别的return中的东西；在函数调用的时候把funcDefRet 赋给 nowTerm
	char tempFuncDef1[256];
	////////////////////////插入中间代码
	char tempFuncLabel[256];
	//////////////////////////将函数名填入符号表
	char func1ID[256];
	int func1Type = 2;
	int func1Value = (strcmp(symid,"INTSYM") == 0) ? 1:2;
	int paraNum = 0;
	/////////////////////////
	strcpy(func1ID,id);
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		while(strcmp(symid,"RBRA1") != 0){
			if(strcmp(symid,"INTSYM") == 0){
				getsym();
				if(strcmp(symid,"IDEN") == 0){
					paraNum++;
					strcpy(tempFuncDef1,token);
					getsym();
					if(strcmp(symid,"COMMA") == 0){
						
						genMidCode("para","int",tempFuncDef1,5);
						getsym();
					}else {
						
						genMidCode("para","int",tempFuncDef1,5);
						getsym();
						break;
					}
				}
			}else if(strcmp(symid,"CHARSYM") == 0){
				getsym();
				if(strcmp(symid,"IDEN") == 0){
					paraNum++;
					strcpy(tempFuncDef1,token);
					getsym();
					if(strcmp(symid,"COMMA") == 0){

						genMidCode("para","char",tempFuncDef1,6);
						getsym();
					}else {
						
						genMidCode("para","char",tempFuncDef1,6);
						getsym();
						break;
					}
				}
			}
		}
		
		

		if(strcmp(symid,"LBRA3") == 0){
			strcpy(tempFuncLabel,genLabel());
			genMidCode(tempFuncLabel," "," ",43);
			address++;
			insertSymTab(func1ID,func1Type,func1Value,address,paraNum,tempFuncLabel);


			getsym();
			ComSen();
			getsym();///////////}
			if(strcmp(temp1,"INTSYM") == 0){
				genMidCode("int",id,tempFuncLabel,1);
			}else if(strcmp(temp1,"CHARSYM") == 0){
				genMidCode("char",id,tempFuncLabel,2);
			}
		}
		printf("This is a funcdef1 statement with return!\n");
	}
}
void FuncDef0(){
	char tempFuncPara[256];
	///////////////////////
	char tempFuncDef0Label[256];
	/////////////////////////////
	int paraNum = 0;
	char func0ID[256];
	int func0Type = 2;
	int func0Value = 0;
	/////////////////////////////
	if(strcmp(symid,"IDEN") == 0){
		strcpy(func0ID,token);
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			getsym();
			while(strcmp(symid,"RBRA1") != 0){
				if(strcmp(symid,"INTSYM") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						paraNum++;
						strcpy(tempFuncPara,token);
						getsym();
						if(strcmp(symid,"COMMA") == 0){

							genMidCode("para","int",tempFuncPara,5);
							getsym();
						}else {

							genMidCode("para","int",tempFuncPara,5);
							getsym();
							break;
						}
					}
				}else if(strcmp(symid,"CHARSYM") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						paraNum++;
						strcpy(tempFuncPara,token);
						getsym();
						if(strcmp(symid,"COMMA") == 0){

							genMidCode("para","char",tempFuncPara,6);
							getsym();
						}else {
							
							genMidCode("para","char",tempFuncPara,6);
							getsym();
							break;
						}
					}
				}
			}
			
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
			}

			if(strcmp(symid,"LBRA3") == 0){
				strcpy(tempFuncDef0Label,genLabel());
				genMidCode(tempFuncDef0Label," "," ",43);
				address++;
				insertSymTab(func0ID,func0Type,func0Value,address,paraNum,tempFuncDef0Label);

				getsym();
				ComSen();
				getsym();
				genMidCode("void",id,tempFuncDef0Label,3);
			}
		}
		printf("This is a funcdef0 statement without return!\n");
	}
}

void Constant(){
	if(strcmp(symid,"PLUS") == 0){
		getsym();
		if(strcmp(symid,"UNINT") == 0){
			strcpy(constantNum,token);
			getsym();
		}
	}else if(strcmp(symid,"MINUS") == 0){
		getsym();
		constantNum[0] = '-';
		if(strcmp(symid,"UNINT") == 0){
			sprintf(constantNum,"%s",token);
			getsym();
		}
	}else if(strcmp(symid,"UNINT") == 0){
		strcpy(constantNum,token);
		getsym();
	}else if(token[0] == '0'){
		strcpy(constantNum,token);
		getsym();
	}else if(strcmp(symid,"ZIFU") == 0){
		strcpy(constantNum,token);
		getsym();
	}
}

void Switch(){
	strcpy(endLabel,genLabel());
	if(strcmp(symid,"SWITCHSYM") == 0){
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			getsym();
			Exp();
			strcpy(switchExp,nowTerm);
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
				if(strcmp(symid,"LBRA3") == 0){
					getsym();
					Case();
					Default();
					if(strcmp(symid,"RBRA3") == 0){
						getsym();
						
						genMidCode(endLabel," "," ",43);
					}
				}
			}
		}
		printf("This is a switch statement!\n");
	}
}
void Case(){
	char caseVar[256];
	char nextCaseLabel[256];
	strcpy(nextCaseLabel,genLabel());
	while(strcmp(symid,"CASESYM") == 0){
		genMidCode(nextCaseLabel," "," ",43);
		getsym();
		Constant();///////constantNum已经确定
		strcpy(caseVar,genVar());
		genMidCode(caseVar,switchExp,constantNum,25);
		strcpy(nextCaseLabel,genLabel());
		genMidCode("BZ",caseVar,nextCaseLabel,28);
		if(strcmp(symid,"COLON") == 0){
			getsym();
			Sentence();
			genMidCode("GOTO",endLabel," ",26);
			
		}
		printf("This is a case statement!\n");
	}
	strcpy(nextCaseLabel,endLabel);
}
void Default(){
	if(strcmp(symid,"DEFAULTSYM") == 0){
		getsym();
		if(strcmp(symid,"COLON") == 0){
			getsym();
			Sentence();
			genMidCode("GOTO",endLabel," ",26);
		}
		printf("This is a default statement!\n");
	}
}
void Sentence(){
	
	if(strcmp(symid,"IFSYM") == 0){
		CondiSen();
	}else if(strcmp(symid,"FORSYM") == 0){
		Cycle();
	}else if(strcmp(symid,"SCANFSYM") == 0){
		Scanf();
		if(strcmp(symid,"SEMI") == 0){
			getsym();
		}
	}else if(strcmp(symid,"PRINTFSYM") == 0){
		Printf();
		if(strcmp(symid,"SEMI") == 0){
			getsym();
		}
	}else if(strcmp(symid,"RETURNSYM") == 0){
		Return();
		if(strcmp(symid,"SEMI") == 0){
			getsym();
		}
	}else if(strcmp(symid,"SWITCHSYM") == 0){
		Switch();
	}else if(strcmp(symid,"IDEN") == 0){
		strcpy(senID,token);
		getsym();
		if(strcmp(symid,"EQUAL") == 0){
			Assign();
			if(strcmp(symid,"SEMI") == 0){
				getsym();
			}
		}else if(strcmp(symid,"LBRA1") == 0){
			FuncCall0();
			if(strcmp(symid,"SEMI") == 0){
				getsym();
			}
		}
	}else if(strcmp(symid,"LBRA3") == 0){
		getsym();
		SentenceList();
		getsym();
	}else if(strcmp(symid,"SEMI") == 0){
		getsym();
	}
}
void SentenceList(){
	while((strcmp(symid,"IFSYM") == 0) || strcmp(symid,"FORSYM") == 0 || (strcmp(symid,"LBRA3") == 0) ||
		(strcmp(symid,"INTSYM") == 0) || (strcmp(symid,"CHARSYM") == 0) || (strcmp(symid,"VOIDSYM") == 0) ||
		(strcmp(symid,"IDEN") == 0) || (strcmp(symid,"SCANFSYM") == 0) || (strcmp(symid,"PRINTFSYM") == 0) || 
		(strcmp(symid,"RETURNSYM") == 0) || (strcmp(symid,"SWITCHSYM") == 0) || (strcmp(symid,"SEMI") == 0)){
		Sentence();
	}
}
void ComSen(){
	if(strcmp(symid,"CONSTSYM") == 0){
		ConExplain();
	}else if((strcmp(symid,"INTSYM") == 0) || (strcmp(symid,"CHARSYM") == 0)){
		VarExplain();
	}
	SentenceList();
}

void main(){
	if((fp = fopen("test.txt","r")) ==NULL){
		printf("File cannot be opened!\n");
		return;
	}else{
		getsym();	
		while(strcmp(symid,"CONSTSYM") == 0){
			ConExplain();
		}
			while(1){
				if((strcmp(symid,"INTSYM") == 0) || (strcmp(symid,"CHARSYM") == 0)){
					strcpy(temp1,symid);
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						strcpy(id,token);
						getsym();
						if((strcmp(symid,"COMMA") == 0) || (strcmp(symid,"LBRA2") == 0) || (strcmp(symid,"SEMI") == 0)){
							type = 1;
							value = -1;
							VarExplain();
							continue;
						}else if(strcmp(symid,"LBRA1") == 0){								
							FuncDef1();
							continue;
						}
					}
				}else if(strcmp(symid,"VOIDSYM") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						strcpy(id,token);
						FuncDef0();
						continue;
					}else if(strcmp(symid,"MAINSYM") == 0){
						Main();
						if(strcmp(symid,"END") == 0){
							break;
						}else{
					
						}
					}
				}else if(strcmp(symid,"END") == 0){
					break;
				}
			}	
	}
	return ;

}
#endif