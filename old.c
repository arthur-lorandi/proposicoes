#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

double pow(double x, double y){
    double res = x;

    if(y==0)
        return 1;
    for( ; y>1; y--)
        res *= x;
    return res;
}

char *pegaVars(char *prop){
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

void updateVars(char *vars, char *newVars){
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

void updateProps(LIST *propsList, char *prop){
    LIST *aux=propsList;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = (LIST*)malloc(sizeof(LIST));
    strcpy(aux->prox->info, prop);
    aux->prox->prox = NULL;
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

    char *tabHead[numCol];
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
    
    bool *tabBody[numCol];
    //preenche tabBody das vars
    for(i=numVars-1; i>=0; i--, mult*=2){
        tabBody[i] = (bool*)malloc(numLin * sizeof(bool));
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

bool not(bool var1, bool var2){
    
    return true;
}
bool and(bool var1, bool var2){

    return true;
}
bool or(bool var1, bool var2){

    return true;
}
bool impl(bool var1, bool var2){

    return true;
}
bool eqvl(bool var1, bool var2){

    return true;
}
//                   ~  /\  \/  -->  <->
//                   ~  /   \   -    <
bool resolveProp(char *propRef, char *vars, char *vals){
    PILHA P_oprs, P_vals;
    PILHA *T_oprs = P_oprs;// ASFDASFASFSAf
    char delim[] = "~/\\-<";
    int i, j, k;
    char *prop = (char*)malloc(sizeof(propRef));
    strcpy(prop, propRef);
    char *aux = prop;
    // troca letras por valores logicos
    for(i=0; vars[i]!='\0'; i++){
        while(strchr(aux, vars[i])!=NULL){
            aux = strchr(aux, vars[i]);
            aux[0] = vals[i];
        }
    }
    // !!! TODA VEZ QUE ANTES DO VAL TIVER ~ MANDA VAL PRA PILHA TROCADO !!!
    // !!! TODA VEZ QUE ANTES DE '(' TIVER ~ MANDA OPR PRA PILHA DIRETO !!!
    for(i=0; prop[i]!='\0'; i++){
        switch(prop[i]){
            case '~':
                if(prop[i+1] == '('){
                    PILHA *novo = (PILHA*)malloc(sizeof(PILHA));
                    novo->info = prop[i];

                }
                break;
            case '/':

                break;
            case '\\':

                break;
            case '-':

                break;
            case '<':

                break;
            case ')':

                break;
        }
    }

}

void resolveTab(TAB tabela, char *vars){
    int i, j, cont=0;
    int numVars = strlen(vars);
    int numLin = pow(2, numVars);
    char vals[numVars];

    // definir vals
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

}

int main(){
    int option, i;
    char prop[50];
    char *vars=NULL;
    LIST *propsList=NULL;
    TAB tabela;
            
    printf("0. Sair\n");
    printf("1. Adicionar proposicao\n");
    printf("2. Ver variaveis\n");
    printf("3. Ver tabela-verdade\n");
    printf("4. Salvar tabela-verdade\n");
    scanf("%d", &option);

    while(option != 0){
        switch(option){
            case 1:
                printf("~   /\\   \\/   -->   <->\n");
                fflush(stdin);
                gets(prop);
                strcpy(prop, strupr(prop));
                if(vars==NULL){
                    vars = pegaVars(prop);
                    
                    propsList = (LIST*)malloc(sizeof(LIST));
                    strcpy(propsList->info, prop);
                    propsList->prox = NULL;
                }else{
                    updateVars(vars, pegaVars(prop));
                    updateProps(propsList, prop);
                }
                break;
            case 2:
                if(vars!=NULL){
                    for(i=0; vars[i]!='\0'; i++)
                        printf("%c ", vars[i]);
                }else
                    printf("Nao foram incluidas variaveis ainda\n");
                break;
            case 3:
                if(propsList!=NULL){
                    tabela = montaTab(propsList, vars);
                    resolveTab(tabela, vars);
                }else{
                    printf("Nao foram incluidas preposicoes ainda\n");
                }

                break; 
            case 4:
                //implementacao apos
                break;
            case 5:
                //??
                break;
        }
        printf("\n0. Sair\n");
        printf("1. Adicionar proposicao\n");
        printf("2. Ver variaveis\n");
        printf("3. Ver tabela-verdade\n");
        printf("4. Salvar tabela-verdade\n");
        scanf("%d", &option);
    }

}
