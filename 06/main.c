#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "translator.h"

int main(int argc, char *argv[]) {

    if(argc == 2) 
    {
    	FILE* vmfile;
    	vmfile=fopen(argv[1], "r");

     if( vmfile==NULL ) {
          printf("Errore in apertura del file da leggere: %s\n", argv[1]);
          exit(1);
     }

     FILE* asmfile;
     char filename[50];

     strcpy(filename,argv[1]);

     filename[strlen(filename)-2] = '\0';
     strcat(filename, "asm");
                    
     asmfile = fopen(filename, "w");
	fprintf(asmfile,"@256\nD=A\n@SP\nM=D\n");
     int x=0;
     int* count=&x;
     filename[strlen(filename)-4] = '\0';
     strcpy(filename,removepath(filename));
     char buf[256];
     while(fgets(buf, 256 , vmfile)!=NULL)
          parse(buf,vmfile, asmfile,filename,count);
     fclose(vmfile);
     fclose(asmfile);

     }else
          printf("Numero di argomenti errato");
     
}