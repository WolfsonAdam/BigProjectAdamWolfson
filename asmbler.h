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

*/
void get_word(char * line,char * word);
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
int line_type(char * word , MACRO_LIST_NODE *head );

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
 *
 macro_head Pointer to the  macros list .
 file_name Name of the source file to process.
 file_am file to write into.
 */
int preProcess(MACRO_LIST_NODE ** macro_head,char *file_name,FILE** file_am);


/* end of macro prossing functions defines and structs  */




#endif
