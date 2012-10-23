/* SymTable.h
 * This file defines the data structure for symbol table
   Symbol Table is implemented as singly linked list.
 */

#include <stdio.h>
#include <string.h>

typedef struct node 			// Node for linked list
{
   	struct node *next;		// Pointer to next node
	char  token[256];		// Array to store the lexeme
	union {
		int ival;
		char cval;
	} val;
} node;

extern node *head;				// Head of Symbol Table
node *curr, *temp;

/* This function takes a character string as arguement. If the string is not
   present in the linked list then it inserts the srting at the last and returns
   the position of the node;
 */
 
int install_token(char *str)		
{
	int i = 0;
        
	temp = (node *)malloc(sizeof(node));	// Allocate memory for new node
        strcpy(temp->token,str);		// Copy the string in the node
        temp->next = NULL;			
 	temp->val.ival=0;

        if (head == NULL)			// First lexeme
        {
                head = temp;
                return 0;
        }
        else
        {
                curr = head;
                while(curr->next!=NULL)		// Search for the string
                {
                        if(strcmp(curr->token, temp->token)==0)
                                return i;
                        else
                        {
                                i++;
                                curr=curr->next;
                        }
                }
                if(strcmp(curr->token, temp->token)==0)	// String found
                        return i;

                curr->next = temp;			// String not found
                return i+1;

        }
}

/* This function is to print the symbol table comtents
 */

void showTable()
{
	int i = 0;
	curr = head;
	if(curr == NULL)
		printf(" IS EMPTY");
        while(curr!=NULL) 
        {
		printf("\n %4d      %s", i, curr->token);
		i++;
		curr=curr->next;
	}
}   

