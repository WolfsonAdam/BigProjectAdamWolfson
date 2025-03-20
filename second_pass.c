#include "asmbler.h"


int label_exist(char *label, ASSEMBLER_TABLE *assembler) {
    int result = 0;

    /* Check if the label is already defined in the label list */
    while (!result && assembler->label_head != NULL)
    {
        if (strcmp(label, (assembler)->label_head->label) == 0)
        {

            result = 1;
        }
        assembler->label_head = assembler->label_head->next;
    }

    /* Check if the label is already defined in the extern list */
    while (!result && assembler->extern_head != NULL)
    {
        if (strcmp(label, (assembler)->extern_head->label) == 0)
        {

            result = 1;
        }
        assembler->extern_head = assembler->extern_head->next;
    }

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
            /* Verify if the label exists in either the label or extern list */
            error_flag += !label_exist(ptr_mach_code->label,*assembler_table);

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


    return  error_flag;
}