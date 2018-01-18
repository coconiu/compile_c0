#ifndef GRAMMER
#define GRAMMER
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "getsym.h"
#include "midcode.h"

#define idLength 1024
#define symtabLength 512
#define maxPre 512

typedef struct{
	char id[idLength];
	int type;		//0--const,1--var,2--func,3--para
	int value;		//������ֵ��typeΪ����ʱ��0��ʾvoid��1��ʾint��2��ʾchar,���Ǳ���-2��ʾint��-3��ʾchar����
	int address;	//��ʶ���洢��ַ���ַƫ��			
	int para;		//�����Ĳ������� �� ����ĳ���
	char funcLabelGlo[256]; ////�����ĵ�ַ����һ����䣩
}Symbol;

typedef struct{
	Symbol symtab[symtabLength];	//���ű�
	int index;						//���ű�ջ��ָ�룬�����ű��еķ��������� /////�ʼΪ0
	int blocksnum;					//��ǰ���ű�ӵ�еķֳ������� /////////////////�ʼΪ0
	int blocksindex[maxPre];		//�ֳ�������					////symTab.blocksindex[0] = 1;
}SymbolTab;

//////////////////////ȫ�ֱ�������
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
SymbolTab symTab;///////���ű�

int expType;
int MainR = 0;

char stepID1[256];
char stepID2[256];

char mainEndLabel[256];
char mainLabel[256];

int constantType = 0;
int funcDef0R = 0;
/////////////Ԥ��ȡ�ı�ʶ�����ͱ�ʶ��������
 char temp1[128],temp2[128];
 //////////char *symid1,*symid2;�μ�����ָ�룬��ָ��Ҫmalloc����Ȼ��Ұָ��
 ///////////�������������⴫�ݵ�ָ��
 FILE *fpSymTab = fopen("symTab.txt","wt");
 int isArr = 0;
 int funcOrIden;
 int subScript;
 int searchValue;
 int searchAdd;
 int searchType;////////scanf���ж���
 char searchLabel[256];
 int isFirstBlock;

 int arrNum = 0;
 int t;
 char nowTerm[128];
 int isConst;
 
 char tempFunc[128];
 int tempStep;
 char funcLabelGlo[256] = {0};
 
 
 char funcDefRet[256];
 char funcCallTemp[256];
 char senID[256];
 char constantNum[256];
 char switchExp[256];
 

 

 int searchParaNum = 0;
 int searchFuncRetType;
////////////����������
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
char *Condition();
void Assign();
void FuncCall1(char *tempID);
void FuncCall0();
void Scanf();
void Printf();
void Return();
void Main();
void FuncDef1();
void FuncDef0();
void Default(char *chenshuwei,char *mememe);
void Integer();
void Constant();
char *Case(char *chenshuwei,char *tempSwitchExp);
void Switch();
void ComSen();
char *Step();
void CondiSen();
///////////��������
void insertSymTab(char *id,int type,int value,int address,int para,char *funcLabelGlo){
	if(type == 2){////////////��������ֻ����Ҹ����ֳ������ڼ�������ڵ������Ƿ��ظ�����
		for(i=1;i<symTab.blocksnum;i++){///////////����Ҫ��0��ʼ����Ϊ��һ�㻹û���κκ��� ����������
			if(strcmp(id,symTab.symtab[symTab.blocksindex[i]].id) == 0){
				//////error
				////return;
			}
		}
		symTab.index++;
		/*symTab.blocksnum++;
		symTab.blocksindex[symTab.blocksnum] = symTab.index;*/
	}else {////////////////������������Ҹò���ű����Ƿ����ظ�
		i = symTab.blocksindex[symTab.blocksnum];
		for(;i<symTab.index;i++){
			if(strcmp(id,symTab.symtab[i].id) == 0){
				////////error
				/////return;
			}
		}
		symTab.index++;
	}
	////////////���޷��ű����� ��ʼ����ű�
	
	strcpy(symTab.symtab[symTab.index].id,id);
	symTab.symtab[symTab.index].type = type;
	symTab.symtab[symTab.index].value = value;
	symTab.symtab[symTab.index].address = address;
	symTab.symtab[symTab.index].para = para;
	strcpy(symTab.symtab[symTab.index].funcLabelGlo,funcLabelGlo);
	fprintf(fpSymTab,"%s,",id);
	fprintf(fpSymTab,"%d,",type);
	fprintf(fpSymTab,"%d,",value);
	fprintf(fpSymTab,"%d,",address);
	fprintf(fpSymTab,"%d,",para);
	fprintf(fpSymTab,"%d;",index);
	fprintf(fpSymTab,"%s\n",funcLabelGlo);
}
void init(){
	symTab.blocksnum = 0;
	symTab.index = 0;
	symTab.blocksindex[0] = 0;
}
void searchSymTab(char *id,int funcOrIden,int isArr,int subScript){//////////subScript �������±꣬���������飬����Ϊ-1
	////////////////////////////////�����int��char ����int�����ͷ��ػ�ȥ���������飬�ͽ�������±귵������ĳһ���ֵ,�������Ƿ����ڵ�һ��
	////////////////////////////////�����������
	if(funcOrIden == 0){////func
		for(i=0;i<=symTab.index;i++){
			int ppp = symTab.blocksindex[i];
			if((strcmp(id,symTab.symtab[i].id) == 0) &&(symTab.symtab[i].type == 2) ){
				strcpy(funcLabelGlo,symTab.symtab[i].funcLabelGlo);
				strcpy(searchLabel,funcLabelGlo);
				searchAdd = symTab.symtab[i].address;
				searchParaNum = symTab.symtab[i].para;
				searchFuncRetType = symTab.symtab[i].value;
				searchValue = -1;
				isFirstBlock = 0;
				break;
			}
		}
		if(i > symTab.index){
			//error(9,errorRow,errorCol);		///////δ�������ĺ�����
			//while(strcmp(symid,"RBRA3") != 0){
			//	getsym();
			//}
		}
		//if(symTab.symtab[symTab.blocksindex[i]].para != para){
		//	error(10,errorRow,errorCol);		//////�������ú��������ǲ���������һ��
		//	while(strcmp(symid,"RBRA3") != 0){
		//		getsym();
		//	}
		//}
		
	}else {////////////////////char or int//const or var
		i = symTab.blocksindex[symTab.blocksnum];
		for(;i<=symTab.index;i++){//////////////�ڵ�ǰ���ű��ѯ
			if(strcmp(id,symTab.symtab[i].id) == 0){
				isConst = (symTab.symtab[i].type == 0);
				searchType = ((symTab.symtab[i].type == 1) && (symTab.symtab[i].value == -2)) || ((symTab.symtab[i].type == 0) && (symTab.symtab[i].para == -3));
				searchAdd = symTab.symtab[i].address;
				searchValue = symTab.symtab[i].value;
				isFirstBlock = (symTab.blocksnum == 0);
				break;
			}
		}
		if(i>symTab.index){/////////////////��ȫ�ַ��ű��ѯ
			for(i=0;i<=symTab.blocksindex[1];i++){
				if(strcmp(symTab.symtab[i].id,id) == 0){
					isConst = (symTab.symtab[i].type == 0);
					searchType = ((symTab.symtab[i].type == 1) && (symTab.symtab[i].value == -2)) || ((symTab.symtab[i].type == 0) && (symTab.symtab[i].para == -3));
					searchAdd = symTab.symtab[i].address;
					searchValue = symTab.symtab[i].value;
					isFirstBlock = 1;
					break;
				}
			}
			if(i>symTab.blocksindex[1]){
				error(11,errorRow,errorCol);
				while(strcmp(symid,"SEMI") != 0){
					getsym();
				}
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
void Factor(){/////////////����
	char place3[256] = {0};
	char place4[256] = {0};
	char place5[256] = {0};
	char place6[256] = {0};
	char FactorID[256];
	char tempID[128];
	if(strcmp(symid,"IDEN") == 0){
		strcpy(tempID,token);
		strcpy(FactorID,token);
		getsym();
		if(strcmp(symid,"LBRA2") == 0){////////����
			char tempExpTerm[256];
			funcOrIden = 1;
			isArr = 1;
			getsym();
			Exp();/////////
			strcpy(tempExpTerm,nowTerm);
			///////////////nowTerm���Ѿ��洢����Ӧ����
			searchSymTab(FactorID,funcOrIden,isArr,atoi(tempExpTerm));

			if(strcmp(symid,"RBRA2") == 0){
				strcpy(nowTerm,genVar());
				genMidCode(nowTerm,FactorID,tempExpTerm,30);
				
				getsym();
			}else{
				error(8,errorRow,errorCol);
				getsym();
				while((strcmp(symid,"IFSYM") != 0) && (strcmp(symid,"SCANFSYM") != 0) && (strcmp(symid,"FORSYM") != 0) && 
					(strcmp(symid,"IFSYM") != 0) && (strcmp(symid,"LBRA3") != 0) && (strcmp(symid,"PRINTFSYM") != 0) &&
					(strcmp(symid,"IDEN") != 0) && (strcmp(symid,"SEMI") != 0) && (strcmp(symid,"SWITCHSYM") != 0) && 
					(strcmp(symid,"RETURNSYM") != 0)
					){
					getsym();
				}
			}
			if(searchType == 1){
				expType = 1;	
			}else{
				expType = 0;
			}
			
			
		}else if(strcmp(symid,"LBRA1") == 0){////////��������
			searchSymTab(tempID,0,0,-1);
			funcOrIden = 0;
			FuncCall1(tempID);///////������������ɲ�����call���м����,�Ҵ�ʱ��nowTerm �Ѿ���return��ֵ��
			

			strcpy(place3,genVar());
			genMidCode(place3,"equal",nowTerm,29);
			strcpy(nowTerm,place3);///////////��ʱ�Ѿ�������м���ʱ�����ĸ�ֵ����ʱ������ΪnowTerm
			
			
			if(searchFuncRetType == 1){
				expType = 0;
			}else if(searchFuncRetType == 2){
				expType = 1;
			}
		}else{//////IDEN
			searchSymTab(tempID,1,0,-1);
			
			//if(isConst){
			//	
			//	genMidCode(genVar(),"equal",tempID,29);
			//	/*itoa(searchValue,tempCon,10);
			//	strcpy(nowTerm,tempCon);
			//	isConst = 1;*/
			//}else{
			//	strcpy(nowTerm,tempID);
			//}
			strcpy(nowTerm,tempID);
			//genMidCode(genVar(),"equal",tempID,29);
			if(searchType == 1){
				expType = 1;
			}else{
				expType = 0;
			}
		}
		
	}else if(strcmp(symid,"ZIFU") == 0){
		expType = 1;
		sprintf(nowTerm,"%d",token[0]);
		strcpy(place5,genVar());
		genMidCode(place5,"equal",nowTerm,40);
		strcpy(nowTerm,place5);
		getsym();
	}else if(strcmp(symid,"LBRA1") == 0){
		//////////////////��exp��
		getsym();
		Exp();////////////nowTerm�Ѿ�����	expTypeҲ�Ѿ�ȷ��
		strcpy(place6,genVar());
		genMidCode(place6,"equal",nowTerm,29);
		strcpy(nowTerm,place6);
		if(strcmp(symid,"RBRA1") == 0){
			getsym();
		}
	}else if((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0) || token[0] == '0' || strcmp(symid,"UNINT") == 0){
		expType = 0;
		Integer();////////////nowTerm����
		strcpy(place4,genVar());
		genMidCode(place4,"equal",nowTerm,40);
		strcpy(nowTerm,place4);
	}
}
void Term(){//////��
	char ttt[256] = {0};
	char mmm[256] = {0};
	char ppp[256] = {0};
	Factor();
	
	
	while(1){
		strcpy(ttt,nowTerm);
		if(strcmp(symid,"MUL") == 0){
			expType = 0;
			getsym();
			Factor();
			
			strcpy(mmm,nowTerm);
			strcpy(ppp,genVar());
			genMidCode(ppp,ttt,mmm,18);
			strcpy(nowTerm,ppp);
		}else if(strcmp(symid,"DIV") == 0){
			expType = 0;
			getsym();
			Factor();
			
			strcpy(mmm,nowTerm);
			strcpy(ppp,genVar());
			genMidCode(ppp,ttt,mmm,19);
			strcpy(nowTerm,ppp);
			
		}else{
			break;
		}
	}
}
void Exp(){//////���ʽ
	////////////expType 
	////////////0:int
	////////////1:char
	char expTemp[256];
	char zzz[256];
	char place1[256];
	char place2[256];
	if(strcmp(symid,"MINUS") == 0){
		expType = 0;
		getsym();
		Term();
		
		strcpy(place1,genVar());
		genMidCode(place1,"equal","0",40);
		strcpy(place2,genVar());
		genMidCode(place2,place1,nowTerm,17);
		strcpy(nowTerm,place2);
		
		while((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0)){
			strcpy(expTemp,nowTerm);
			if(strcmp(symid,"PLUS") == 0){
				getsym();
				Term();
				strcpy(zzz,genVar());
				genMidCode(zzz,expTemp,nowTerm,16);
				strcpy(nowTerm,zzz);
			}else if(strcmp(symid,"MINUS") == 0){
				getsym();
				Term();
				strcpy(zzz,genVar());
				genMidCode(zzz,expTemp,nowTerm,17);
				strcpy(nowTerm,zzz);
			}
		}
	}else if(strcmp(symid,"PLUS") == 0){
		expType = 0;
		getsym();
		Term();
		
		
		while((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0)){
			strcpy(expTemp,nowTerm);
			if(strcmp(symid,"PLUS") == 0){
				getsym();
				Term();
				strcpy(zzz,genVar());
				genMidCode(zzz,expTemp,nowTerm,16);
				strcpy(nowTerm,zzz);
			}else if(strcmp(symid,"MINUS") == 0){
				getsym();
				Term();
				strcpy(zzz,genVar());
				genMidCode(zzz,expTemp,nowTerm,17);
				strcpy(nowTerm,zzz);
			}
		}
	}else {
		Term();
		
		
		while((strcmp(symid,"PLUS") == 0) || (strcmp(symid,"MINUS") == 0)){
			strcpy(expTemp,nowTerm);
			if(strcmp(symid,"PLUS") == 0){
				expType = 0;
				getsym();
				Term();
				strcpy(zzz,genVar());
				genMidCode(zzz,expTemp,nowTerm,16);
				strcpy(nowTerm,zzz);
			}else if(strcmp(symid,"MINUS") == 0){
				expType = 0;
				getsym();
				Term();
				strcpy(zzz,genVar());
				genMidCode(zzz,expTemp,nowTerm,17);
				strcpy(nowTerm,zzz);
			}
		}		
	}
}

void ConDef(){
	char tempCon[128];
	type = 0;
	getsym();
	if(strcmp(symid,"INTSYM") == 0){
		getsym();
		while(1){
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
								para = -2;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
								
								genMidCode("int",id,tempCon,14);
								getsym();
							}else if(strcmp(symid,"SEMI") == 0){
								para = -2;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);
								
								genMidCode("int",id,tempCon,14);
								
								break;
							}else{
								error(11,errorRow,errorCol);
								while(strcmp(symid,"SEMI") != 0){
									getsym();
								}
								break;
							}
						}
					}else if(strcmp(symid,"MINUS") == 0){
						getsym();
						if(strcmp(symid,"UNINT") == 0){
							value = 0-atoi(token);
							sprintf(tempCon,"%d",value);
							getsym();
							if(strcmp(symid,"COMMA") == 0){
								para = -2;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

								genMidCode("int",id,tempCon,14);

								getsym();
							}else if(strcmp(symid,"SEMI") == 0){
								para = -2;
								address++;
								
								insertSymTab(id,type,value,address,para," ");
								printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

								genMidCode("int",id,tempCon,14);
								break;
							}else{
								error(11,errorRow,errorCol);
								while(strcmp(symid,"SEMI") != 0){
									getsym();
								}
								break;
							}
						}
					}else if(strcmp(symid,"UNINT") == 0){
						value = atoi(token);
						sprintf(tempCon,"%d",value);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							para = -2;
							address++;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("int",id,tempCon,14);
							getsym();
						}else if(strcmp(symid,"SEMI") == 0){
							para = -2;
							address++;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("int",id,tempCon,14);
							break;
						}else{
							error(11,errorRow,errorCol);
							while(strcmp(symid,"SEMI") != 0){
								getsym();
							}
							break;
						}
					}else if(strcmp(symid,"ZIFU") == 0){
						sprintf(tempCon,"%d",token[0]);
						value = atoi(token);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							para = -2;
							address++;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("int",id,tempCon,14);
							getsym();
						}else if(strcmp(symid,"SEMI") == 0){
							para = -2;
							address++;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("int",id,tempCon,14);
							break;
						}else{
							error(11,errorRow,errorCol);
							while(strcmp(symid,"SEMI") != 0){
								getsym();
							}
							break;
						}
					}
				}
			}else{
				error(5,errorRow,errorCol);
				while(strcmp(symid,"SEMI") != 0){
					getsym();
				}
				break;
			}
		}
	}else if(strcmp(symid,"CHARSYM") == 0){
		getsym();
		while(1){
			if(strcmp(symid,"IDEN") == 0){
				strcpy(id,token);
				getsym();
				if(strcmp(symid,"EQUAL") == 0){
					getsym();
					if(strcmp(symid,"ZIFU") == 0){
						value = token[0];
						sprintf(tempCon,"%d",token[0]);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							address++;
							para = -3;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("char",id,tempCon,15);
							getsym();
						}else if(strcmp(symid,"SEMI") == 0){
							address++;
							para = -3;
							
							insertSymTab(id,type,value,address,para," ");
							printf("%s,%d,%d,%d,%d\n",id,type,value,address,para);

							genMidCode("char",id,tempCon,15);
							
							break;
						}else{
							error(11,errorRow,errorCol);
							while(strcmp(symid,"SEMI") != 0){
								getsym();
							}
							break;
						}
					}
				}
			}
		}
	}else{
		error(4,errorRow,errorCol);
		while(strcmp(symid,"SEMI") != 0){
			getsym();
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
	///////////type = 1 �������������Ѿ�ȷ��
	type = 1;
	char tempVarDef[256];
	if(strcmp(symid,"COMMA") == 0){
		address++;
		para = -1;
		if(strcmp(temp1,"INTSYM") == 0){
			value = -1;
		}else if(strcmp(temp1,"CHARSYM") == 0){
			value = -2;
		}
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
								/*value = -1;*/
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
								
							}else if(strcmp(symid,"SEMI") == 0){
								/*value = -1;*/
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
							}else{
								error(11,errorRow,errorCol);
								while(strcmp(symid,"SEMI") != 0){
									getsym();
								}
								break;
							}
						}
					}
				}else if(strcmp(symid,"COMMA") == 0){
					if(strcmp(temp1,"INTSYM") == 0){
						value = -1;
					}else if(strcmp(temp1,"CHARSYM") == 0){
						value = -2;
					}
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
					if(strcmp(temp1,"INTSYM") == 0){
						value = -1;
					}else if(strcmp(temp1,"CHARSYM") == 0){
						value = -2;
					}
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
					error(11,errorRow,errorCol);
					while(strcmp(symid,"SEMI") != 0){
						getsym();
					}
					break;
				}
			}
		}
	}else if(strcmp(symid,"LBRA2") == 0){
		type = 1;
		getsym();
		if(strcmp(temp1,"INTSYM") == 0){
			value = -1;
		}else if(strcmp(temp1,"CHARSYM") == 0){
			value = -2;
		}
		if(strcmp(symid,"UNINT") == 0){
			strcpy(tempVarDef,token);
			para = atoi(token);
			getsym();
			if(strcmp(symid,"RBRA2") == 0){
				getsym();
				if(strcmp(symid,"COMMA") == 0){
					
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
										}else{
											error(11,errorRow,errorCol);
											while(strcmp(symid,"SEMI") != 0){
												getsym();
											}
											break;
										}
									}else{
										error(8,errorRow,errorCol);
										while(strcmp(symid,"SEMI") != 0){
											getsym();
										}
									}
								}else{
									error(12,errorRow,errorCol);
									while(strcmp(symid,"SEMI") != 0){
										getsym();
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
							}else{
								error(11,errorRow,errorCol);
								while(strcmp(symid,"SEMI") != 0){
									getsym();
								}
								break;
							}							
						}else{
							error(13,errorRow,errorCol);
							while(strcmp(symid,"SEMI") != 0){
								getsym();
							}
							break;
						}
					}
				}else if(strcmp(symid,"SEMI") == 0){
					
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
				}else{
					error(11,errorRow,errorCol);
					while(strcmp(symid,"SEMI") != 0){
						getsym();
					}
				}
			}else{
				error(8,errorRow,errorCol);
				while(strcmp(symid,"SEMI") != 0){
					getsym();
				}
			}
		}else{
			error(12,errorRow,errorCol);
			while(strcmp(symid,"SEMI") != 0){
				getsym();
			}
		}
	}else if(strcmp(symid,"SEMI") == 0){
		if(strcmp(temp1,"INTSYM") == 0){
			value = -1;
		}else if(strcmp(temp1,"CHARSYM") == 0){
			value = -2;
		}
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
	///////////type = 1 �������������Ѿ�ȷ��
	type = 1;
	if(strcmp(symid,"COMMA") == 0 || strcmp(symid,"LBRA2") == 0){
		VarDef();
		if(strcmp(symid,"SEMI") == 0){
			getsym();
			printf("This a VarExplain statement!\n");
		}
	}else if(strcmp(symid,"SEMI") == 0){////type = 1�Ѿ�ȷ��
		address++;
		para = -1;
		if(strcmp(temp1,"INTSYM") == 0){
			value = -1;
		}else if(strcmp(temp1,"CHARSYM") == 0){
			value = -2;
		}
		insertSymTab(id,type,value,address,para," ");
		if(strcmp(temp1,"INTSYM") == 0){
			genMidCode("var","int",id,10);
		}else if(strcmp(temp1,"CHARSYM") == 0){
			genMidCode("var","char",id,10);
		}
		
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
//////////////////ȷ����tempStep
char *Step(){ 
	char step1[256];
	tempStep = 0;
	
	if((strcmp(symid,"UNINT") == 0) && token[0] != '0'){
		sprintf(step1,"%s",token);
		tempStep = atoi(token);
		getsym();
	}else{
		error(14,errorRow,errorCol);
		exit(0);
	}
	return step1;
}
//int ConditionN(char *conditionL,char *conditionR,int conditionOpValue){
//	
//	char place1[256];
//	Exp();
//	strcpy(conditionL,nowTerm);
//	if(strcmp(symid,"LESS") == 0){
//		getsym();
//		Exp();
//		conditionOpValue = 20;
//		strcpy(conditionR,nowTerm);
//		//strcpy(condition01,genVar());
//		//genMidCode(condition01,tempCondiExpNow,nowTerm,20);
//	}else if(strcmp(symid,"LESSOREQUAL") == 0){
//		getsym();
//		Exp();
//		conditionOpValue = 21;
//		strcpy(conditionR,nowTerm);
//		//strcpy(condition01,genVar());
//		//genMidCode(condition01,tempCondiExpNow,nowTerm,21);
//	}else if(strcmp(symid,"MORE") == 0){
//		getsym();
//		Exp();
//		conditionOpValue = 22;
//		strcpy(conditionR,nowTerm);
//		//strcpy(condition01,genVar());
//		//genMidCode(condition01,tempCondiExpNow,nowTerm,22);
//	}else if(strcmp(symid,"MOREOREQUAL") == 0){
//		getsym();
//		Exp();
//		conditionOpValue = 23;
//		strcpy(conditionR,nowTerm);
//		//strcpy(condition01,genVar());
//		//genMidCode(condition01,tempCondiExpNow,nowTerm,23);
//	}else if(strcmp(symid,"NOTEQUAL") == 0){
//		getsym();
//		Exp();
//		conditionOpValue = 24;
//		strcpy(conditionR,nowTerm);
//		//strcpy(condition01,genVar());
//		//genMidCode(condition01,tempCondiExpNow,nowTerm,24);
//	}else if(strcmp(symid,"ALEQUAL") == 0){
//		getsym();
//		Exp();
//		conditionOpValue = 25;
//		strcpy(conditionR,nowTerm);
//		//strcpy(condition01,genVar());
//		//genMidCode(condition01,tempCondiExpNow,nowTerm,25);
//	}else{
//		strcpy(place1,genVar());
//		genMidCode(place1,"equal","0",40);
//		conditionOpValue = 24;
//		strcpy(conditionR,place1);
//	}
//	return conditionOpValue;
//}
char *Condition(){
	char tempCondiExpNow[128];
	char place1[256];
	char place2[256];
	char condition01[256];
	Exp();
	strcpy(tempCondiExpNow,nowTerm);
	if(strcmp(symid,"LESS") == 0){
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
	}else{
		strcpy(place1,genVar());
		genMidCode(place1,"equal","0",40);
		strcpy(condition01,genVar());
		genMidCode(condition01,tempCondiExpNow,place1,24);
	}
	return condition01;
}
void Cycle(){
	int tempStep;

	char beginCycleLabel[256];

	char beforeCondi[256];
	char endCycleLabel[256];
	
	char cycleRet[256];

	char cycleID1[256];
	char cycleID2[256];
	char cycleID3[256];
	char cycleVar[256];
	
	char place[256];
	char place1[256];
	char place2[256];
	char step1[256];
	char step2[256];

	/*char conditionL[256];
	char conditionR[256];
	int conditionOpValue = 0;*/
	strcpy(beginCycleLabel,genLabel());
	strcpy(beforeCondi,genLabel());
	strcpy(endCycleLabel,genLabel());
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
						genMidCode("GOTO",beginCycleLabel," ",26);

						genMidCode(beforeCondi," "," ",43);
						getsym();
						strcpy(cycleRet,Condition());
						genMidCode("BZ",cycleRet,endCycleLabel,28);
						//conditionOpValue = ConditionN(conditionL,conditionR,conditionOpValue);/////////conditionL,conditionR,conditionOpValueȷ��
						
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
											strcpy(step1,Step());/////////int tempStep�Ѿ�ȷ����char *step1
											
											if(strcmp(symid,"RBRA1") == 0){
												
												genMidCode(beginCycleLabel," "," ",43);
												getsym();
												Sentence();
												

												strcpy(place,genVar());
												genMidCode(place,"equal",step1,40);
												strcpy(place1,genVar());
												genMidCode(place1,cycleID3,place,16);
												genMidCode(cycleID2,"equal",place1,29);
												/*strcpy(place2,genVar());
												genMidCode(place2,conditionL,conditionR,conditionOpValue);

												genMidCode("BNZ",place2,tempCycleLabel,27);*/
												genMidCode("GOTO",beforeCondi," ",26);
												
												genMidCode(endCycleLabel," "," ",43);
											}else{
												error(21,errorRow,errorCol);
												exit(0);
											}
										}else if(strcmp(symid,"MINUS") == 0){
											getsym();
											strcpy(step1,Step());/////////int tempStep�Ѿ�ȷ����char *step1
											
											if(strcmp(symid,"RBRA1") == 0){
												
												genMidCode(beginCycleLabel," "," ",43);
												getsym();
												Sentence();
												
												

												strcpy(place,genVar());
												genMidCode(place,"equal",step1,40);
												strcpy(place1,genVar());
												genMidCode(place1,cycleID3,place,17);
												genMidCode(cycleID2,"equal",place1,29);
												/*strcpy(place2,genVar());
												genMidCode(place2,conditionL,conditionR,conditionOpValue);

												genMidCode("BNZ",place2,tempCycleLabel,27);*/
												genMidCode("GOTO",beforeCondi," ",26);
												
												genMidCode(endCycleLabel," "," ",43);
											}else{
												error(21,errorRow,errorCol);
												exit(0);
											}
										}else{
											error(20,errorRow,errorCol);
											exit(0);
										}
									}else{
										error(18,errorRow,errorCol);
										exit(0);
									}
								}else{
									error(19,errorRow,errorCol);
									exit(0);
								}
							}else{
								error(18,errorRow,errorCol);
								exit(0);
							}
						}else{
							error(11,errorRow,errorCol);
							exit(0);
						}
					}else{
						error(11,errorRow,errorCol);
						exit(0);
					}
				}else{
					error(17,errorRow,errorCol);
					exit(0);
				}
			}else{
				error(16,errorRow,errorCol);
				exit(0);
			}
		}else{
			error(15,errorRow,errorCol);
			exit(0);
		}
		printf("This is a cycle statement!\n");
	}
}	

//////////////ȷ����char condition01[256];��¼�����������ı���

void CondiSen(){
	char condiSenL1[256];
	char condiSenL2[256];
	char condiRet[256];
	strcpy(condiSenL1,genLabel());//////////ָ��if������
	strcpy(condiSenL2,genLabel());//////////ָ��else�����䣬��û��else�Ͳ��ð���

	if(strcmp(symid,"IFSYM") == 0){
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			getsym();
 			strcpy(condiRet,Condition());
			genMidCode("BZ",condiRet,condiSenL1,28);
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
					Sentence();
					genMidCode("GOTO",condiSenL2," ",26);
					genMidCode(condiSenL1," "," ",43);
					
						
						if(strcmp(symid,"ELSESYM") == 0){
							getsym();
								/*genMidCode("BZ",condition01,condiSenL2,28);*/
								Sentence();
								genMidCode(condiSenL2," "," ",43);
								
							
						}else{
							genMidCode(condiSenL2," "," ",43);
						}
						
					
				
			}else{
				error(21,errorRow,errorCol);
				exit(0);
			}
		}else{
			error(15,errorRow,errorCol);
			exit(0);
		}
		printf("This is a condisen statement!\n");
	}
}

void Assign(){///////////////senID
	char assignArr[256];
	char place[256];
	if(strcmp(symid,"EQUAL") == 0){
		getsym();
		Exp();
		genMidCode(senID,"equal",nowTerm,29);
		
		if(strcmp(symid,"SEMI") == 0){
			getsym();
		}
	}else if(strcmp(symid,"LBRA2") == 0){
		getsym();
		Exp();
		strcpy(assignArr,nowTerm);
		if(strcmp(symid,"RBRA2") == 0){
			getsym();
			if(strcmp(symid,"EQUAL") == 0){
				getsym();
				Exp();
				genMidCode(senID,assignArr,nowTerm,31);
				if(strcmp(symid,"SEMI") == 0){
					getsym();
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
void FuncCall1(char *tempID){
	char funcName[256];
	int funcCall1Num = 0;
	if(strcmp(symid,"LBRA1") == 0){
		searchSymTab(tempID,0,0,-1);
		strcpy(funcName,tempID);
		getsym();
		while(strcmp(symid,"RBRA1") != 0){
			Exp();
			genMidCode("push",nowTerm," ",8);
			funcCall1Num++;
			if(strcmp(symid,"COMMA") == 0){
				getsym();
			}
		}
		
		/*if(searchParaNum != funcCall1Num){
			error(22,errorRow,errorCol);
			while(strcmp(symid,"SEMI")!=0){
				getsym();
			}
		}*/
		//genMidCode("GOTO",funcLabelGlo," ",26);////////������������תָ��
		if(strcmp(symid,"RBRA1") == 0){
			//strcpy();
			getsym();
		}else{
			error(21,errorRow,errorCol);
			while(strcmp(symid,"SEMI")!=0){
				getsym();
			}
		}
		searchSymTab(tempID,0,0,-1);
		genMidCode("call",funcName,searchLabel,7);
		strcpy(funcCallTemp,genVar());
		genMidCode(funcCallTemp,"equal","RET",44);
		strcpy(nowTerm,funcCallTemp);
	}
	printf("This is a funccall1 statement!\n");
}
void FuncCall0(){
	int funcCall0Num = 0;
		if(strcmp(symid,"LBRA1") == 0){
			searchSymTab(senID,0,0,-1);
			
			getsym();
			while(strcmp(symid,"RBRA1") != 0){
				Exp();
				genMidCode("push",nowTerm," ",8);
				funcCall0Num++;
				if(strcmp(symid,"COMMA") == 0){
					getsym();
				}
			}
		/*	if(funcCall0Num != searchParaNum){
				error(22,errorRow,errorCol);
				while(strcmp(symid,"SEMI")!=0){
					getsym();
				}
			}*/
		
			//genMidCode("GOTO",funcLabelGlo," ",26);////////������������תָ��
			if(strcmp(symid,"RBRA1") == 0){
				//strcpy();
				getsym();
				if(strcmp(symid,"SEMI") == 0){
					getsym();
				}else{
					error(11,errorRow,errorCol);
					while(strcmp(symid,"SEMI") != 0){
						getsym();
					}
				}
			}else{
				error(21,errorRow,errorCol);
				while(strcmp(symid,"SEMI")!=0){
					getsym();
				}
			}
			genMidCode("call",senID,searchLabel,47);
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
			searchSymTab(scanfID,1,0,-1);
			if(searchType == 0){
				genMidCode("scanf",scanfID,"int",39);
			}else if(searchType == 1){
				genMidCode("scanf",scanfID,"char",46);
			}
			
			getsym();
			while(strcmp(symid,"RBRA1") != 0){
				if(strcmp(symid,"COMMA") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						strcpy(scanfID,token);
						searchSymTab(scanfID,1,0,-1);
						if(searchType == 0){
							genMidCode("scanf",scanfID,"int",39);
						}else{
							genMidCode("scanf",scanfID,"char",46);
						}
						getsym();
					}
				}
			}
			getsym();
			if(strcmp(symid,"SEMI") == 0){
				getsym();
			}
		}
	}else{
		error(15,errorRow,errorCol);
		while(strcmp(symid,"SEMI") != 0){
			getsym();
		}
	}
	printf("This is a read statement!\n");
}
void Printf(){
	char kkk[256];
	char lll[256];
	getsym();
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		if(strcmp(symid,"STRING") == 0){
			strcpy(lll,genVar());
			genMidCode(lll,"equal",token,48);
			
			genMidCode("printf",lll,"string",42);
			getsym();
			if(strcmp(symid,"COMMA") == 0){
				getsym();
				Exp();
				strcpy(kkk,genVar());
				genMidCode(kkk,"equal",nowTerm,29);
				if(expType == 0){
					genMidCode("printf",kkk,"int",41);
				}else if(expType == 1){
					genMidCode("printf",kkk,"char",45);
				}
				if(strcmp(symid,"RBRA1") == 0){
					getsym();
					if(strcmp(symid,"SEMI") == 0){
						getsym();
					}
				}else{
					error(23,errorRow,errorCol);
					while(strcmp(symid,"SEMI") != 0){
						getsym();
					}
				}
			}else if(strcmp(symid,"RBRA1") == 0){
				getsym();
				if(strcmp(symid,"SEMI") == 0){
					getsym();
				}
			}else{
				error(23,errorRow,errorCol);
				while(strcmp(symid,"SEMI") != 0){
					getsym();
				}
			}
		}else{
			Exp();
			strcpy(kkk,genVar());
			genMidCode(kkk,"equal",nowTerm,29);
			
			if(expType == 0){
				genMidCode("printf",kkk,"int",41);
			}else if(expType == 1){
				genMidCode("printf",kkk,"char",45);
			}
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
				if(strcmp(symid,"SEMI") == 0){
					getsym();
				}
			}
		}
	}else{
		error(15,errorRow,errorCol);
		while(strcmp(symid,"SEMI") != 0){
			getsym();
		}
	}
	
	printf("This is a printf statement!\n");
}
void Return(){
	getsym();
	if(strcmp(symid,"LBRA1") == 0){
		getsym();
		Exp();
		genMidCode("RET",nowTerm," ",9);
		if(strcmp(symid,"RBRA1") == 0){
			getsym();
			if(strcmp(symid,"SEMI") == 0){
				getsym();
			}
		}
	}else if(strcmp(symid,"SEMI") == 0){
		genMidCode("RET"," "," ",9);
		if(strcmp(symid,"SEMI") == 0){
			getsym();	
		}
		
	}else{
		error(11,errorRow,errorCol);
		while(strcmp(symid,"SEMI") != 0){
			getsym();
		}
	}
	printf("This is a return statement!\n");
}

void Main(){
	
	strcpy(mainLabel,genLabel());
	genMidCode(mainLabel," "," ",43);
	genMidCode("void","main",mainLabel,4);
	
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
		}else{
			error(21,errorRow,errorCol);
			exit(0);
		}
	}else{
		error(15,errorRow,errorCol);
		exit(0);
	}
	address++;
	insertSymTab("main",2,0,address,0," ");
	symTab.blocksindex[symTab.blocksnum] = index;
	
	genMidCode("RET"," "," ",9);
	
	strcpy(mainEndLabel,genLabel());
	genMidCode(mainEndLabel," "," ",43);
	printf("This is a main statement!\n");
}
void FuncDef1(){////////////////���ۺ��ֺ�����Ҫ����һ��funcDefRet(ȫ�ֱ���)void���͵ķ���һ��'0'�����return�еĶ������ں������õ�ʱ���funcDefRet ���� nowTerm
	char tempFuncDef1[256];
	////////////////////////�����м����
	char tempFuncLabel[256];
	//////////////////////////��������������ű�
	char func1ID[256];
	int func1Type = 2;
	int func1Value = (strcmp(temp1,"INTSYM") == 0) ? 1:2;
	int defParaNum = 0;
	/////////////////////////
	symTab.blocksnum++;
	symTab.blocksindex[symTab.blocksnum] = symTab.index+1;


	strcpy(func1ID,id);
	strcpy(tempFuncLabel,genLabel());
	
	address++;
	genMidCode(tempFuncLabel," "," ",43);
	
	if(strcmp(temp1,"INTSYM") == 0){
		genMidCode("int",id,tempFuncLabel,1);
	}else if(strcmp(temp1,"CHARSYM") == 0){
		genMidCode("char",id,tempFuncLabel,2);
	}
	
	if(strcmp(symid,"LBRA1") == 0){
		
		while(strcmp(symid,"RBRA1") != 0){
			getsym();
			if(strcmp(symid,"INTSYM") == 0){
				getsym();
				if(strcmp(symid,"IDEN") == 0){
					
					strcpy(tempFuncDef1,token);
					getsym();
					if(strcmp(symid,"COMMA") == 0){
						address++;
						insertSymTab(tempFuncDef1,1,-1,address,-1," ");
						genMidCode("para","int",tempFuncDef1,5);
						defParaNum++;
					}else {
						address++;
						insertSymTab(tempFuncDef1,1,-1,address,-1," ");
						genMidCode("para","int",tempFuncDef1,5);
						defParaNum++;
						break;
					}
				}else{
					error(13,errorRow,errorCol);
					exit(0);
				}
			}else if(strcmp(symid,"CHARSYM") == 0){
				getsym();
				if(strcmp(symid,"IDEN") == 0){
					defParaNum++;
					strcpy(tempFuncDef1,token);
					getsym();
					if(strcmp(symid,"COMMA") == 0){
						address++;
						insertSymTab(tempFuncDef1,1,-2,address,-1," ");
						genMidCode("para","char",tempFuncDef1,6);
						
					}else {
						address++;
						insertSymTab(tempFuncDef1,1,-2,address,-1," ");
						genMidCode("para","char",tempFuncDef1,6);
						
						break;
					}
				}else{
					error(13,errorRow,errorCol);
					exit(0);
				}
			}
		}
		insertSymTab(func1ID,func1Type,func1Value,address,defParaNum,tempFuncLabel);
		
		
		getsym();
		if(strcmp(symid,"LBRA3") == 0){
			getsym();
			ComSen();
			if(strcmp(symid,"RBRA3") == 0){
				getsym();
			}
		}else{
			error(25,errorRow,errorCol);
			exit(0);
		}
		
			

			
			
			
		genMidCode("RET"," "," ",9);
		
		printf("This is a funcdef1 statement with return!\n");
	}else{
		error(15,errorRow,errorCol);
		exit(0);
	}
}
void FuncDef0(){
	funcDef0R = 0;
	char tempFuncPara[256];
	///////////////////////
	char tempFuncDef0Label[256];
	/////////////////////////////
	int defParaNum = 0;
	char func0ID[256];
	int func0Type = 2;
	int func0Value = 0;
	symTab.blocksnum++;
	symTab.blocksindex[symTab.blocksnum] = symTab.index+1;
	/////////////////////////////
	if(strcmp(symid,"IDEN") == 0){
		strcpy(func0ID,token);
		strcpy(tempFuncDef0Label,genLabel());
		
		address++;
		
		
		genMidCode(tempFuncDef0Label," "," ",43);
		genMidCode("void",id,tempFuncDef0Label,3);
		
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			
			while(strcmp(symid,"RBRA1") != 0){
				getsym();
				if(strcmp(symid,"INTSYM") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						defParaNum++;
						strcpy(tempFuncPara,token);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							address++;
							insertSymTab(tempFuncPara,1,-1,address,-1," ");
							genMidCode("para","int",tempFuncPara,5);
							
						}else {
							address++;
							insertSymTab(tempFuncPara,1,-1,address,-1," ");
							genMidCode("para","int",tempFuncPara,5);
							
							break;
						}
					}
				}else if(strcmp(symid,"CHARSYM") == 0){
					getsym();
					if(strcmp(symid,"IDEN") == 0){
						defParaNum++;
						strcpy(tempFuncPara,token);
						getsym();
						if(strcmp(symid,"COMMA") == 0){
							address++;
							insertSymTab(tempFuncPara,1,-2,address,-1," ");
							genMidCode("para","char",tempFuncPara,6);
							
						}else {
							address++;
							insertSymTab(tempFuncPara,1,-2,address,-1," ");
							genMidCode("para","char",tempFuncPara,6);
							
							break;
						}
					}
				}
			}
			insertSymTab(func0ID,func0Type,func0Value,address,defParaNum,tempFuncDef0Label);
			
			
			getsym();

			if(strcmp(symid,"LBRA3") == 0){
				

				getsym();
				ComSen();
				getsym();
				
			}
		}else{
			error(15,errorRow,errorCol);
			exit(0);
		}
		genMidCode("RET"," "," ",9);
		printf("This is a funcdef0 statement without return!\n");
	}else{
		error(26,errorRow,errorCol);
		exit(0);
	}
}

void Constant(){
	if(strcmp(symid,"PLUS") == 0){
		getsym();
		if(strcmp(symid,"UNINT") == 0){
			sprintf(constantNum,"%s",token);
			getsym();
		}
	}else if(strcmp(symid,"MINUS") == 0){
		getsym();
		
		if(strcmp(symid,"UNINT") == 0){
			sprintf(constantNum,"-%s",token);
			getsym();
		}
	}else if(strcmp(symid,"UNINT") == 0){
		sprintf(constantNum,"%s",token);
		getsym();
	}else if(token[0] == '0'){
		sprintf(constantNum,"%s",token);
		getsym();
	}else if(strcmp(symid,"ZIFU") == 0){
		sprintf(constantNum,"%d",token[0]);
		getsym();
	}
}

void Switch(){
	char chenshuwei[256];
	char tempSwitchExp[256];
	char mememe[256];
	strcpy(chenshuwei,genLabel());
	if(strcmp(symid,"SWITCHSYM") == 0){
		getsym();
		if(strcmp(symid,"LBRA1") == 0){
			getsym();
			Exp();
			strcpy(tempSwitchExp,nowTerm);
			if(strcmp(symid,"RBRA1") == 0){
				getsym();
				if(strcmp(symid,"LBRA3") == 0){
					getsym();
					strcpy(mememe,Case(chenshuwei,tempSwitchExp));
					if(strcmp(symid,"DEFAULTSYM") == 0){
						Default(chenshuwei,mememe);	
						if(strcmp(symid,"RBRA3") == 0){
							getsym();
						}
					
						genMidCode(chenshuwei," "," ",43);
					}else{
						error(27,errorRow,errorCol);
						exit(0);
					}
				}
			}
		}else{
			error(15,errorRow,errorCol);
			while(strcmp(symid,"RBRA1") != 0){
				getsym();
			}
		}
		printf("This is a switch statement!\n");
	}
}
char *Case(char *chenshuwei,char *tempSwitchExp){
	char caseVar[256];
	char place[256];
	char caseNumVar[256];
	char nextCaseLabel[256];
	
	strcpy(nextCaseLabel,genLabel());
	while(strcmp(symid,"CASESYM") == 0){
		
		genMidCode(nextCaseLabel," "," ",43);
		getsym();
		Constant();///////constantNum�Ѿ�ȷ��(char *)

		/////����case(1)��caseNumVar1 = 1;
		/*itoa(constantNum,place,10);*/
		strcpy(caseNumVar,genVar());
		genMidCode(caseNumVar,"equal",constantNum,40);
		///////����switch(x) case 1        result = (x==caseNumVar)
		strcpy(caseVar,genVar());
		genMidCode(caseVar,tempSwitchExp,caseNumVar,25);
		strcpy(nextCaseLabel,genLabel());
		genMidCode("BZ",caseVar,nextCaseLabel,28);
		if(strcmp(symid,"COLON") == 0){
			getsym();
			Sentence();
			genMidCode("GOTO",chenshuwei," ",26);
			
		}
		printf("This is a case statement!\n");
	}
	return nextCaseLabel;
	
}
void Default(char *chenshuwei,char *mememe){
	if(strcmp(symid,"DEFAULTSYM") == 0){
		getsym();
		if(strcmp(symid,"COLON") == 0){
			genMidCode(mememe," "," ",43);
			getsym();
			Sentence();
			genMidCode("GOTO",chenshuwei," ",26);
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
	}else if(strcmp(symid,"PRINTFSYM") == 0){
		Printf();
	}else if(strcmp(symid,"RETURNSYM") == 0){
		Return();
	}else if(strcmp(symid,"SWITCHSYM") == 0){
		Switch();
	}else if(strcmp(symid,"IDEN") == 0){
		strcpy(senID,token);
		getsym();
		if(strcmp(symid,"EQUAL") == 0){
			Assign();
		}else if(strcmp(symid,"LBRA1") == 0){
			FuncCall0();
		}else if(strcmp(symid,"LBRA2") == 0){
			Assign();
		}
	}else if(strcmp(symid,"LBRA3") == 0){
		getsym();
		SentenceList();
		if(strcmp(symid,"RBRA3") == 0){
			getsym();
		}
	}else if(strcmp(symid,"SEMI") == 0){
		getsym();
	}
}
void SentenceList(){
	while((strcmp(symid,"IFSYM") == 0) || strcmp(symid,"FORSYM") == 0 || (strcmp(symid,"LBRA3") == 0) ||
		(strcmp(symid,"INTSYM") == 0) || (strcmp(symid,"CHARSYM") == 0) ||
		(strcmp(symid,"IDEN") == 0) || (strcmp(symid,"SCANFSYM") == 0) || (strcmp(symid,"PRINTFSYM") == 0) || 
		(strcmp(symid,"RETURNSYM") == 0) || (strcmp(symid,"SWITCHSYM") == 0) || (strcmp(symid,"SEMI") == 0)){

			Sentence();
			if(strcmp(symid,"RETURNSYM") == 0){
				funcDef0R = 1;
				MainR = 1;
			}
	}
}
void ComSen(){
	if(strcmp(symid,"CONSTSYM") == 0){
		ConExplain();
	}
	if((strcmp(symid,"INTSYM") == 0) || (strcmp(symid,"CHARSYM") == 0)){
		while((strcmp(symid,"INTSYM") == 0) || (strcmp(symid,"CHARSYM") == 0)){
			strcpy(temp1,symid);
			getsym();
			
			if(strcmp(symid,"IDEN") == 0){
				strcpy(id,token);
				getsym();
				if((strcmp(symid,"SEMI") == 0) || (strcmp(symid,"COMMA") == 0) || (strcmp(symid,"LBRA2") == 0)){
					type = 1;
					VarExplain();
						
				}else {
					/////error?
				}
			}
		}
	}
	SentenceList();
}

void grammer(){
	char readTest[256];
	printf("Please input the path of testfile:\n");
	gets(readTest);

	if((fp = fopen(readTest,"r")) ==NULL){
		printf("File cannot be opened!\n");
		return;
	}else{
		init();
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