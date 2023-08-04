#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "symbolTable.h"

tabella compTable = {
    28, {
    {"0",   42},
    {"1",   63},
    {"-1",   58},
    {"D",   12},
    {"A",   48},
    {"!D",   13},
    {"!A",   49},
    {"-D",   15},
    {"-A",   51},
    {"D+1",   31},
    {"A+1",   55},
    {"D-1",   14},
    {"A-1",   50},
    {"D+A",   2},
    {"D-A",   19},
    {"A-D",   7},
    {"D&A",   0},
    {"D|A",   21},
    {"M",   112},
    {"!M",   113},
    {"-M",   115},
    {"M+1",   119},
    {"M-1",   114},
    {"D+M",   66},
    {"D-M",   83},
    {"M-D",   71},
    {"D&M",   64},
    {"D|M",   85}
    }
};

tabella jumpTable = {
    7, {
    {"JGT", 1}, {"JEQ", 2}, {"JGE", 3}, {"JLT", 4}, {"JNE", 5}, {"JLE", 6}, {"JMP", 7}
    }
};

tabella destTable = {
    7, {
    {"M",   1}, {"D",   2}, {"MD",  3}, {"A",  4}, {"AM",  5}, {"AD",  6}, {"AMD",  7}
    }
};

instruction *parseInstruction(char *riga) {
    printf("Istruzione pre parse: %s\n",riga);
    riga = pulisciRiga(riga);

    if(strlen(riga) == 0) {
        printf("riga vuota\n");
        return NULL;
    }

    if(riga[0] == '(') return NULL;
    
    if(riga[0] == '@') {
        if(riga[1] <= '9' && riga[1] >= '0') {
        instruction* ainstr=(instruction*)malloc(sizeof(instruction));
        riga++;
        unsigned int* tmp=(unsigned int*)malloc(sizeof(unsigned int));
        sscanf(riga,"%u",tmp);
        if(*tmp<(1<<16)){
                ainstr->valore=*tmp;
                ainstr->type=A;
                printf("valore a instr: %d\n",*tmp);

                return ainstr;
            }
        }
    }
    else {
        unsigned char comp, dest, jump;
        parseC(riga,&comp,&dest,&jump);
        instruction* cinstr=((instruction*)malloc(sizeof(instruction)));
        cinstr->type=C;
        cinstr->comp=comp;
        if(dest==MISSING)
            cinstr->dest=0;
        else
            cinstr->dest=dest;
        if(jump==MISSING)
            cinstr->jump=0;
        else
            cinstr->jump=jump;

        return cinstr;
    }
    
    abort();

}

//fa il parsing della C-instruction ed assegna i valori alle 3 componenti
void parseC(char *riga, unsigned char *comp, unsigned char *dest, unsigned char *jump) {
	*dest = 0;
    *jump = 0;
    int compflag=0,jumpflag=0;
    char *jumpSubString = riga;
    while(*jumpSubString!='\0' && jumpflag==0){
        if(*jumpSubString==';'){
            jumpflag=1;
        }
        else{
            jumpSubString++;
        }
    }
    if(*jumpSubString!= '\0') {
        printf("; presente\n");
        *jumpSubString = '\0';
        jumpSubString++; 
        jumpSubString = rimuoviSpazi(jumpSubString);
        *jump = traduciInBinario(jumpSubString, &jumpTable);
    }
    else{
        printf("; assente\n");
        *jump = MISSING;
    }
    char *compSubString = riga;
    while(*compSubString!='\0' && compflag==0){
        if(*compSubString=='=')
            compflag=1;
        else
            compSubString++;
    }
    if(*compSubString != '\0') {
        printf("= presente\n");
        *compSubString = '\0';
        compSubString++; 
        compSubString = rimuoviSpazi(compSubString);
        *comp = traduciInBinario(compSubString, &compTable);

        riga = rimuoviSpazi(riga);
        *dest=traduciInBinario(riga,&destTable);
    }
    else{
        printf("= assente");
        riga = rimuoviSpazi(riga);
        *comp = traduciInBinario(riga, &compTable);
        *dest=MISSING;
    }
    printf("valore c instr comp: %d, dest: %d, jump: %d\n",*comp,*dest,*jump);

}

int isInstruction(char *riga) {
    riga=pulisciRiga(riga);
    if(strlen(riga) == 0) return 0;
    else if(riga[0] == '(') return 0;
    else return 1;
}

//traduce, semplicemente cercando nelle tabelle
unsigned char traduciInBinario(char *chiave, tabella *tabella) {
    
    int i;
    for(i = 0; i < tabella->lunghezza; i++) {
        if(strcmp(chiave, tabella->righe[i].chiave) == 0) {
            return tabella->righe[i].valore;
        }
    }

    return MISSING;
}


char *pulisciRiga(char *riga) {
    riga = rimuoviSpazi(riga);
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
        riga = rimuoviSpazi(riga);
    }

    return riga;
}

char *rimuoviSpazi(char *str) {
    int i=0;
    int count = 0;
    while(str[i]!='\0'){
        if (str[i] != ' '){
            str[count] = str[i];
            count++;
        }
        i++;
    }
    str[count] = '\0';

    return str; 
}

void rimuoviNewLine(char *str){
	if(str[strlen(str) - 1] == '\n' && str[strlen(str) - 2] == '\r')
        str[strlen(str) - 2] = '\0';
}
