#ifndef ERROR
#define ERROR
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
void error(int x,int errorRow,int errorCol){
	switch(x){
		case 0:printf("At line(%d),column(%d):\nError(%d):An IDEN starts with a number!\n",errorRow,errorCol,x);		break;
		case 1:printf("At line(%d),column(%d):\nError(%d):An UNINT starts with 0!\n",errorRow,errorCol,x);				break;
		case 2:printf("At line(%d),column(%d):\nError(%d):A single exc!\n",errorRow,errorCol,x);						break;
		case 3:printf("At line(%d),column(%d):\nError(%d):An illegal letter!\n",errorRow,errorCol,x);					break;
		case 4:printf("At line(%d),column(%d):\nError(%d):Neither char or int after a CONST!\n",errorRow,errorCol,x);	break;
		case 5:printf("At line(%d),column(%d):\nError(%d):Not a IDEN after const (char|int)!\n",errorRow,errorCol,x);	break;
		case 6:printf("At line(%d),column(%d):\nError(%d):An illegal single quote!\n",errorRow,errorCol,x);				break;
		case 7:printf("At line(%d),column(%d):\nError(%d):An illegal double quote!\n",errorRow,errorCol,x);				break;
		case 8:printf("At line(%d),column(%d):\nError(%d):A ']' was lost!\n",errorRow,errorCol,x);						break;
		case 9:printf("At line(%d),column(%d):\nError(%d):An error func!\n",errorRow,errorCol,x);						break;
		case 10:printf("At line(%d),column(%d):\nError(%d):An error func!\n",errorRow,errorCol,x);						break;
		case 11:printf("At line(%d),column(%d):\nError(%d):A SEMI was lost!\n",errorRow,errorCol,x);					break;
		case 12:printf("At line(%d),column(%d):\nError(%d):Array subscript is not a number!\n",errorRow,errorCol,x);	break;
		case 13:printf("At line(%d),column(%d):\nError(%d): Not an ID after (int|char)!\n",errorRow,errorCol,x);		break;
		case 14:printf("At line(%d),column(%d):\nError(%d):Step is not a num!\n",errorRow,errorCol,x);					break;
		case 15:printf("At line(%d),column(%d):\nError(%d):A '(' was lost!\n",errorRow,errorCol,x);						break;
		case 16:printf("At line(%d),column(%d):\nError(%d):Not an ID after '('!\n",errorRow,errorCol,x);				break;
		case 17:printf("At line(%d),column(%d):\nError(%d):Not an equal in Cycle!\n",errorRow,errorCol,x);				break;
		case 18:printf("At line(%d),column(%d):\nError(%d):Step is not an ID!\n",errorRow,errorCol,x);					break;
		case 19:printf("At line(%d),column(%d):\nError(%d):A '=' is lost!\n",errorRow,errorCol,x);						break;
		case 20:printf("At line(%d),column(%d):\nError(%d):Not a (+|-) in step!\n",errorRow,errorCol,x);				break;
		case 21:printf("At line(%d),column(%d):\nError(%d):A ')' was lost!\n",errorRow,errorCol,x);						break;
		case 22:printf("At line(%d),column(%d):\nError(%d):Paranums are different when calling!\n",errorRow,errorCol,x);break;
		case 23:printf("At line(%d),column(%d):\nError(%d):Printf ends without ')'!\n",errorRow,errorCol,x);			break;
		case 24:printf("At line(%d),column(%d):\nError(%d):Para without (char|int)!\n",errorRow,errorCol,x);			break;
		case 25:printf("At line(%d),column(%d):\nError(%d):A LBRA3 was lost!\n",errorRow,errorCol,x);					break;
		case 26:printf("At line(%d),column(%d):\nError(%d):Not an ID after void!\n",errorRow,errorCol,x);				break;
		case 27:printf("At line(%d),column(%d):\nError(%d):A default sentence was lost!\n",errorRow,errorCol,x);		break;
		case 28:printf("At line(%d),column(%d):\nError(%d):A ':' was lost! \n",errorRow,errorCol,x);					break;
		case 29:printf("At line(%d),column(%d):\nError(%d):SwitchExp and CaseExp have different types!\n",errorRow,errorCol,x);	break;
		default:																										break;
	}	
}

#endif