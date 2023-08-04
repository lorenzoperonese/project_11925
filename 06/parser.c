#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "translator.h"

void push(FILE* f,char* line,char filename[]){
	line=line+5; 	        //il puntatore punta alla parola dopo push
	if(*line=='s'){			//static
		line=line+7;
        wstatic(f,line,filename);
        fprintf(f,"D=M\n");
	}
	else if(*line=='l'){	//local
		line=line+6;
        wlocal(f,line);
        fprintf(f,"A=D+A\nD=M\n");
	}
	else if(*line=='a'){	//argument
		line=line+9;
        wargument(f,line);
        fprintf(f,"A=D+A\nD=M\n");
	}
	else if(*line=='c'){	//constant
		line=line+9;
		wconstant(f,line);
	}
    wpush(f);
}

void pop(FILE* f,char* line,char filename[]){
    line=line+4;
    if(*line=='s'){			//static
		line=line+7;
        wstatic(f,line,filename);
        fprintf(f,"D=A\n");
	}
	else if(*line=='l'){	//local
		line=line+6;
        wlocal(f,line);
        fprintf(f,"D=D+A\n");
	}
	else if(*line=='a'){	//argument
		line=line+9;
        wargument(f,line);
        fprintf(f,"D=D+A\n");
	}
    wpop(f);
}

void function(FILE* f,char* line){
    line=line+9;
    char* name=(char*)malloc(sizeof(char));
    sscanf(line,"%s",name);
    line=line+strlen(name)+1;
    int local_v;
    sscanf(line,"%d",&local_v);
    wfunction(f,name,local_v);
    free(name);
}

void call(FILE* f,char* line,int* count){
    line=line+5;
    char* name=(char*)malloc(sizeof(char));
    sscanf(line,"%s",name);
    line=line+strlen(name)+1;
    int arg;
    sscanf(line,"%d",&arg);
    wcall(f,name,arg,count);
    free(name);
}

void label(FILE* f,char* line){
    line=line+6;
    char* name=(char*)malloc(sizeof(char));
    sscanf(line,"%s",name);
    wlabel(f,name);
    free(name);
}

void go_to(FILE* f,char* line){
    line=line+5;
    char* name=(char*)malloc(sizeof(char));
    sscanf(line,"%s",name);
    wgoto(f,name);
    free(name);
}

void if_goto(FILE* f,char* line){
    line=line+8;
    char* name=(char*)malloc(sizeof(char));
    sscanf(line,"%s",name);
    wif_goto(f,name);
    free(name);
}

void parse(char* buf,FILE* vmfile, FILE* asmfile, char filename[],int* count) {
    buf=pulisciRiga(buf);
    if(strlen(buf)>0){
        rimuoviNewLine(buf);
        fprintf(asmfile,"//%s\n",buf);
        char tmp[10];
        sscanf(buf,"%s",tmp);
        if(strcmp(tmp,"push")==0)
            push(asmfile,buf,filename);
        else if(strcmp(tmp,"pop")==0)
            pop(asmfile,buf,filename);
        else if(strcmp(tmp,"function")==0)
            function(asmfile,buf);
        else if(strcmp(tmp,"call")==0)
            call(asmfile,buf,count);
        else if(strcmp(tmp,"return")==0)
            wreturn(asmfile);
        else if(strcmp(tmp,"label")==0)
            label(asmfile,buf);
        else if(strcmp(tmp,"goto")==0)
            go_to(asmfile,buf);
        else if(strcmp(tmp,"if-goto")==0)
            if_goto(asmfile,buf);
        else if(strcmp(tmp,"add")==0)
            wadd_wsub(asmfile,'+');
        else if(strcmp(tmp,"sub")==0)
            wadd_wsub(asmfile,'-');
        else if(strcmp(tmp,"neg")==0)
            wneg_wnot(asmfile,'-');
        else if(strcmp(tmp,"not")==0)
            wneg_wnot(asmfile,'!');
        else if(strcmp(tmp,"and")==0)
            wand_wor(asmfile,'&');
        else if(strcmp(tmp,"or")==0)
            wand_wor(asmfile,'|');
        else if(strcmp(tmp,"eq")==0)
            wcomp(asmfile,"JEQ",count);
        else if(strcmp(tmp,"lt")==0)
            wcomp(asmfile,"JLT",count);
        else if(strcmp(tmp,"gt")==0)
            wcomp(asmfile,"JGT",count);
    }else
        fprintf(asmfile,"//Linea vuota\n");
}


void rimuoviNewLine(char *str){
	if(str[strlen(str) - 1] == '\n' && str[strlen(str) - 2] == '\r')
        str[strlen(str) - 2] = '\0';
}

char *pulisciRiga(char *riga) {
    char *commento = riga;
    char *commentopre=riga;
    commento++;
    if(*commento=='\0'||*commentopre=='\0') return riga;
    int flag=0;
    while(*commento!='\0'&&*commentopre!='\0'&&flag==0){
        if(*commento=='/'&&*commentopre=='/')
            flag++;
        else{
            commentopre++;
            commento++;
        }
    }
    if(*commento != '\0') {
        *commentopre = '\0';
    }

    return riga;
}

char* removepath(char* filename){
    char* tmp=filename;
    while(*tmp!='\0'){
        if(*tmp=='/')
            strcpy(filename,tmp+1);
        tmp++;
    }
    return filename;
}
