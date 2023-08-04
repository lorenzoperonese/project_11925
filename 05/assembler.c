#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "parser.h"
#include "symbolTable.h"

//data è il *numero* corrispondente alla codifica binaria a 16 bit
void output(FILE *f, unsigned short data) {
    char temp[18];
	int j;
    for(j = 0; j < 16; j++) {
        temp[j] = (data >> (15-j)) & 1 ? '1' : '0';
    }

	temp[16] = '\n';
    temp[17] = '\0';

    printf("in binario: %s", temp);
    fprintf(f, "%s", temp);
    
}


//legge il file, traduce, ed invoca output
void assemble(FILE *asmfile, FILE *hackfile) {
    char buf[256];
    symbolTable st;
    initDefault(&st);

    while(fgets(buf, 256 , asmfile)!=NULL) {
	rimuoviNewLine(buf);
	printf("ho letto %s\n", buf);
	parseSymbols(buf, &st);
    }
 
    rewind(asmfile);

    while(fgets(buf, 256 , asmfile)!=NULL) {
	rimuoviNewLine(buf);
	replaceSymbols(buf, &st);
	instruction* op;
 	op = parseInstruction(buf);
	if(op != NULL) {
	    unsigned short data;
	    if(op->type == A) {
			printf("a instr: metto in data il valore %d\n",op->valore);
	    	data = op->valore; 	
	    }
		else {
    	data=(7<<13)+(op->comp<<6)+(op->dest<<3)+op->jump;
		}

	   	printf("numero: %u\n", data);
 		output(hackfile, data);
	   	free(op);
	   	}

	}
}




