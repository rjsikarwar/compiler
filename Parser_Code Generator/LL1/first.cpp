#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include"first.h"
using namespace std;

/*Returns 0 if not non-terminal else returns 1*/
int check_non_term(char non_term [][20],char term[],int n)
{
	for(int i=0;i<n;i++)
		if(!strcmp(non_term[i],term))
			return 1;
	return 0;
}

/*Adds a string 'term' to first list ,and increments number of such terms*/
void add_first(first f_list[], char start[],char term[],int n,int k)
{
	for(int i=0;i<n;i++)
	{
		if(!strcmp(f_list[i].start,start))
		{
			f_list[i].prod_id[f_list[i].num]=k;
			strcpy(f_list[i].flist[f_list[i].num++],term);
		}
	}
}

/*Returns index of the particular string 'term' in the first list , i.e. whether index of the non-terminal with which the production starts ,if not found returns -1*/
int index_check(first f_list[],char term[],int n)
{
	for(int i=0;i<n;i++)
		if(!strcmp(f_list[i].start,term))
			return i;
	return -1;
}

/* Checks whether a particular string 'term' exists in the array of strings , returns 1 if exists else returns 0*/
int exists_in_arr(char arr1[][20],char term[],int n)
{
	for(int i=0;i<n;i++)
		if(!strcmp(arr1[i],term))
			return 1;
	return 0;
}


/*The following algorithm is used to compute FIRST:
	1> In the first run of the production list, all productions leading to a first term as a terminal i.e. say S->aB where S is a non-terminal, 'a' is a terminal . All such productions are checked as completed, i.e. no further need to assess them. Incorporate such terminals in first of corresponding start symbols (non-terminals ).
	2> Repeat the following(3-5) till no. of checked productions match the total no. of productions, note 'chk' variable denotes number of checked productions.
	3> For every production S->ABC.... ,where A is a non-terminal check whether productions starting with 'A' are complete or not, if complete then incorporate FIRST(A) - epsilon into FIRST(S) by checking whether they already exist in FIRST(S) or not.
	4> If epsilon exists in FIRST(A) then also add FIRST(B)-epsilon into FIRST(S) and so on.
	5> Only if epsilon exists in FIRST(A),FIRST(B),FIRST(C) such that C is the ending term of the production add epsilon to FIRST(S).
*/
void compute_first(int k, int i,prod production[],char non_term [][20] ,first f_list[],int num_non)
{


	k=1;					
	int chk=0;
	int d_it=1;
	int ch=0;
//	cout<<"i = "<<i<<endl;
	while(chk<i-2)
	{
		d_it=1;
		rec:
		if(d_it>production[k].dcount){chk++;continue;}
		if(production[k].check!=1)
		{
			
//			cout<<"chk = "<<chk<<" Checking k = "<<k<<"d_it = "<<d_it<<endl<<endl;
			int in=index_check(f_list,production[k].deriv[d_it],num_non);
//			cout<<"non-term = "<<production[k].deriv[d_it]<<" index = "<<in<<endl;
			if(check_non_term(non_term,production[k].deriv[d_it],num_non)!=1)
			{

				add_first(f_list,production[k].start,production[k].deriv[d_it],num_non,k);
//				cout<<"Added "<<production[k].deriv[1]<<" to "<<production[k].start<<endl;
//				cout<<"Marking production "<<k<<endl;
				production[k].check=1;
				chk++;
			}
			else
			{
				for(int l=0;l<f_list[in].occ_iter;l++)
				{
					int p_in= f_list[in].occ[l].val;
			//		cout<<"Production to check = "<<p_in<<endl;
					if(production[p_in].check==1)
					{
			//			cout<<"Checking production "<<p_in<<" starting with "<<production[p_in].start<<endl;
						for(int j=0;j<f_list[in].num;j++)
						{
							int in_1=index_check(f_list,production[k].start,num_non);
			//				cout<<production[k].start<<" : "<<in_1<<endl;
			//				cout<<"Checking whether "<<f_list[in].flist[j]<<" exists in "<<f_list[in_1].start<<endl;
							if(exists_in_arr(f_list[in_1].flist,f_list[in].flist[j],f_list[in_1].num) ==0)
							{
								f_list[in_1].prod_id[f_list[in_1].num]=k;
								strcpy(f_list[in_1].flist[f_list[in_1].num++],f_list[in].flist[j]);
						//-----		chk++;
			//					cout<<"Added xx "<<f_list[in].flist[j]<<" to "<<production[k].start<<endl;
								
							}
						}
						if(exists_in_arr(f_list[in].flist,(char *)"epsilon",f_list[in].num))
						{
							d_it++;
							goto rec;
						}
						chk++;
			//			cout<<"Marking production "<<k<<endl;
						production[k].check=1;
						break;
					
					}
				}
			}
		}
		
		k++;
	//	cout<<"chk = "<<chk<<" k = "<<k<<endl;
		if(k>i-2)k=1;
	}
//	cout<<"final chk = "<<chk<<endl;
}

void generate_first(char *filename,first *f_list,prod production[],int *num,int *num_prod,char non_term[][20],char start[20],char terminal[][20],int *num_term)
{
	fstream f;					/*opening a filestream f */
	f.open(filename,ios::in);

	
	char line[100];
	
	int i=0;
	
	f.getline(line,100);				/*The first line is meant for defining the list of non-terminals (space separated) */
	stringstream s;
	s<<line;
	char junk[20];
	
	s>>junk;
	while(s>>non_term[i++])
	{
		strcpy(f_list[i-1].start,non_term[i-1]);
		f_list[i-1].num=0;
	}
	int num_non=i;			//num_non =No. of non terminals
	s.clear();

	f.getline(line,100);				/*The third line in the input file is meant for defining the start symbol in the grammar */
	s<<line;
	int t=0;
	
	s>>junk;		
	while(s>>terminal[t++])
	{
		strcpy(f_list[t-1].start,non_term[t-1]);
		f_list[t-1].num=0;
	}
	*num_term=t;			//num_term =No. of terminals
	s.clear();



	f.getline(line,100);				/*The third line in the input file is meant for defining the start symbol in the grammar */
	s<<line;
	s>>junk>>start;
	i=0;


	while(f)
	{
		f.getline(line,100);	/*Extracting linewise from the filestream */
		stringstream s;		/*Building a stringstream object to read the line word by word */
		s<<line;		/*Copy the line into the stringstream s */
		char str[100];		/*Temporary buffer to hold the string */
		int j=0;
		while(s>>str)		/*Start manipulating the stringstream object */
		{
			if(j==0)	/*The beginning of the line is always denoted by the Start symbol */
			{
				strcpy(production[i].start,str);
				int in=index_check(f_list,str,num_non);
				int iter=f_list[in].occ_iter++;
				f_list[in].occ[iter].val=i;
				production[i].dcount=0;
				production[i].check=0;
			}
			else if(!strcmp("|",str))	/*Incase of '|' a new production rule is created with the same start symbol */
			{
				strcpy(production[i+1].start,production[i].start);
				int in=index_check(f_list,production[i].start,num_non);
                                int iter=f_list[in].occ_iter++;
				f_list[in].occ[iter].val=i+1;

				production[i+1].dcount=0;
				production[i].check=0;
				i++;
				j=1;
			}
			else if(strcmp("->",str))	/*Ignore '->' , for all other strings ,incorporate them into deriv list of production*/
			{
				strcpy(production[i].deriv[j-1],str);
				production[i].dcount++;
				}
			j++;
		}
		i++;
	}
	int k=1;
	/*Displaying the production list has been commented out , uncomment if you wish to see the list of productions */
	/*
	cout<<"Production list\n";
	
	
	while(k<i-1)
	{
		cout<<production[k].start<<" -> ";	
		int j=0;
		while(j++<production[k].dcount)
		{
			cout<<production[k].deriv[j]<<" ";
		}
		cout<<endl;
		k++;
	}	
	*/

	compute_first(k, i,production,non_term ,f_list,num_non); /*Calling compute_first function */

	/*Displaying the first table */
	cout<<"FIRST TABLE \n";
	for(k=0;k<num_non-1;k++)
	{
		cout<<f_list[k].start<<" : ";
		for(int j=0;j<f_list[k].num;j++)
			cout<<f_list[k].flist[j]<<", "/*<<" corresponding to production "*/;//<<f_list[k].prod_id[j]<<" , ";
		cout<<endl;
	}
	
	*num=num_non;
	*num_prod=i-1;

}	/*End */
