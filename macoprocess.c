#include "asmbler.h"


void add_to_macro_list(MACRO_LIST_NODE ** head ,char * macro_name ,MACRO_CONTENT_NODE * head_content) {
    MACRO_LIST_NODE * ptr;
    MACRO_LIST_NODE * new_node;
    ptr = NULL;
    /* Allocate memory for a new macro node */
    new_node = my_malloc(sizeof(MACRO_LIST_NODE));
    memset(new_node->macro_name , '\0',MAXLINE);

    strcpy( new_node->macro_name , macro_name);
    new_node->head_content = head_content;
    new_node->next = NULL;

    /* If the macro list is empty, set the new node as the head */
    if(*head == NULL){

        *head = new_node;
        return;
    }
    /* Traverse the list to append the new node at the end */
    ptr = *head;
    while(ptr->next != NULL){

        ptr = ptr->next;
    }
    ptr->next = new_node;
}

void add_content_to_list(MACRO_CONTENT_NODE ** head_content , char *line_to_add){
    MACRO_CONTENT_NODE * ptr = NULL;
    /* Allocate memory for a new content node */
    MACRO_CONTENT_NODE * content_to_add = my_malloc(sizeof(MACRO_CONTENT_NODE));
    memset(content_to_add->line , '\0' , sizeof(content_to_add->line));

    strcpy(content_to_add->line , line_to_add);
    content_to_add->next = NULL;

    /* If the content list is empty, set the new node as the head */
    if( *head_content == NULL){
        *head_content = content_to_add;
        return;
    }
    /* Traverse the content list to append the new node at the end */
    ptr = *head_content;
    while( ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->next = content_to_add;
}

MACRO_LIST_NODE * find_macro(MACRO_LIST_NODE * head , char * name_to_find){

    while(head != NULL){

        if(strcmp(head->macro_name , name_to_find) == 0){

            return head;
        }
        head = head->next;
    }
    return NULL;/* Return NULL if macro not found */
}

int line_type(char * word , MACRO_LIST_NODE *head ) {

    /* Check if the line is a macro declaration */
    if(strcmp(word , "mcro" ) == 0){

        return DECLARATION;
    }
    /* Check if the line marks the end of a macro */
    if(strcmp(word , "mcroend" ) == 0){

        return DECLARATION_END;
    }
    /* Check for empty line */
    if(word[0] == '\0' || word [0]== '\n'){

        return EMPTY_LINE;
    }
    /* Check if the line calls an existing macro */
    if(find_macro(head , word) != NULL){

        return MACRO_CALL;
    }
    /* Default case if no conditions are met (ordinary line)*/
    return NOT_MACRO;

}


void free_macro_content(MACRO_CONTENT_NODE **head){
    MACRO_CONTENT_NODE *temp;
    /* Loop through each element of the list and free it */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}

void free_macro_list(MACRO_LIST_NODE **head){
    MACRO_LIST_NODE *temp;
    /* Loop through the macro list and free each node */
    while (*head != NULL) {
        /* Free the macro content associated with each macro */
        if((*head)->head_content->next != NULL){
            free_macro_content(&((*head)->head_content->next));
        }
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}



short Macro_Name_Check(char * word ,MACRO_LIST_NODE * head , int line_count) {
    short result = 1,endchecks=0;
    int i = 0,len;
    len = strlen(word);
    if(word[0] == '\0') {
        printf("missing macro name error in line %d",line_count);
        result= 0;
        endchecks=1;
    }
    /* Check if the macro name exceeds the maximum allowed length */
    if(!endchecks&&strlen(word) >= MAXLABLE){
        printf("macro name exceeds the maximum label length error in line %d" , line_count);
        result= 0;
        endchecks=1;
    }
    /* Check if the macro name starts with a digit */
    if(!endchecks&&isdigit(word[0])){
        printf("macro name starts with a digit error in line %d" , line_count);
        result = 0;
        endchecks=1;
    }
    /* Loop through the characters of the macro name to validate them */
    while(i < len){
        if( isupper(word[i]) == 0 && islower(word[i]) == 0 && isdigit(word[i]) == 0 && word[i] != '_'){
            printf("invalid character in macro name error in line %d " , line_count);

            result = 0;
            endchecks=1;
        }
        i++;
    }
    /* Check if the macro name already exists in the macro list */
    while(!endchecks && head != NULL){
        if(strcmp(head->macro_name , word) == 0){
            printf("macro name already exists in the macro list error in line %d" ,line_count);
            result= 0;
            endchecks=1;
            break;
        }
        head = head->next;
    }
    /* Check if the macro name is a reserved word */
    if(!endchecks&&!reservedWordCheck(word)) {
       printf("macro name is reserved word error in line %d" , line_count);
        result= 0;
        endchecks=1;
    }
    return  result;
}

int preProcess(MACRO_LIST_NODE ** macro_head,char *file_name,char ** file_amPtr) {
    char * file_am ,* file_as;
    char line[MAXLINE] , word[MAXLINE] , macro_name[MAXLINE];
    FILE * fptr_am , * fptr_as;
    int type = NOT_MACRO ,  line_count = 1;
    int error_flag = 1 , final_result = 1;

    MACRO_CONTENT_NODE *head_content = NULL;
    MACRO_LIST_NODE * head_macro= NULL;

    memset(line , '\0',sizeof(line));
    memset(macro_name , '\0',sizeof(macro_name));
    file_as=my_malloc(MAXLINE);
    file_am=my_malloc(MAXLINE);
    /* Add file suffixes for the input and output files */
    file_as = strcpy(file_as , file_name);
    file_as = strcat(file_as , ".as");
    file_am = strcpy(file_am , file_name);
    file_am = strcat(file_am , ".am");


    /* Open the source file for reading */
    fptr_as  = fopen(file_as , "r");

    if(fptr_as == NULL){
        printf("File opening error couldnt open as file \n");
        exit(1);
    }
    /* Open the output file for writing */
    fptr_am = fopen(file_am , "w");
    *file_amPtr=file_am;
    if(fptr_am == NULL){
        printf("File opening error couldnt open am file");
        exit(1);
    }

    /* Main loop to scan the source file line by line */
    while(fgets(line , sizeof(line) , fptr_as) != NULL){

        get_word(line , word);
        /* Determine the type of the line */

        type = line_type(word , (*macro_head) );

        switch(type){
            case DECLARATION:
                /* Check macro name validity and handle error flags */

                get_word(line+strlen(word) , macro_name);

                error_flag = Macro_Name_Check(macro_name,  *macro_head, line_count);
                if(!error_flag ){
                    final_result = 0;
                }

                memset(line , '\0',sizeof(line));
                /* Save macro content until endmacr is found */
                while(fgets(line , sizeof(line) , fptr_as) != NULL ){
                    line_count++;
                    get_word(line , word);
                    type = line_type(word , *macro_head);

                    if(type == DECLARATION_END){
                         /* Check for the validity of the macro end line */
                        get_word(line+strlen(word) , word);

                        error_flag = (word[0] == '\0');

                        if(!error_flag ){
                            final_result = 0;
                            printf("extra characters after macro end error in line %d\n" , line_count);
                        }

                        break;
                    }
                    /* Add the line to the macro is valid */
                    if(error_flag  ){
                        add_content_to_list( &head_content , line);
                    }

                    memset(line , '\0',sizeof(line));

                }
                /* Add the macro to the macro list if no errors occurred */
                if(error_flag ){
                    add_to_macro_list( macro_head , macro_name ,head_content);
                }

                break;

            case MACRO_CALL:

                /* Handle macro call by writing its content to the output file */
                head_macro = find_macro(*macro_head , word);
                if(head_macro != NULL){
                    head_content =  head_macro->head_content;
                    while(head_content != NULL){
                        fprintf(fptr_am ,"%s" ,head_content->line );
                        head_content = head_content->next;
                    }
                }
                break;

            case EMPTY_LINE:
                /* Skip empty lines */
                break;

            case NOT_MACRO:
                /* Write the ordinary line from the source to the output file */
                fprintf(fptr_am ,"%s" ,line);
                break;
        }

        line_count++;
        memset(line , '\0',sizeof(line));
        memset(macro_name , '\0',sizeof(macro_name));
        head_content = NULL;
        error_flag = 1;
    }

    line_count = 0;



    file_as = NULL;
    fprintf(fptr_am ,"%s" ,"\n");

    fclose(fptr_am);
    fclose(fptr_as);
    free(file_as);
    /* If there was an error, remove the output file */
    if(!final_result){
        if( remove(file_am) != 0 ){
            printf("File removing error");
            exit(1);
        }
    }
    

    return final_result;
}
