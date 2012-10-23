
/*First Occurence is a struct used for defining the no. of occurrences of a particular non-terminal as the starting symbol for a production */
typedef struct _first_occ{
	int val;
	}first_occ;

/*Prod is a struct used for defining the list of productions , used as prod [], it contains a start symbol, the derivations from the start symbol, note
	that the start symbol is itself not a part of the derivation unless explicitly mentioned in the grammar, 
	that is to say : S->aB , will contain S as start, and 'a' and 'B' in the derivation list.
	it contains a dcount which keeps a count of the length of the derivation. in the above example dcount=2;
	a check field is introduced to mark it as checked if the production has been totally accounted for. as in A->bc, has no terminals, hence it will be
	marked as checked after the first run. (If you're not sure about this leave it, it'll be explained later)
*/
typedef struct _prod{
	char start[10];
	char deriv[10][20];
	int dcount;
	int check;
	}prod;

/*First is the struct used to define the first table, it has a start state, a list of symbols which are in its FIRST(), 
	flist stores all the list of terminals that the particular non-terminal (start) produces in its FIRST
	no. of such occurrences, that is num (count of flist)
	occ gives the list of all positions of non-terminals that are in FIRST(start)
	occ_iter denotes the count of occ
*/
typedef struct _first{
	char start[20];
	char flist[100][20];
	int prod_id[100];
	first_occ occ[20];
	int occ_iter;
	int num;
	}first;





/*index_check function returns the index of a particular string in first, i.e. the index of the term in the start table */
int index_check(first [],char [],int);

void add_first(first [],char [],char [],int,int);
/*add_first function adds a non-terminal to the first table by looking up the term in the first table */

int check_non_term(char [][20],char [],int);
/*checks whether it is a non-terminal or not */

int exists_in_arr(char [][20],char [],int);
/*checks whether the string exists in the array of strings */

void compute_first(int , int ,prod [],char [][20] ,first [],int );
/*computes the first table */

void generate_first(char *,first *,prod *,int *,int *,char [][20],char *,char [][20],int *);
/*generates the first table by calling compute_first */

