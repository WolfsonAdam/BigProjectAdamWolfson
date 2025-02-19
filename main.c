#include <stdio.h>
#include "asmbler.h"
int main(void) {
    char *file_am;
    char file_as[MAXLINE];
    MACRO_LIST_NODE *  macro_head=NULL;  /* Head of macro list */
    LABEL_LIST *  label_head=NULL; /* Head of label list */
    ENTRY_LIST * entry_head=NULL; /* Head of entry list */
    EXTERN_LIST * extern_head=NULL; /* Head of extern list */
    MACHINE_CODE_COMMAND * command_head=NULL; /* Head of command list */
    MACHINE_CODE_INSTRUCTION * instruction_head=NULL; /* Head of instruction list */
    ASSEMBLER_TABLE assembler ,*assemblerPtr ;
    assemblerPtr=&assembler;
    assemblerPtr->macro_head=macro_head;
    assemblerPtr->label_head=label_head;
    assemblerPtr->entry_head=entry_head;
    assemblerPtr->command_head=command_head;
    assemblerPtr->extern_head=extern_head;
    assemblerPtr->instruction_head=instruction_head;
    
    strcpy(file_as,"test");
    
    preProcess(&macro_head,file_as,&file_am);
    firstpass(&assemblerPtr,file_am);
    free_assembler_table(&assemblerPtr);
    return 0;
}
