#include "asmbler.h"
void * my_malloc(long size) {
    void * ptr = malloc(size);
    /*Check if  failed*/
    if(ptr == NULL){
        printf("malloc failed\n");
        exit(1);/* Exit the program if memory allocation fails */
    }
    return ptr; /* Return void pointer to allocated memory */
}

void get_word(char * line,char * word) {
    int sentence_len,index_line,index_word;
    char c;
    const char divider=',';
    index_line=0;
    index_word = 0;
    sentence_len = strlen(line);
    
    /* cycles threw white spaces untill not a white space or reached the end of the line*/
    c = line[index_line];

    while  ((c == ' ' || c == '\t') && index_line< sentence_len) {

        index_line++;
        c = line[index_line];
    }

   
    if(c==',') {
        word[index_word] = ',';
        sentence_len--;
        index_word++;
    }
    /* create the word */
    
    while (!isspace(c) && c != divider && index_line< sentence_len) {

        word[index_word] = c;
        
        index_word++;
        index_line++;
        c = line[index_line];
    }
    
    /* end of string without \n*/
    word[index_word] = '\0';
    
}

short reservedWordCheck(char * word) {
    int i;
    short result;
    
    /* List of reserved register names */
    const char *registers[TOTAL_REGISTERS] = {
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"
    };
    /* List of reserved command names */
    const char *commands[TOTAL_COMMANDS]={
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "red",
        "prn",
        "rts",
        "stop"};
        result = 1;
    for(i = 0 ; i < TOTAL_COMMANDS ; i++ ){
        if( strcmp(word , commands[i] ) == 0){
            /* If the label matches a command, print an error and mark as invalid */

            result = 0;
        }
    }

    for(i = 0 ; i < TOTAL_REGISTERS ; i++ ){
        if( strcmp(word , registers[i] ) == 0){
            /* If the label matches a register, print an error and mark as invalid */

            result = 0;
        }
    }

    if( strcmp(word , ".data" ) == 0
        || strcmp(word , ".string" ) == 0
        || strcmp(word , ".entry" ) == 0
        || strcmp(word , ".extern" ) == 0
        ||strcmp(word , "macr" ) == 0
        ||strcmp(word , "endmacr" ) == 0 ){

        result = 0;
    }


    return result;
}
