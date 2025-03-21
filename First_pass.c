
#include "asmbler.h"
void int_to_mila(int number ,mila * mila) {
    int  number_24bit = number & ON_24_BIT; /* Mask to keep only 24 bits */
    number_24bit=number_24bit - (number_24bit >> 23 << 24);/* Mask to keep the sign of the number*/
    number_24bit=number_24bit& ON_24_BIT;
    mila->binary=number_24bit;
}
void insert_address_list(ADDRESS_Node **address_list, int address){
    ADDRESS_Node *ptr;
    ADDRESS_Node *new_node ;

    ptr = NULL;
    new_node = my_malloc(sizeof(ADDRESS_Node));

    new_node->adr = address;
    new_node->next = NULL;


    /* If the list is empty, set the new node as the head */
    if (*address_list == NULL)
    {
        *address_list = new_node;

    } else
    {
        /* Traverse the list to find the last node */
        ptr = *address_list;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }

}

void insert_Label_List(ASSEMBLER_TABLE **assembler , char *label, int address) {
    LABEL_LIST *ptr;
    LABEL_LIST *new_node ;

    ptr = NULL;
    new_node = my_malloc(sizeof(LABEL_LIST));
    new_node->addr_list=my_malloc(sizeof(ADDRESS_Node));
    /* Initialize the new label node */
    memset(new_node->label, '\0', sizeof(new_node->label));
    strcpy(new_node->label, label);
    
    new_node->addr_list->adr = address;
    new_node->addr_list->next = NULL;
    new_node->next = NULL;
    printf("current lable is %s\n",label);
    /* If the list is empty, set the new node as the head */
    if ((*assembler)->label_head == NULL)
    {   
        
        printf("list empty \n");
        (*assembler)->label_head = new_node;
       

    } else
    {
        printf("not empty \n");
        /* Traverse the list to find the last node */
        ptr = (*assembler)->label_head;
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

int first_pass_line_type(char *word, char *commands[],int* command) {
    
    int i;
    if(word[0]=='\0'){
        return EMPTY_LINE;
    }
    if(strcmp(word, ".entry") == 0) {
        return IS_ENTRY;
    }

    if(strcmp(word, ".extern") == 0) {
        return IS_EXTERN;
    }

    for (i = 0; i < TOTAL_COMMANDS; i++) {
        if(strcmp(word, commands[i]) == 0) {
            *command=i; /* matches the value in Command_Codes*/
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

void add_to_instruction_list(MACHINE_CODE_INSTRUCTION **instruction_list , int address , mila mila) {

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
    mila current;
    
    /* Check if the string starts and ends with " */
    
    if (word[0] != '"') {
        
        return 0;
    }
    if (word[strlen(word)-1] != '"') {
        
        return 0;
    }

    /* Iterate over the characters in the string, excluding the surrounding quotes */
    for (i = 1; i < strlen(word)-1; i++) {
        int_to_mila(word[i], &current);
        add_to_instruction_list(instruction_list , *address , current);/* Add character as instruction */
        (*address)++;
        
    }
    /* Add a terminating null character (MILA = 0) at the end of the string */
    int_to_mila(0, &current);
    add_to_instruction_list(instruction_list , *address , current);
    
    (*address)++;
    return 1;
}
int examin_label(char *word,int line_count) {
    
    short result = 1,endchecks=0;
    int i = 0,len;
    len = strlen(word);
    
    if(word[len-1] == ':') {
        len=len-1;
    }
    if(word[0] == ':') {
        printf("missing label name error in line %d\n",line_count);
        result= 0;
        endchecks=1;
    }
    /* Check if the label name exceeds the maximum allowed length */
    if(!endchecks&&strlen(word) >= MAXLABLE){
        printf("label name exceeds the maximum label length error in line %d\n" , line_count);
        result= 0;
        endchecks=1;
    }
    /* Check if the label name starts with a digit */
    if(!endchecks&&isdigit(word[0])){
        printf("label name starts with a digit error in line %d\n" , line_count);
        result = 0;
        endchecks=1;
    }
    /* Loop through the characters of the macro name to validate them */
    while(i < len){
        if( isupper(word[i]) == 0 && islower(word[i]) == 0 && isdigit(word[i]) == 0 ){
            printf("invalid character in label name error in line %d\n" , line_count);

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
    LABEL_LIST * ptr_label = (assembler)->label_head;
    EXTERN_LIST * ptr_extern = (assembler)->extern_head;
    
    MACRO_LIST_NODE * ptr_macro = (assembler)->macro_head;
    /* Check if the label is already defined in the macro list */
    while (result && ptr_macro != NULL)
    {
        if (strcmp(label, ptr_macro->macro_name) == 0)
        {
            printf("%s %s\n",label, "LABEL_ALREADY_DEFINED");
            result = 0;
        }
        ptr_macro = ptr_macro->next;
    }

    /* Check if the label is already defined in the label list */
    while (result && ptr_label != NULL)
    {
        if (strcmp(label, ptr_label->label) == 0)
        {
            printf("%s %s\n",label, "LABEL_ALREADY_DEFINED");
            result = 0;
        }
        ptr_label = ptr_label->next;
    }

    /* Check if the label is already defined in the extern list */
    while (result && ptr_extern != NULL)
    {
        if (strcmp(label, ptr_extern->label) == 0)
        {
            printf("%s %s\n",label, "LABEL_ALREADY_DEFINED");
            result = 0;
        }
        ptr_extern = ptr_extern->next;
    }



    return result;
}

int operand_Type_Identifier( char **registers, char operand[MAXLABLE]){
    int i;
    int result = LABEL;
    

    /* Check if the first character is legal (must be a digit, letter, '&' for ADDRESS, or '#' for numbers) */
    if( !isdigit(operand[0]) && !isupper(operand[0])  && !islower(operand[0]) && operand[0] != '&' && operand[0] != '#'){
        return NO_OP;/* Return NO_OP for invalid operands */
    }
    /* Determine the operand type based on the first character */
    switch (operand[0]){
        /* Case for register operands  */
        case 'r':
            for(i = 0 ; i < TOTAL_REGISTERS ; i++){
                if( ( strcmp(registers[i], operand) ) == 0 ){
                    result = REGISTER; /* Register operand */
                }
            }

        break;
        /* Case for adress operands ( "&{lablenamehere}") */
        case '&':
            result = ADDRESS;
        break;

        /* Case for number operands ( "#5") */
        case '#':
            result = NUMBER; /* Number operand */
        break;

        /* Default case for label operands */
        default:
            result = LABEL;/* Label operand */
    }
    return result;/* Return the identified operand type */
}



int Entry_Examine(char * word , int line_counter , ASSEMBLER_TABLE *assembler , char **registers){

    int type;
    EXTERN_LIST * temp;
    int result = 1;


    /* Identify the type of the operand (it should be a label) */
    type = operand_Type_Identifier(registers, word);
    result = examin_label(word, line_counter);
    /* Ensure the operand is a valid label */
    if(type != LABEL || !result )
    {
        printf("operand is not a valid label error in line %d", line_counter);
        result = 0;
    }

    temp  = assembler->extern_head;
    if(result == 1)
    {
        while(temp!= NULL)
        {
            if( ( strcmp(word, temp->label) ) == 0)
            {
                /* Check if the label is already defined in the `.extern` list */
                printf("ALREADY_DEFINED_AS_EXTERN error in line %d", line_counter);
                result = 0;
            }
            temp = temp->next;
        }
    }

    return result;
}

int Extern_Examine(char * word , int line_counter , ASSEMBLER_TABLE *assembler , char **registers){

    int type;
    EXTERN_LIST * temp;
    int result = 1;


    /* Identify the type of the operand (it should be a label) */
    type = operand_Type_Identifier(registers, word);
    result = examin_label(word, line_counter);
    /* Ensure the operand is a valid label */
    if(type != LABEL || !result )
    {
        printf("operand is not a valid label error in line %d", line_counter);
        result = 0;
    }

    temp  = assembler->entry_head;
    if(result == 1)
    {
        while(temp!= NULL)
        {
            if( ( strcmp(word, temp->label) ) == 0)
            {
                /* Check if the label is already defined in the `.extern` list */
                printf("ALREADY_DEFINED_AS_EXTERN error in line %d", line_counter);
                result = 0;
            }
            temp = temp->next;
        }
    }

    return result;
}

int number_Examine(char * word,int line_counter) {
    long number;
    int i=0,result = 1;
    
    if(word[0]== '+' || word[0] == '-') {
        
        i=1;
    }
    for(; i <strlen(word); i++) {
        if(!isdigit(word[i])) {
            
            result=0;
            printf("%s is not a valid number error in line %d\n", word, line_counter);
            return result;
        }

    }
    number = atol(word);
    if(number > MAX_NUMBER_24BIT || number < MIN_NUMBER_24BIT) {
        printf("out of bounds error in line %d\n", line_counter);
        result=0;
    }
    return result;
}

int extract_Data(MACHINE_CODE_INSTRUCTION **instruction_list, char *line, int *address, int line_counter) {
    int skip ,result;
    char number[MAXLABLE],comma[MAXLABLE];
    mila mila;

    result=1;
    skip=get_word(line,number);
    while (number[0] != '\0') {
        if(!number_Examine(number,line_counter) ) {
            result=0;
            break;
        }

        int_to_mila(atoi(number),&mila);
        add_to_instruction_list(instruction_list , *address , mila);
        
        skip+=get_word(line+skip,comma);
        (*address)++;
        if(comma[0] != ',' && comma[0]!='\0') {
            result=0;
            printf(" missing comma error error in line %d\n", line_counter);
            break;
        }
        skip+=get_word(line+skip,number);
    }
    return result;
}

void insert_Command_List(MACHINE_CODE_COMMAND **command_list , int addr, char label[MAXLABLE] , mila binary_code , int type)
{
    MACHINE_CODE_COMMAND * ptr = NULL;
    MACHINE_CODE_COMMAND * new_node = my_malloc(sizeof(MACHINE_CODE_COMMAND) );
    new_node->addr = addr;
    memset(new_node->label , '\0' ,sizeof(new_node->label));
    strcpy(new_node->label , label);
    new_node->type = type;
    new_node->binary_code = binary_code;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if(*command_list == NULL){
        *command_list = new_node;
        return;
    }
    /* Traverse the list to find the last node */
    ptr = *command_list;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    /* Append the new node to the end of the list */
    ptr->next = new_node;

}



void Mila(MACHINE_CODE_COMMAND **command_list, int command_code, char *src_op, int type_src, char *dest_op, int type_dest,int funct, int *IC) {
    int oprand_value;
    char lable[MAXLABLE];
    mila new_mila;
    oprand_value=0;
    lable[0]='\0';
    new_mila.binary=0;
    new_mila.binary|=A; /*all commands have A bit as 1 and R and E bits as 0 according to instructions*/
    new_mila.binary|=(command_code<<OPCODE); /* fills the op code field in the biary code  */
    new_mila.binary|=(funct<<FUNCT);/* fills the funct code field in the biary code  */
    
    /* ZERO OPRAND COMMANDS */
    if(command_code==RTS||command_code==STOP) {

        insert_Command_List(command_list, *IC, lable, new_mila, type_src);
        (*IC)++;
        

    }
    /* ONE OPRAND COMMANDS */
    if(command_code==CLR||command_code==NOT||command_code==INC||command_code==DEC||command_code==JMP||command_code==BNE||command_code==JSR||command_code==RED||command_code==PRN) {

        new_mila.binary|=(type_dest<<MIONDST);
        /* if oprand is register */
        if(type_dest == REGISTER) {
            /* skips the r and gets the registers number */
            oprand_value=atoi(dest_op+1);
            new_mila.binary|=(oprand_value<<RGDST);

        }
        /* adds the command binarry code to the list */
        insert_Command_List(command_list, *IC, lable, new_mila, type_src);
        (*IC)++;
        
        /* if number add the number in the next binary code  if lable or addres add them in second pass cause might not be defined yet */
       if(type_dest == NUMBER) {

           new_mila.binary=A;
           oprand_value=atoi(dest_op+1);
           new_mila.binary|=(oprand_value<<FUNCT); /* diract number starts right after ARE in the mila */
       }else {
           /* handles addres or label */
           strcpy(lable,dest_op);
           new_mila.binary=SECONDPASS;
       }
        /* if register the command is already complete */
        if(type_dest != REGISTER) {
            insert_Command_List(command_list, *IC, lable, new_mila, type_dest);
            (*IC)++;
        }
    }
    /* TWO OPRAND COMMANDS */
    if(command_code==MOV||command_code==CMP||command_code==ADD||command_code==SUB||command_code==LEA) {
        new_mila.binary|=(type_dest<<MIONDST);
        new_mila.binary|=(type_src<<MIONSRC);
        
        /* if oprand is register */
        if(type_src == REGISTER) {
            /* skips the r and gets the registers number */
            oprand_value=atoi(src_op+1);
            new_mila.binary|=(oprand_value<<RGSRC);

        }
        /* if oprand is register */
        if(type_dest == REGISTER) {
            /* skips the r and gets the registers number */
            oprand_value=atoi(dest_op+1);
            new_mila.binary|=(oprand_value<<RGDST);

        }
        insert_Command_List(command_list, *IC, lable, new_mila, type_dest);
        (*IC)++;
        

        if(type_src == NUMBER) {

            new_mila.binary=A;
            oprand_value=atoi(src_op+1);
            new_mila.binary|=(oprand_value<<FUNCT); /* diract number starts right after ARE in the mila */
        }else {
            /* handles  label */
            strcpy(lable,src_op);
            new_mila.binary=SECONDPASS;
        }
        /* if register the command is already complete */
        if(type_src != REGISTER) {
            insert_Command_List(command_list, *IC, lable, new_mila, type_src);
            (*IC)++;
            
        }

    
        if(type_dest == NUMBER) {

            new_mila.binary=A;
            oprand_value=atoi(dest_op+1);
            new_mila.binary|=(oprand_value<<FUNCT); /* diract number starts right after ARE in the mila */
        }else {
            /* handles  label */
            strcpy(lable,dest_op);
            new_mila.binary=SECONDPASS;
        }
        /* if register the command is already complete */
        if(type_dest != REGISTER) {
            insert_Command_List(command_list, *IC, lable, new_mila, type_dest);
            (*IC)++;
            
        }

    }
    

}

int examine_oprand(char * oprand ,int oprand_type,int line_counter) {
    int result=1;
    switch(oprand_type) {
        case NUMBER:
            /* skips the # at the start of a number */
            result=number_Examine(oprand+1,line_counter);
            break;
        case ADDRESS:
            /* skips the & in the lable name */
            oprand+=1;
            result=examin_label(oprand,line_counter);

        case LABEL:
            result=examin_label(oprand,line_counter);
            break;
        case NO_OP:
            result=0;
    }
    return result;
}

int command_check(int command_code ,char * line,int line_counter, char **registers) {
    char word[MAXLABLE], dest[MAXLABLE], src[MAXLABLE];
    int dest_type,src_type,result=1,skip=0,noExtraTXT;
    
    if(command_code==RTS||command_code==STOP) {

        /* check for empty line in the end */
    }
    /* 1 OPRAND COMMANDS */
    if(command_code==CLR||command_code==NOT||command_code==INC||command_code==DEC||command_code==JMP||command_code==BNE||command_code==JSR||command_code==RED||command_code==PRN) {
        skip+=get_word(line,dest);
        dest_type=operand_Type_Identifier(registers,dest);
        result=examine_oprand(dest,dest_type,line_counter);

        switch(command_code) {
            case CLR:
            case NOT:
            case INC:
            case DEC:
            case RED:
                if(dest_type!=REGISTER && dest_type!=LABEL) {
                    /* only accept registers and labels */
                    result=0;
                    printf("invalid oprand  error in line  %d\n",line_counter);
                }
                break;
            case JMP:
            case BNE:
            case JSR:
                if(dest_type!=ADDRESS && dest_type!=LABEL) {
                    /* jmp bne and jsr only accept address and lables */
                    result=0;
                    printf("invalid oprand  error in line  %d\n",line_counter);
                }
                break;
            case PRN:
                if(dest_type==ADDRESS) {
                    result=0;
                    printf("invalid oprand  error in line  %d\n",line_counter);
                }
        }

    }
    /* 2 OPRAND COMMANDS */
    if(command_code==MOV||command_code==CMP||command_code==ADD||command_code==SUB||command_code==LEA) {
        skip+=get_word(line,src);
        src_type=operand_Type_Identifier(registers,src);
        
        /* checks the validity of the src oprand */
        if(src_type==ADDRESS) {
            result=0;
            printf("invalid oprand  error in line  %d\n",line_counter);
        }else if(command_code==LEA) {
            if(src_type!=LABEL) {
                result=0;
                printf("invalid oprand  error in line  %d\n",line_counter);
            }
        }
        skip+=get_word(line+skip,word);
        
        if(word[0]!=','&&strlen(word)!=1) {
            result=0;
            printf("missing comma  error in line  %d\n",line_counter);
            return result;
        }
        skip+=get_word(line+skip,dest);
        dest_type=operand_Type_Identifier(registers,dest);
        result=examine_oprand(dest,dest_type,line_counter)&examine_oprand(src,src_type,line_counter);
        /* checks the validity of the dst oprand */
        if(dest_type==ADDRESS) {
            result=0;
            printf("invalid oprand  error in line  %d\n",line_counter);
        }else if(dest_type==NUMBER &&command_code!=CMP) {
            result=0;
            printf("invalid oprand  error in line  %d\n",line_counter);
        }

    }
    get_word(line+skip,word);
    /* true if line is empty after the oprands as it should false if line isnt empty + prints error msg*/
    noExtraTXT = word[0]=='\0';
    if(!noExtraTXT) {
        printf("extra text error in line   %d\n",line_counter);
    }
    return result&noExtraTXT;
}

int commandCodeToOpCode(int command_code,int *opCode,int *functCode) {
    int result=1;
    *opCode=0;
    *functCode=0;
    switch(command_code) {
        case MOV:
            *opCode=0;
            *functCode=0;
            break;
        case CMP:
            *opCode=1;
            *functCode=0;
            break;
        case SUB:
            *functCode+=1;
        case ADD:
            *opCode=2;
            *functCode+=1;
            break;
        case LEA:
            *opCode=4;
            break;
        case DEC:
            *functCode+=1;
        case INC:
            *functCode+=1;
        case NOT:
            *functCode+=1;
        case CLR:
            *functCode+=1;
            *opCode=5;
            break;
        case JSR:
            *functCode+=1;
        case BNE:
            *functCode+=1;
        case JMP:
            *functCode+=1;
            *opCode=9;
            break;
        case RED:
            *opCode=12;
            break;
        case PRN:
            *opCode=13;
        case RTS:
            *opCode=14;
            break;
        case STOP:
            *opCode=15;
            break;
        default:
            result=0;
            break;
    }

    return result;
}

int convert_Command(MACHINE_CODE_COMMAND **command_list,  char **registers, char *line, int command_code, int *IC , int line_counter)
{
    int result=1,skip=0,opCode,functCode;
    char src_op[MAXLABLE], dest_op[MAXLABLE],comma[MAXLABLE];
    int type_src = NO_OP , type_dest = NO_OP;

    memset(src_op , '\0' , MAXLABLE);
    memset(dest_op , '\0' , MAXLABLE);
    result=command_check(command_code,line,line_counter,registers);
    /* command check found an error */
    if(!result) {
        return 0;
    }
    commandCodeToOpCode(command_code,&opCode,&functCode);
    /* Handle different command types based on the command code */
    switch(command_code)
    {
        /* Two operand commands (e.g., MOV, CMP, ADD, SUB, LEA) */
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
            /* Extract the source operand (before the comma) */
            skip+=get_word(line, src_op);
            /* Extract the destination operand (after the comma) */
            skip+=get_word(line + skip, comma);
            skip+=get_word (line + skip , dest_op);
            /* Identify the types of the source and destination operands */
            type_src =  operand_Type_Identifier(registers, src_op);
            type_dest =  operand_Type_Identifier(registers, dest_op);
            /* Generate the machine code (MILA) for the command and operands */
            Mila(command_list, command_code, src_op, type_src, dest_op, type_dest, functCode,IC);
            break;

         /* Single operand commands (e.g., CLR, NOT, INC, JMP, etc.) */
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
        case PRN:
        case JSR:
            /* Extract the destination operand */
            skip+=get_word (line + skip , dest_op);
            /* Identify the type of the destination operand */
            type_dest =  operand_Type_Identifier(registers, dest_op);
            /* Generate the machine code (MILA) for the command and destination operand */
            Mila(command_list, command_code, NULL, 0, dest_op, type_dest, functCode,IC);

            break;

        /* Commands with no operands (e.g., RTS, STOP) */
        case RTS:
        case STOP:
            /* Generate the machine code (MILA) for commands with no operands */
            Mila(command_list, command_code, NULL, NO_OP, NULL, NO_OP, functCode,IC);

    }


    return result;
}


int firstpass(ASSEMBLER_TABLE **assembler, char *file_name ,int* IC ,int* DC) {
    FILE *fptr = NULL;
    char line[MAXLINE], label[MAXLABLE], word[MAXLABLE],oprand[MAXLABLE];
    int  skip = 0, line_counter = 1,label_skip = 0 , type = UNDEFINED_INSTRUCTION, command=0;
    int error_flag = 0 , error = 0 ;/* Flags to track errors during the first pass */

    /* Register names for operand identification */
     char *registers[] =
    {
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"
    };
     char *commands[TOTAL_COMMANDS]={
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

    *IC = 100, *DC = 1;
    memset(label, '\0', sizeof(label));
    memset(line, '\0', sizeof(line));

    /* Open the am file */
    fptr = fopen(file_name, "r");
    if(fptr == NULL)
    {
        printf("File opening error couldnt open am file \n");
        exit(1);
    }

    /* Process each line of the am file */
    while( fgets(line , MAXLINE , fptr) != NULL)
    {
        /* Extract the label (if any) from the line */

        skip=get_word(line, word);
        if(word[skip-1] == ':')
        {
            error = !examin_label(label, line_counter);/* Check the validity of the label */
            error +=!compare_label_with_other_lists(label,*assembler) ;
            if(error ){
                type = IS_ERROR;
            }
            strcpy(label,word);
            label_skip = skip;
            skip+=get_word(line + label_skip,word); /* len of the lable + the reserved word including all the write spaces*/

        }

        /* If no label error, determine the line type (directive or command) */
        if (!error  )
        {   
            
            
            type = first_pass_line_type(word, commands,&command);
        }

        /* Process the line based on its type (command, directive, etc.) */
            switch(type)
            {
                case IS_ENTRY:

                    if (label_skip )
                    {

                        /* Add the label to the label list with IC + DC */
                        /* reread the instruction they arent 100% clear so im not sure if to add this
                        insert_Label_List(&((*assembler)->label_head), label, IC + DC );
                        */
                    }
                /* Examine the .entry directive for errors */
                get_word(line +skip,oprand);
                error = !Entry_Examine(oprand, line_counter , *assembler , registers);
                if(!error ){
                    /* Add entry to the entry list */
                    insert_Entry_List(&((*assembler)->entry_head), label);
                }
                break;


                case IS_EXTERN:

                    if (label_skip)
                    {
                        /* Add the label to the label list with IC + DC */
                        /* reread the instruction they arent 100% clear so im not sure if to add this
                        insert_Label_List(&((*assembler)->label_head), label, IC + DC );
                         */
                    }

                get_word(line + skip,oprand);
                error = !Extern_Examine(oprand, line_counter , *assembler , registers);
                if(!error ){
                    /* Add entry to the entry list */
                    insert_Extern_List(&((*assembler)->extern_head), label);
                }
                break;


                case IS_STRING:

                    if (label_skip )
                    {
                        insert_Label_List(assembler, label, *IC + *DC - 1);
                    }
                /* Examine the .string directive for errors */
                get_word(line +skip,oprand);
                
                error = !convert_string(&(*assembler)->instruction_head,oprand ,DC);

                break;

                case IS_DATA:

                    if (label_skip )
                    {
                        /* Add the label to the label list */
                        insert_Label_List(assembler, label,*IC + *DC - 1);
                    }
                /* Examine the .data directive for errors */
                error = !extract_Data(&((*assembler)->instruction_head), line + skip, DC,line_counter);

                break;

                case IS_COMMAND:
                    /* Handle assembly commands */
                        if (label_skip )
                        {
                            
                            /* Add the label to the label list with IC */
                            insert_Label_List(assembler, label, *IC );
                        }
                /* Examine the command for errors and add to command list */
                        
                   error = !convert_Command(&((*assembler)->command_head), registers, line + skip, command, IC , line_counter);

                break;

                
                
                case EMPTY_LINE:
                break;
                        

                default:
                    /* Handle undefined instructions (lines that don't match any valid directive or command) */
                        if(line[0] != '\0'){
                            printf("Line %d: undefined instruction\n", line_counter);
                        }

            }

            /* Reset variables for the next line */
            type=UNDEFINED_INSTRUCTION;
            label_skip = 0;
            error_flag += error;
            error = 0;
            skip=0;
            line_counter++;
            command = -1;
            memset(label, '\0', sizeof(label));
            memset(line, '\0', sizeof(line));
            memset(word, '\0', sizeof(word));
            memset(oprand, '\0', sizeof(oprand));
            
        }
        /* Close the file after processing */
        fclose(fptr);
        fptr = NULL;

       return error_flag;
    }
void free_ADDRESS_Node(ADDRESS_Node **head) {
    ADDRESS_Node *temp;
    /* Loop through the address list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }
}

void free_LABEL_LIST(LABEL_LIST **head){
    LABEL_LIST *temp;
    /* Loop through the label list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free_ADDRESS_Node(&temp->addr_list);
        free(temp);
        temp = NULL;
    }

}
void free_ENTRY_LIST(ENTRY_LIST **head) {
    free_LABEL_LIST(head);
}

void free_EXTERN_LIST(EXTERN_LIST **head) {
    free_LABEL_LIST(head);
}


void free_MACHINE_CODE_COMMAND(MACHINE_CODE_COMMAND **head){
    MACHINE_CODE_COMMAND *temp;
    /* Loop through the command list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}
void free_MACHINE_CODE_INSTRUCTION(MACHINE_CODE_INSTRUCTION **head){
    MACHINE_CODE_INSTRUCTION *temp;
    /* Loop through the instruction list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}

void free_assembler_table(ASSEMBLER_TABLE **table) {
    free_LABEL_LIST(&(*table)->label_head);
    free_ENTRY_LIST(&(*table)->entry_head);
    free_EXTERN_LIST(&(*table)->extern_head);
    free_MACHINE_CODE_COMMAND(&(*table)->command_head);
    free_MACHINE_CODE_INSTRUCTION(&(*table)->instruction_head);
    free_macro_list(&(*table)->macro_head);
}