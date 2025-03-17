#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef ASMBLER_H
#define ASMBLER_H
/* start of general utill for the whole project */
#define MAXLINE 81 /* maximun size on a line including the \n */
#define MAXLABLE 31 /*maximun size on a lable */
#define TOTAL_COMMANDS 16 /* number of commands*/
#define TOTAL_REGISTERS 8 /* number of registers*/
#define MAX_ASCII_VALUE 127
#define MIN_NUMBER_24BIT -8388608
#define MAX_NUMBER_24BIT  8388607
#define ON_24_BIT 0xffffff
#define ON_8_BIT 0xff
#define A 4
#define R 2
#define E 1
#define FUNCT 3
#define OPCODE 18
#define MIONSRC 16
#define MIONDST 11
#define RGSRC 13
#define RGDST 8
#define SECONDPASS -1
/*

  This function allocates memory of the specified size and checks if the allocation
  was successful. If memory allocation fails, an error message is printed and the
  program exits.

  size The size of the memory to allocate (in bytes).
  returns Pointer to the allocated memory.
 */
void * my_malloc(long size);

/*
 This function gets the next word in a given line.

 line is the line of text to search in.
 word is the value that's going to get returned.
 changed at 14/1/2025 while writing first pass added return. needed it for
 return returns the length we traveled on the line
*/
int get_word(char * line,char * word);
/*

 This function checks whether a given label is a reserved word in the assembler.
 It checks against command names, register names, and assembler directives. If

 param word The label to check.
 return 1 if the label is valid, 0 if it is reserved.
 */
short reservedWordCheck(char * word);
/* end of general utill for the whole project */







/* starts of macro prossing functions defines and structs  */
enum macro_line_types {
    NOT_MACRO=0,
    DECLARATION,
    DECLARATION_END,
    EMPTY_LINE,
    MACRO_CALL
  };
/* represents the content of a macro */
typedef struct MACRO_CONTENT_NODE {
    char line[MAXLINE]; /* Line of macro content */
    struct MACRO_CONTENT_NODE * next; /* Pointer to the next line */
} MACRO_CONTENT_NODE;
/*a list of marcos name + content  */
typedef struct MACRO_LIST_NODE {
    char  macro_name [MAXLINE]; /* Name of the macro */
    MACRO_CONTENT_NODE * head_content; /* Head of macro content */
    struct MACRO_LIST_NODE * next; /* Pointer to the next macro */
} MACRO_LIST_NODE;

/*
  This function searches for a macro in the list with the provided name.
  If found, it returns the macro; otherwise, it returns NULL.

  head Pointer to the head of the macro list.
  name_to_find The name of the macro to search for.
  return Pointer to the found macro or NULL if not found.
 */
MACRO_LIST_NODE * find_macro(MACRO_LIST_NODE * head , char * name_to_find);

/*
  This function allocates memory for a new macro node and appends it to the end
  of the macro list. The macro contains a name and content linked list.

 head Pointer to the head of the macro list.
 macro_name Name of the macro to be added.
 head_content Pointer to the content of the macro.
 */
void add_to_macro_list(MACRO_LIST_NODE ** head ,char * macro_name ,MACRO_CONTENT_NODE * head_content);

/*
 This function allocates memory for a new content node and appends a line of content to the end of the macro content linked list.

 head_content Pointer to the head of the content list.
 line_to_add Line of text to be added to the content list.
 */
void add_content_to_list(MACRO_CONTENT_NODE ** head_content , char *line_to_add);

/*
 This function categorizes the line into one of the following types:
 NOT_MACRO,
 DECLARATION,
 DECLARATION_END,
 EMPTY_LINE,
 MACRO_CALL

 line The original line of text.
 head Pointer to the macro list head.
 return The type of the line as an integer value.
 */
int line_type(char * line , MACRO_LIST_NODE *head );

/*
 This function frees all the memory used by the MACRO_CONTENT linked list.

 head Pointer to the head of the MACRO_CONTENT list.
 */
void free_macro_content(MACRO_CONTENT_NODE **head);

/*
 This function frees all the memory used by the MACRO_LIST linked list. It also
 frees the associated MACRO_CONTENT_Node for each macro.

 head Pointer to the head of the MACRO_LIST.
 */
void free_macro_list(MACRO_LIST_NODE **head);


/*
 * This function validates a macro name by checking its length,
 * making sure it doesn't start with a digit,
 * and confirming it contains only alphanumeric characters or underscores.
 * It also checks if the macro name is already defined or if it's a reserved word.
 *
 word Pointer to the macro name.
 head Pointer to the head of the macro list to check for duplicate names.
 line_count The current line number in the source file.
 * @return short indicating whether the macro name is valid 1 or not 0.
 */
short Macro_Name_Check(char * word ,MACRO_LIST_NODE * head , int line_count);


/*
 This function reads an assembly source file, processes macros, and writes the
 processed content to a new file with ".am" suffix. It also handles macro
 declaration and calls.

 macro_head Pointer to the  macros list .
 file_name Name of the source file to process.
 file_am file to write into.
 */
int preProcess(MACRO_LIST_NODE ** macro_head,char *file_name,char** file_am);


/* end of macro prossing functions defines and structs  */

/* start of FIRST PASS and SECOND PASS functions defines and structs  */

/* Types of instructions in FIRST PASS  */
enum first_pass_line_types {
 IS_ENTRY=100,
 IS_EXTERN,
 IS_STRING,
 IS_DATA,
 IS_COMMAND,
 UNDEFINED_INSTRUCTION,
 IS_ERROR
};
typedef enum Command_Codes{
 MOV=0,
 CMP,
 ADD,
 SUB,
 LEA,

 CLR,
 NOT,
 INC,
 DEC,
 JMP,
 BNE,
 JSR,
 RED,
 PRN,

 RTS,
 STOP

}Command_Code;

enum operand_type {
 NUMBER,
 LABEL,
 ADDRESS,
 REGISTER,
 NO_OP

};
/* represents 24 bits  */
typedef struct mila {
 unsigned int binary;
}mila;
typedef struct MACHINE_CODE_INSTRUCTION{
 int addr;/* Address of the instruction */
 mila  mila;/* Binary code (mila) */
 struct MACHINE_CODE_INSTRUCTION *next;/* Pointer to the next instruction */
}MACHINE_CODE_INSTRUCTION;

typedef struct ADDRESS_Node{
 int adr;/* Address */
 struct ADDRESS_Node * next;/* Pointer to the next address */
}ADDRESS_Node;

typedef struct ENTRY_LIST {
 char label[MAXLABLE]; /* Label name */
 ADDRESS_Node * addr_list; /* List of addresses for this entry  if LABEL_LIST only one node*/
 struct ENTRY_LIST * next; /* Pointer to the next entry */
} ENTRY_LIST,EXTERN_LIST,LABEL_LIST;



typedef struct MACHINE_CODE_COMMAND{
 int  addr; /* Address of the command */
 mila  binary_code;  /* Binary code for the command */
 char label[MAXLABLE];  /* Label associated with the command */
 int type;  /* Type of command */
 struct MACHINE_CODE_COMMAND * next;  /* Pointer to the next command */
}MACHINE_CODE_COMMAND;

typedef struct ASSEMBLER_TABLE {
 MACRO_LIST_NODE *  macro_head;  /* Head of macro list */
 LABEL_LIST *  label_head; /* Head of label list */
 ENTRY_LIST * entry_head; /* Head of entry list */
 EXTERN_LIST * extern_head; /* Head of extern list */
 MACHINE_CODE_COMMAND * command_head; /* Head of command list */
 MACHINE_CODE_INSTRUCTION * instruction_head; /* Head of instruction list */
} ASSEMBLER_TABLE;

void free_ADDRESS_Node(ADDRESS_Node **head);
void free_LABEL_LIST(LABEL_LIST **head);
void free_ENTRY_LIST(ENTRY_LIST **head);
void free_EXTERN_LIST(EXTERN_LIST **head);
void free_MACHINE_CODE_COMMAND(MACHINE_CODE_COMMAND **head);
void free_MACHINE_CODE_INSTRUCTION(MACHINE_CODE_INSTRUCTION **head);
void free_assembler_table(ASSEMBLER_TABLE **table);
/* this functions convarts a 32 bit int to a mila struct of 24 bits
 without returning any errors
 *
 number the 32 int to be convarted

 */
void int_to_mila(int number ,mila * mila);

void insert_address_list(ADDRESS_Node **address_list, char *label, int address);
/*
 This function adds a new instruction node, containing an address and a unsigned short value,
 to the end of the linked list of machine code instructions. If the list is empty,
 the new node becomes the head.

 instruction_list Pointer to the head of the MACHINE_CODE_INSTRUCTION linked list.
 address The address associated with the instruction.
 mila unsigned short instruction representation to store in the node.
 */
void add_to_instruction_list(MACHINE_CODE_INSTRUCTION **instruction_list , int address , mila mila);

/*
  This function allocates memory for a new LABEL_LIST node and appends it to the end
  of the LABEL list. The label contains a name a address and a  linked list.

 label_list Pointer to the head of the label list.
 label Name of the label to be added.
 address The address associated with the label.
 */
void insert_Label_List(LABEL_LIST **label_list, char *label, int address);

/*
  This function allocates memory for a new ENTRY_LIST node and appends it to the end
  of the ENTRY_LIST. The label contains a name a address linked list and a  linked list.
  the address linked list statrs as null

 entry_list Pointer to the head of the label list.
 label Name of the label to be added.
 */
void insert_Entry_List(ENTRY_LIST **entry_list, char *label);

/*
  This function allocates memory for a new EXTERN_LIST node and appends it to the end
  of the EXTERN_LIST. The label contains a name a address linked list and a  linked list.
  the address linked list statrs as null

 EXTERN_LIST Pointer to the head of the label list.
 label Name of the label to be added.
 */
void insert_Extern_List(EXTERN_LIST **entry_list, char *label);

/*
 This function categorizes the line into one of the following types:
 IS_ENTRY,
 IS_EXTERN,
 IS_STRING,
 IS_DATA,
 IS_COMMAND,
 UNDEFINED_INSTRUCTION,
 IS_ERROR

 word The first word in the line we are categorizing.
 commands Pointer to the array that contains the command names .
 return The type of the line as an integer value.
 */
int first_pass_line_type(char *word, char *commands[],int *command);

/*
 * This function parses a string literal from the provided assembly line, converts each character
 * into a instruction representation, and adds each character as an instruction to the
 * `MACHINE_CODE_INSTRUCTION` list. If the string is correctly formatted (enclosed in quotes),
 * it adds each character followed by a terminating zero (null character) to the list.
 *
 instruction_list Pointer to the head of the MACHINE_CODE_INSTRUCTION linked list.
 word The word in the assembly code containing the string .
 address Pointer to the current address, which is incremented as new instructions are added.
 return Returns 1 if the string is correctly extracted, 0 if there is an error (i.e., no opening quote).
 */
int convert_string(MACHINE_CODE_INSTRUCTION **instruction_list, char *word, int *address);

/*
 This function checks if a given label is valid according to the following rules:
 label cannot be empty. cannot start with a digit. can only contain alphanumeric characters (digits, uppercase, lowercase).
 cannot be a reserved word (register name, command, or directive).

 If any of these conditions are violated, an error is printed and the function returns 0.
 Otherwise, it returns 1.

 word The label to examine for validity.
 line_count The current line number in the source file .
 Returns 1 if the label is valid, or 0 if it is invalid.
 */
int examin_label(char *word,int line_count);



int compare_label_with_other_lists(char *label, ASSEMBLER_TABLE *assembler);


int operand_Type_Identifier( char **registers, char operand[MAXLABLE]);

int Entry_Examine(char * word , int line_counter , ASSEMBLER_TABLE *assembler , char **registers);

int Extern_Examine(char * line , int line_counter , ASSEMBLER_TABLE *assembler , char **resgisters);

int number_Examine(char * word,int line_counter);

int extract_Data(MACHINE_CODE_INSTRUCTION **instruction_list, char *line, int *address, int line_counter) ;

void insert_Command_List(MACHINE_CODE_COMMAND **command_list , int addr, char label[MAXLABLE] , mila binary_code , int type);

void Mila(MACHINE_CODE_COMMAND **command_list, int command_code, char *src_op, int type_src, char *dest_op, int type_dest,int funct, int *IC);

int examine_oprand(char * oprand ,int oprand_type,int line_counter);

int command_check(int command_code ,char * line,int line_counter, char **registers);

int commandCodeToOpCode(int command_code,int *opCode,int *functCode);

int convert_Command(MACHINE_CODE_COMMAND **command_list,  char **registers, char *line, int command_code, int *IC , int line_counter);

int firstpass(ASSEMBLER_TABLE **assembler, char *file_name );
/* end of FIRST PASS and SECOND PASS functions defines and structs  */
#endif
