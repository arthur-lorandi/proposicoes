#include <stdio.h>
#include <string.h>

void main(){
    char str[] = "churros";

    int strTam = strlen(str);
    int espDef = 3;
    for(int i=0; i<=2; i++)
        printf("|%*s%-*s", espDef, "", espDef+strTam, str);
    printf("|\n");
    printf("|\n");
    
}