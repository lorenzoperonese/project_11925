#define NREGISTERS 4

void push(FILE* f,char* line,char filename[]);

void pop(FILE* f,char* line,char filename[]);

void function(FILE* f,char* line);

void call(FILE* f,char* line,int* count);

void label(FILE* f,char* line);

void go_to(FILE* f,char* line);

void if_goto(FILE* f,char* line);

void parse(char* buf,FILE* vmfile, FILE* asmfile, char filename[],int* count);

void rimuoviNewLine(char *str);

char* pulisciRiga(char *riga);

char* removepath(char* filename);