#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BASE_ALLING 2

typedef struct list{
    char info[50];
    struct list *prox;
} LIST;

typedef struct pilha{
    char info;
    struct pilha *prox;
} PILHA;

typedef struct tab{
    char **tabHead;
    bool **tabBody;
} TAB;

char not(char var){
    if(var == 'T')
        return 'F';
    return 'T';
}
char and(char var1, char var2){
    if(var1 == 'T' && var2 == 'T')
        return 'T';
    return 'F';
}
char or(char var1, char var2){
    if(var1 == 'T' || var2 == 'T')
        return 'T';
    return 'F';
}
char impl(char var1, char var2){
    if(var1 == 'T' && var2 == 'F')
        return 'F';
    return 'T';
}
char eqvl(char var1, char var2){
    if(var1 == var2)
        return 'T';
    return 'F';
}

int pow(int x, int y){//double?
    int res = x;

    if(y==0)
        return 1;
    for( ; y>1; y--)
        res *= x;
    return res;
}

PILHA *empilha(PILHA *pilha, char info){
    PILHA *novo = (PILHA*)malloc(sizeof(PILHA));
    
    if(novo!=NULL){
        novo->info = info;
        novo->prox = pilha;
        return novo;
    }else
        printf("ERRO EM ALOCAR MEMORIA\n");
    return NULL;
}

char desempilha(PILHA **pilha){
    PILHA *aux;
    char info;

    if(*pilha!=NULL){
        aux = *pilha;
        info = aux->info;
        *pilha = aux->prox;
        free(aux);
    }else
        printf("pilha vazia\n");

    return info;
}

char *getVars(char *prop){
    int i, newSize;
    char *vars = (char*)malloc(sizeof(char));
       
    vars[0] = '\0';
    for(i=0; prop[i]!='\0'; i++){
        if(prop[i] >= 'A' && prop[i] <= 'Z' && strchr(vars, prop[i])==NULL){
            newSize = strlen(vars)+2;
            vars = (char*)realloc(vars, newSize);
            vars[newSize-1] = '\0';
            vars[newSize-2] = prop[i];
        }
    }
    return vars;
}

void addVars(char *vars, char *newVars){
    int i, j;
    char *aux = NULL;

    strcat(vars, newVars);
    for(i=0; vars[i]!='\0'; i++){
        for(j=1+i; vars[j]!='\0'; j++){
            if(vars[i] == vars[j])
                vars[j] = '*';
        }
    }
    strcpy(newVars, "");
    aux = strtok(vars, "*");
    while(aux!=NULL){
        strcat(newVars, aux);
        aux = strtok(NULL, "*");
    }
    strcpy(vars, newVars);
}

void addProps(LIST *propsList, char *prop){
    LIST *aux=propsList;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = (LIST*)malloc(sizeof(LIST));
    strcpy(aux->prox->info, prop);
    aux->prox->prox = NULL;
}

char *remProp(LIST **propsList, int index){//QUEBRA SE TIVER 1 PROP SO
    int i;
    LIST *aux = *propsList;
    LIST *ant = *propsList;
    char *info;

    if(index == 0){
        if(aux->prox==NULL){
            *propsList = NULL;
            
        }
        *propsList = aux->prox;//<---
    }else{
        for(i=0; i<index; i++){
            ant = aux;
            aux = aux->prox;
        }
        ant->prox = aux->prox;
    }
    info = (char*)malloc(strlen(aux->info) * sizeof(char));
    strcpy(info, aux->info);
    free(aux);

    return info;
}

TAB montaTab(LIST *propsList, char *vars){
    int i, j, mult=1, cont=1; 
    int numProps, numVars, numLin, numCol;
    bool item = false;
    LIST *aux = propsList;
    TAB tabela;

    numVars = strlen(vars);
    numLin = pow(2, numVars);
    for(numProps=1; aux->prox!=NULL; numProps++)
        aux = aux->prox;
    numCol = numVars + numProps + 1;

    char **tabHead;
    tabHead = (char**)malloc(numCol * sizeof(char));

    //preenche tabHead
    for(i=0; vars[i]!='\0'; i++){
        tabHead[i] = (char*)malloc(2*sizeof(char));
        tabHead[i][0] = vars[i];
        tabHead[i][1] = '\0';
    }
    aux = propsList;
    for( ; aux!=NULL; i++){
        tabHead[i] = (char*)malloc(strlen(aux->info) * sizeof(char));
        strcpy(tabHead[i], aux->info);
        aux = aux->prox;
    }
    tabHead[i] = (char*)malloc(4*sizeof(char));
    strcpy(tabHead[i], "VAL");
    
    //aloca memoria de tabBody
    bool **tabBody;
    tabBody = (bool**)malloc(numCol * sizeof(bool));
    for(i=numCol; i>=0; i--)
        tabBody[i] = (bool*)malloc(numLin * sizeof(bool));

    //preenche tabBody das vars
    for(i=numVars-1; i>=0; i--, mult*=2){
        for(j=numLin-1; j>=0; j--, cont++){
            tabBody[i][j] = item;
            if(cont==mult){
                if(item)
                    item = false;
                else
                    item = true;
                cont = 0;
            }
        }
    }

    tabela.tabBody = tabBody;
    tabela.tabHead = tabHead;
    return tabela;
}

bool resolveProp(char *propRef, char *vars, char *vals){
    PILHA *P_oprs = NULL, *P_vals = NULL;
    int i;
    char *prop = (char*)malloc(sizeof(propRef));
    strcpy(prop, propRef);
    char *aux;
    // troca letras por valores logicos | PROBLEMA
    for(i=0; vars[i]!='\0'; i++){
        aux = prop;
        while(strchr(aux, vars[i])!=NULL){
            aux = strchr(aux, vars[i]);
            aux[0] = vals[i];
        }
    }
    // !!! TODA VEZ QUE ANTES DO VAL TIVER ~ MANDA VAL PRA PILHA TROCADO !!!
    // !!! TODA VEZ QUE ANTES DE '(' TIVER ~ MANDA OPR PRA PILHA DIRETO !!!
    for(i=0; prop[i]!='\0'; i++){
        if(prop[i] == 'T' || prop[i] == 'F'){
            P_vals = empilha(P_vals, prop[i]);
        }else if(prop[i] == '~'){
            if(prop[i+1] == 'T' || prop[i+1] == 'F'){
                P_vals = empilha(P_vals, not(prop[i+1]));
            }else if(prop[i+1] == '('){
                P_oprs = empilha(P_oprs, '~');
            }
            i++;
        }else if(prop[i] == '/' || prop[i] == '\\' || prop[i] == '-' || prop[i] == '<'){
            P_oprs = empilha(P_oprs, prop[i]);
            i++;
        }else if(prop[i] == ')'){
            switch(desempilha(&P_oprs)){
                case '~':
                    P_vals = empilha(P_vals, not(desempilha(&P_vals)) );
                    break;
                case '/':
                    P_vals = empilha(P_vals, and(desempilha(&P_vals), desempilha(&P_vals)) );
                    break;
                case '\\':
                    P_vals = empilha(P_vals, or(desempilha(&P_vals), desempilha(&P_vals)) );
                    break;
                case '-':
                    P_vals = empilha(P_vals, impl(desempilha(&P_vals), desempilha(&P_vals)) );
                    break;
                case '<':
                    P_vals = empilha(P_vals, eqvl(desempilha(&P_vals), desempilha(&P_vals)) );
                    break;
            }
            if(P_oprs->info == '~'){
                desempilha(&P_oprs);
                P_vals = empilha(P_vals, not(desempilha(&P_vals)) );
                //resolver problema ~(T /\ T) -> T
            }
        }
    }
    while(P_oprs!=NULL){
        switch(desempilha(&P_oprs)){
            case '~':
                P_vals = empilha(P_vals, not(desempilha(&P_vals)) );
                break;
            case '/':
                P_vals = empilha(P_vals, and(desempilha(&P_vals), desempilha(&P_vals)) );
                break;
            case '\\':
                P_vals = empilha(P_vals, or(desempilha(&P_vals), desempilha(&P_vals)) );
                break;
            case '-':
                P_vals = empilha(P_vals, impl(desempilha(&P_vals), desempilha(&P_vals)) );
                break;
            case '<':
                P_vals = empilha(P_vals, eqvl(desempilha(&P_vals), desempilha(&P_vals)) );
                break;
        }
    }

    if(P_vals->info=='T')
        return true;
    else
        return false;
}

void resolveTab(TAB tabela, char *vars){
    int i, j, cont=0;
    int numVars = strlen(vars);
    int numLin = pow(2, numVars);
    int numCol;
    char vals[numVars];

    while(strcmp(tabela.tabHead[numVars+cont], "VAL") != 0){
        for(j=0; j<numLin; j++){
            for(i=0; i<numVars; i++){
                if(tabela.tabBody[i][j])
                    vals[i] = 'T';
                else
                    vals[i] = 'F';
            }
            tabela.tabBody[numVars+cont][j] = resolveProp(tabela.tabHead[numVars+cont], vars, vals);
        }
        cont++;
    }
    numCol = cont + numVars + 1;
    for(j=0; j<numLin; j++){
        for(i=numVars; i<numCol; i++){
            if(tabela.tabBody[i][j]==false){
                tabela.tabBody[numCol-1][j] = false;
                break;
            }else if(tabela.tabBody[i][j]==true && i==numCol-2)
                tabela.tabBody[numCol-1][j] = true;
        }
    }
}

void mostraTab(TAB tabela){
    int i, j, numCol, numLin;
    int allingRight, allingLeft;

    for(i=0, numCol=1; strcmp(tabela.tabHead[i], "VAL")!=0; i++, numCol++);
    for(i=0, numLin=1; tabela.tabHead[i][1]=='\0'; i++, numLin*=2);
    
    int tamCol[numCol];
    for(i=0; i<numCol; i++)
        tamCol[i] = strlen(tabela.tabHead[i]);

    for(i=0; i<numCol; i++){
        printf("|%*s%-*s", BASE_ALLING, "", BASE_ALLING+tamCol[i], tabela.tabHead[i]);
    }
    printf("|\n");

    for(j=0; j<numLin; j++){
        for(i=0; i<numCol; i++){
            allingRight = BASE_ALLING + tamCol[i] - tamCol[i]/2;
            allingLeft = BASE_ALLING + tamCol[i]/2;
            printf("|%*s%-*s", allingLeft, "", allingRight, tabela.tabBody[i][j]?"V":"F");
        }
        printf("|\n");
    }
}

int main(){ //COM 5 VARS OU 4 PROPS O PROG TRAVA
    int option, i;
    char prop[50];
    char *vars=NULL;
    LIST *propsList=NULL, *aux=NULL;
    TAB tabela;
            
    printf("\t0. Sair\n");
    printf("\t1. Adicionar proposicao\n");
    printf("\t2. Remover preposicao\n");
    printf("\t3. Ver variaveis\n");
    printf("\t4. Ver tabela-verdade\n");
    printf("\t5. Salvar tabela-verdade\n");
    scanf("%d", &option);

    while(option != 0){
        switch(option){
            case 1:
                printf("~   /\\   \\/   ->   <>\n");
                fflush(stdin);
                gets(prop);
                strcpy(prop, strupr(prop));
                if(vars==NULL){
                    vars = getVars(prop);
                    
                    propsList = (LIST*)malloc(sizeof(LIST));
                    strcpy(propsList->info, prop);
                    propsList->prox = NULL;
                }else{
                    addVars(vars, getVars(prop));
                    addProps(propsList, prop);
                }
                break;
            case 2:
                if(propsList!=NULL){
                    aux = propsList;
                    printf("Remover qual proposicao?\n");
                    for(i=0; aux!=NULL; i++){
                        printf("\t[%d] %s\n", i, aux->info);
                        aux = aux->prox;
                    }
                    scanf("%d", &i);
                    printf("A preposicao [%s] foi removida.\n", remProp(&propsList, i));
                }else{
                    printf("Nao foram incluidas proposicoes ainda.\n");
                }
                break;
            case 3:
                if(vars!=NULL){
                    for(i=0; vars[i]!='\0'; i++)
                        printf("%c ", vars[i]);
                }else
                    printf("Nao foram incluidas variaveis ainda.\n");
                break;
            case 4:
                if(propsList!=NULL){
                    tabela = montaTab(propsList, vars);
                    resolveTab(tabela, vars);
                    mostraTab(tabela);
                }else{
                    printf("Nao foram incluidas preposicoes ainda.\n");
                }
                break; 
            case 5:
                //implementacao apos
                break;
            case 6:
                //??
                break;
        }
        printf("\n0. Sair\n");
        printf("1. Adicionar proposicao\n");
        printf("2. Remover preposicao\n");
        printf("3. Ver variaveis\n");
        printf("4. Ver tabela-verdade\n");
        printf("5. Salvar tabela-verdade\n");
        scanf("%d", &option);
    }

    return 0;
}

    /*
    P_aux = P_vals; // ~T -> ~( T /\ T )
    while(P_aux != NULL){
        printf("1-%c\n", P_aux->info);
        P_aux = P_aux->prox;
    }
    P_aux = P_oprs;
    while(P_aux != NULL){
        printf("2-%c\n", P_aux->info);
        P_aux = P_aux->prox;
    }
    */