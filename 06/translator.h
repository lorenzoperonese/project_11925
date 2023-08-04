void wconstant(FILE* f,char* val);

void wlocal(FILE* f,char* val);

void wstatic(FILE* f,char* val,char filename[]);

void wargument(FILE* f,char* val);

void wpush(FILE* f);

void wpop(FILE* f);

void wadd_wsub(FILE* f,char op);

void wneg_wnot(FILE* f,char op);

void wand_wor(FILE* f,char op);

void wcomp(FILE* f,char op[],int* count);

void wfunction(FILE* f,char* name,int local_v);

void wcall(FILE* f,char* name,int arg,int* count);

void wreturn(FILE* f);

void wlabel(FILE* f,char* name);

void wgoto(FILE* f,char* name);

void wif_goto(FILE* f,char* name);