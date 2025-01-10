#include <stdio.h>
#include "asmbler.h"
int main(void) {
    FILE *fp2;
    
    char a[MAXLINE]="test";
    
    MACRO_LIST_NODE * macro_head=NULL;
    preProcess(&macro_head,a,&fp2);
    

    return 0;
}
