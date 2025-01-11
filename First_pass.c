
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

void insert_Extern_List(EXTERN_LIST **entry_list, char *label) {
    insert_Entry_List(entry_list, label);
}
