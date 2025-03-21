#include "asmbler.h"


int label_exist(char *label,EXTERN_LIST * extern_head,LABEL_LIST *  label_head) {
    int result = 0;

    /* Check if the label is already defined in the label list */
    while ( label_head != NULL)
    {
        printf("%s  ",label_head->label);
        if (strcmp(label,label_head->label) == 0)
        {
            
            result = 1;
        }
        label_head = label_head->next;
    }
    printf("\n");
    /* Check if the label is already defined in the extern list */
    while ( extern_head != NULL)
    {
        printf("%s  ",extern_head->label);
        if (strcmp(label,extern_head->label) == 0)
        {
            
            result = 1;
        }
       extern_head = extern_head->next;
    }
    printf("\n");
    return result;
}


int Second_Pass(ASSEMBLER_TABLE ** assembler_table , char * file_name , int IC , int DC){

    LABEL_LIST * ptr_label = (*assembler_table)->label_head;
    EXTERN_LIST * ptr_extern = (*assembler_table)->extern_head;
    ENTRY_LIST * ptr_entry = (*assembler_table)->entry_head;
    MACHINE_CODE_COMMAND * ptr_mach_code = (*assembler_table)->command_head;
    int error_flag = 0;

    /* Iterate through the machine code list and resolve labels */
    while(ptr_mach_code != NULL){
         /* Check if the mila is marked for the second pass */
        if(ptr_mach_code->binary_code.binary == SECONDPASS){
            printf("lable being checked is %s\n",ptr_mach_code->label);
            /* Verify if the label exists in either the label or extern list */
            error_flag += !label_exist(ptr_mach_code->label,(*assembler_table)->extern_head,(*assembler_table)->label_head);

            /* Check if the label is in the label list */
            while(ptr_label != NULL){
                if(strcmp(ptr_label->label ,ptr_mach_code->label) == 0){
                    ptr_mach_code->binary_code.binary = R;
                    ptr_mach_code->binary_code.binary |= (ptr_label->addr_list->adr)<<FUNCT;
                }

                ptr_label = ptr_label->next;
            }

            /* Check if the label is in the extern list */
            while(ptr_extern != NULL){
                if(strcmp(ptr_extern->label ,ptr_mach_code->label) == 0 ){
                    ptr_mach_code->binary_code.binary = E;
                    insert_address_list( &(ptr_extern->addr_list) ,ptr_mach_code->addr );
                }
                ptr_extern = ptr_extern->next;
            }

        }

        /* Reset pointers for the next iteration */
        ptr_entry = (*assembler_table)->entry_head;
        ptr_extern = (*assembler_table)->extern_head;
        ptr_label = (*assembler_table)->label_head;
        ptr_mach_code = ptr_mach_code->next;
    }

    /* Handle entry labels */
    ptr_label = (*assembler_table)->label_head;
    while(ptr_label != NULL){
        while(ptr_entry != NULL){
            if(strcmp(ptr_entry->label ,ptr_label->label) == 0 ){
                insert_address_list( &(ptr_entry->addr_list) ,ptr_label->addr_list->adr );
            }
            ptr_entry = ptr_entry->next;
        }
        /* Reset entry pointer for the next label */
        ptr_entry = (*assembler_table)->entry_head;
        ptr_label = ptr_label->next;
    }
    if(error_flag == 0) {
        makeFiles(assembler_table,file_name,IC,DC);
    }
    printf("error flag is %d",error_flag);
    return  error_flag;
}
void printFormatedLine(FILE * filePtr,unsigned int info , int adr) {
    int adrLen,infoLen,i;
    char  adr_string[7],info_string[6],tempInfo[6],tempAdr[7];
    memset(adr_string, '0', sizeof(adr_string));
    memset(info_string, '0', sizeof(info_string));
    memset(tempInfo, '\0', sizeof(tempInfo));
    memset(tempAdr, '\0', sizeof(tempAdr));
    sprintf(tempAdr, "%d", adr);
    sprintf(tempInfo, "%x", info);
    adrLen=strlen(tempAdr);
    infoLen=strlen(tempInfo);

    for(i=0;i<adrLen;i++) {
        adr_string[strlen(adr_string)-adrLen+i] = tempAdr[i];
    }

    for(i=0;i<infoLen;i++) {
        info_string[strlen(info_string)-infoLen+i] = tempInfo[i];
    }
    fprintf(filePtr,"%s %s",adr_string,info_string);
}
void makeObjectFile(ASSEMBLER_TABLE ** assembler_table , char *file_name , int IC , int DC) {
    MACHINE_CODE_COMMAND * ptr_mach_code = (*assembler_table)->command_head;
    MACHINE_CODE_INSTRUCTION * ptr_machInstruction_code = (*assembler_table)->instruction_head;
    FILE * ob_ptr;
    file_name = strcat(file_name , ".ob");
    /* Open the object file for writing */
    ob_ptr = fopen(file_name , "w");
    /* Check if the file opened successfully */

    if(ob_ptr == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    /* print number of commands and data */
    fprintf(ob_ptr,"\n\t%d %d\n" , IC - 100 , DC - 1);

    /* Write the commands to the file in octal format */
    while(ptr_mach_code != NULL){
        printFormatedLine(ob_ptr,ptr_mach_code->binary_code.binary , ptr_mach_code->addr );
        ptr_mach_code = ptr_mach_code->next;
    }
    /* Write the instructions to the file in octal format */
    while(ptr_machInstruction_code != NULL){
        printFormatedLine(ob_ptr,ptr_machInstruction_code->mila.binary , ptr_machInstruction_code->addr + IC - 1  );
        ptr_machInstruction_code = ptr_machInstruction_code->next;
    }
    /* Close the object file */
    fclose(ob_ptr);
    /* Free the memory for the object file name */
    free(ob_ptr);
}

void makeEntryFile(ASSEMBLER_TABLE ** assembler_table , char *file_name) {
    int fileEmpty;
    FILE * ob_ptr;
    ENTRY_LIST * ptr_entries = (*assembler_table)->entry_head;
    ADDRESS_Node * ptr_adr = NULL;
    char  string[MAXLABLE];
    memset(string, '\0', sizeof(string));
    file_name = strcat(file_name , ".ent");
    /* Open the entry file for writing */
    ob_ptr = fopen(file_name , "w");
    /* Check if the file opened successfully */

    if(ob_ptr == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fileEmpty=1;
    while(ptr_entries != NULL) {
        ptr_adr=ptr_entries->addr_list;
        while(ptr_adr != NULL) {
            sprintf(string, "%s \t %d\n", ptr_entries->label,ptr_adr->adr);
            fprintf(ob_ptr,"%s", string);
            fileEmpty=0;

            memset(string, '\0', sizeof(string));
            ptr_adr = ptr_adr->next;
        }
        ptr_entries = ptr_entries->next;
    }
    fclose(ob_ptr);

    if(fileEmpty ) {
        if(remove(file_name) != 0) {
            printf("Error removing file\n");
            exit(1);
        }
    }
    free(file_name);
}

void makeExternFile(ASSEMBLER_TABLE ** assembler_table , char *file_name) {
    int fileEmpty;
    FILE * ob_ptr;
    EXTERN_LIST * ptr_externs = (*assembler_table)->extern_head;
    ADDRESS_Node * ptr_adr = NULL;
    char  string[MAXLABLE];
    memset(string, '\0', sizeof(string));
    file_name = strcat(file_name , ".ext");
    /* Open the entry file for writing */
    ob_ptr = fopen(file_name , "w");
    /* Check if the file opened successfully */

    if(ob_ptr == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fileEmpty=1;
    while(ptr_externs != NULL) {
        ptr_adr=ptr_externs->addr_list;
        while(ptr_adr != NULL) {
            sprintf(string, "%s \t %d\n", ptr_externs->label,ptr_adr->adr);
            fprintf(ob_ptr,"%s", string);
            fileEmpty=0;

            memset(string, '\0', sizeof(string));
            ptr_adr = ptr_adr->next;
        }
        ptr_externs = ptr_externs->next;
    }
    fclose(ob_ptr);

    if(fileEmpty ) {
        if(remove(file_name) != 0) {
            printf("Error removing file\n");
            exit(1);
        }
    }
    free(file_name);
}
void makeFiles(ASSEMBLER_TABLE ** assembler , char *file_name , int IC , int DC) {
    makeObjectFile(assembler , file_name ,IC ,DC);
    makeEntryFile(assembler , file_name );
    makeExternFile(assembler , file_name );


}