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
	char pos[100];
	char type[20];
} node;

extern node *head;				// Head of Symbol Table
node *curr, *temp;

/* This function takes a character string as arguement. If the string is not
   present in the linked list then it inserts the srting at the last and returns
   the position of the node;
 */
 
int install_token(char *str, int row, int col, char *str2)		
{
	int i = 0;
        char pos_temp[10],pos_temp2[20];
        bzero(pos_temp2,20); 
        sprintf(pos_temp,"%d",row);
        strcat(pos_temp2,pos_temp);
        strcat(pos_temp2,":");
        sprintf(pos_temp,"%d",col);
        strcat(pos_temp2,pos_temp);
	temp = (node *)malloc(sizeof(node));	// Allocate memory for new node
        strcpy(temp->token,str);		// Copy the string in the node
        strcpy(temp->pos,pos_temp2);
        strcpy(temp->type, str2);
        temp->next = NULL;			

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
                        	{
                        	 /*memcpy((void *)(curr->pos+strlen(curr->pos)),",",1);
                        	 memcpy((void *)(curr->pos+strlen(curr->pos)+1),temp->pos,strlen(temp->pos));*/
                        	strcat(curr->pos,",");
                                strcat(curr->pos,temp->pos);
                                return i;
                                }
                        else
                        {
                                i++;
                                curr=curr->next;
                        }
                }
                if(strcmp(curr->token, temp->token)==0)	// String found
                        {
                         strcat(curr->pos,",");
                         strcat(curr->pos,temp->pos);
                         return i;
                        }

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
	FILE *fp1;
	fp1 = fopen("Symtable.txt","w");
	fprintf(fp1," %s%20s%20s%20s","S.No.","Token","Type","Position");
	fprintf(fp1,"\n--------------------------------------------------------------------------------------------------------");
	if(curr == NULL)
		printf(" IS EMPTY");

        while(curr!=NULL) 
        {
		fprintf(fp1,"\n %d      %20s	%20s	%20s", i, curr->token,curr->type,curr->pos);
		i++;
		curr=curr->next;
	}
fclose(fp1);	
}   

