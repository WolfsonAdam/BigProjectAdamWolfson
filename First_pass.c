
#include "asmbler.h"

void insert_Label_List(LABEL_LIST **label_list, char *label, int address){
    LABEL_LIST *ptr;
    LABEL_LIST *new_node ;

    ptr = NULL;
    new_node = my_malloc(sizeof(LABEL_LIST));
    /* Initialize the new label node */
    memset(new_node->label, '\0', sizeof(new_node->label));
    strcpy(new_node->label, label);
    new_node->label[MAXLABLE - 1] = '\0';
    new_node->addr_list->adr = address;
    new_node->addr_list->next = NULL;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*label_list == NULL)
    {
        *label_list = new_node;

    } else
    {
        /* Traverse the list to find the last node */
        ptr = *label_list;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }

}

void insert_Entry_List(ENTRY_LIST **entry_list, char *label) {
    ENTRY_LIST *ptr;
    ENTRY_LIST *new_node;
    new_node = my_malloc(sizeof(LABEL_LIST));
    ptr = NULL;

    /* Initialize the new label node */
    memset(new_node->label, '\0', sizeof(new_node->label));
    strcpy(new_node->label, label);
    new_node->label[MAXLABLE - 1] = '\0';
    new_node->addr_list = NULL;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*entry_list == NULL)
    {
        *entry_list = new_node;

    } else
    {
        /* Traverse the list to find the last node */
        ptr = *entry_list;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }

}
/* same data structure */
void insert_Extern_List(EXTERN_LIST **entry_list, char *label) {
    insert_Entry_List(entry_list, label);
}

int first_pass_line_type(char *word, char *commands[]) {
    int i;
    if(strcmp(word, ".entry") == 0) {
        return IS_ENTRY;
    }

    if(strcmp(word, ".extern") == 0) {
        return IS_EXTERN;
    }

    for (i = 0; i < TOTAL_COMMANDS; i++) {
        if(strcmp(word, commands[i]) == 0) {
            return IS_COMMAND;
        }
    }

    if(strcmp(word,".string")==0) {
        return IS_STRING;
    }

    if(strcmp(word,".data")==0) {
        return IS_DATA;
    }

    return UNDEFINED_INSTRUCTION;
}

void add_to_instruction_list(MACHINE_CODE_INSTRUCTION **instruction_list , int address , unsigned short mila) {

    MACHINE_CODE_INSTRUCTION *ptr = NULL;
    MACHINE_CODE_INSTRUCTION *new_node = my_malloc(sizeof(MACHINE_CODE_INSTRUCTION));

    /* Initialize the new node */
    new_node->addr = address;
    new_node->mila = mila;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*instruction_list == NULL) {
        *instruction_list = new_node;
    } else {
        /* Traverse the list to find the last node */
        ptr = *instruction_list;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }


}

int convert_string(MACHINE_CODE_INSTRUCTION **instruction_list, char *word, int *address) {

    int i;

    /* Check if the string starts and ends with " */
    if (word[0] != '"') {
        return 0;
    }
    if (word[strlen(word)-1] != '"') {
        return 0;
    }

    /* Iterate over the characters in the string, excluding the surrounding quotes */
    for (i = 1; i < strlen(word)-1; i++) {
        add_to_instruction_list(instruction_list , *address , word[i]);/* Add character as instruction */
        (*address)++;
    }
    /* Add a terminating null character (MILA = 0) at the end of the string */

    add_to_instruction_list(instruction_list , *address , 0);
    (*address)++;
    return 1;
}
int examin_label(char *word,int line_count) {
    short result = 1,endchecks=0;
    int i = 0,len;
    len = strlen(word);
    if(word[0] == '\0') {
        printf("missing label name error in line %d",line_count);
        result= 0;
        endchecks=1;
    }
    /* Check if the macro name exceeds the maximum allowed length */
    if(!endchecks&&strlen(word) >= MAXLABLE){
        printf("label name exceeds the maximum label length error in line %d" , line_count);
        result= 0;
        endchecks=1;
    }
    /* Check if the macro name starts with a digit */
    if(!endchecks&&isdigit(word[0])){
        printf("label name starts with a digit error in line %d" , line_count);
        result = 0;
        endchecks=1;
    }
    /* Loop through the characters of the macro name to validate them */
    while(i < len){
        if( isupper(word[i]) == 0 && islower(word[i]) == 0 && isdigit(word[i]) == 0 ){
            printf("invalid character in label name error in line %d " , line_count);

            result = 0;
            endchecks=1;
        }
        i++;
    }

    /* Check if the macro name is a reserved word */
    if(!endchecks&&!reservedWordCheck(word)) {
        printf("label name is reserved word error in line %d" , line_count);
        result= 0;
        endchecks=1;
    }


    return  result;

}

int compare_label_with_other_lists(char *label, ASSEMBLER_TABLE *assembler)
{
    int result = 1;

    /* Check if the label is already defined in the macro list */
    while (result && assembler->macro_head != NULL)
    {
        if (strcmp(label, assembler->macro_head->macro_name) == 0)
        {
            printf("%s %s\n",label, "LABEL_ALREADY_DEFINED");
            result = 0;
        }
        assembler->macro_head = assembler->macro_head->next;
    }

    /* Check if the label is already defined in the label list */
    while (result && assembler->label_head != NULL)
    {
        if (strcmp(label, (assembler)->label_head->label) == 0)
        {
            printf("%s %s\n",label, "LABEL_ALREADY_DEFINED");
            result = 0;
        }
        assembler->label_head = assembler->label_head->next;
    }

    /* Check if the label is already defined in the extern list */
    while (result && assembler->extern_head != NULL)
    {
        if (strcmp(label, (assembler)->extern_head->label) == 0)
        {
            printf("%s %s\n",label, "LABEL_ALREADY_DEFINED");
            result = 0;
        }
        assembler->extern_head = assembler->extern_head->next;
    }



    return result;
}