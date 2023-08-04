#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "translator.h"

const char reg[5][5]={"LCL","ARG","THIS","THAT"};

void wconstant(FILE* f,char* val){
	fprintf(f,"@%s\nD=A\n",val);
}

void wlocal(FILE* f,char* val){
	fprintf(f,"@LCL\nD=M\n@%s\n",val);
}

void wstatic(FILE* f,char* val,char filename[]){
	fprintf(f,"@%s.%s\n",filename,val);
}

void wargument(FILE* f,char* val){
	fprintf(f,"@ARG\nD=M\n@%s\n",val);
}

void wpush(FILE* f){
	fprintf(f,"@SP\nM=M+1\nA=M-1\nM=D\n");
}

void wpop(FILE* f){
	fprintf(f,"@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n");
}

void wadd_wsub(FILE* f,char op){
	fprintf(f,"@SP\nMD=M-1\nA=D\nD=M\nA=A-1\nM=M%cD\n",op);
}

void wneg_wnot(FILE* f,char op){
	fprintf(f,"@SP\nA=M-1\nM=%cM\n",op);
}

void wand_wor(FILE* f,char op){
	fprintf(f,"@SP\nMD=M-1\nA=D\nD=M\nA=A-1\nM=M%cD\n",op);
}

void wcomp(FILE* f,char op[],int* count){
	fprintf(f,"@SP\nMD=M-1\nA=D\nD=M\nA=A-1\nM=M-D\nD=M\n@$%d\nD;%s\nD=0\n@$%d\n0;JMP\n" 
	"($%d)\nD=-1\n($%d)\n@SP\nA=M-1\nM=D\n",*count,op,*count+1,*count,*count+1);
	(*count)+=2;
}

void wfunction(FILE* f,char* name,int local_v){
	fprintf(f,"(%s) \n",name);
	for(int i=0;i<local_v;i++){
		wconstant(f,"0");
		wpush(f);
	}
}

void wcall(FILE* f,char* name,int arg,int* count){
	fprintf(f,"@$%d\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n",*count);
	for(int i=0;i<NREGISTERS;i++){
		fprintf(f,"@%s\nD=M\n",reg[i]);
		wpush(f);
	}
	fprintf(f,"@SP\nD=M\n@%d\nD=D-A\n@ARG\nM=D\n@SP\nD=M\n@LCL\nM=D\n@%s\n0;JMP\n($%d)\n",(arg+NREGISTERS+1),name,*count);
	(*count)++;
}

void wreturn(FILE* f){
	fprintf(f,"@LCL\nD=M\n@R14\nM=D\n@%d\nA=D-A\nD=M\n@R15\nM=D\n@ARG\nD=M\n",NREGISTERS+1);
	wpop(f);
	fprintf(f,"@ARG\nD=M+1\n@SP\nM=D\n");
	for(int i=NREGISTERS-1;i>=0;i--)
		fprintf(f,"@R14\nD=M\n@%d\nA=D-A\nD=M\n@%s\nM=D\n",NREGISTERS-i,reg[i]);
	fprintf(f,"@R15\nA=M\n0;JMP\n");
}

void wlabel(FILE* f,char* name){
	fprintf(f,"(%s)\n",name);
}

void wgoto(FILE* f,char* name){
	fprintf(f,"@%s\n0;JMP\n",name);
}

void wif_goto(FILE* f,char* name){
	fprintf(f,"@SP\nAM=M-1\nD=M\n@%s\nD;JNE\n",name);
}