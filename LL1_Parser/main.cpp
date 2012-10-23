#include<iostream>
using namespace std;
#include "first.h"
#include "follow.h"
#include<cstring>
/*main function */

typedef struct _ll1_table{
	prod production[2];
	int num;
	}ll1_table;



int index_in_arr(char arr[][20],char term[],int n)
{
	for(int i=0;i<n;i++)
		if(!strcmp(arr[i],term))
			return i;
	return -1;
}

int prod_of_first(first f_list,char term[])
{
	
	for(int i=0;i<f_list.num;i++)
		if(!strcmp(f_list.flist[i],term))
			return f_list.prod_id[i];
	return -1;
}

int follow_check(follow_table fol,char term[])
{
	for(int i=0;i<fol.num;i++)
		if(!strcmp(fol.flist[i],term))
			return 1;
	return 0;
}

void display_prod(prod p)
{
	cout<<p.start<<" -> ";
	for(int i=0;i<=p.dcount;i++)
		cout<<p.deriv[i];
	cout<<"\t";
}
int main()
{
	follow_table fol[100];
	first f_list[100];
	prod production[100];
	char start[20],non_term[100][20],terminal[100][20];
	int num_term,num_non,num_prod;
	generate_follow((char *)"grammar.txt",fol,f_list,production,start,non_term,terminal,&num_non,&num_prod,&num_term);

	num_non--;
	ll1_table table[50][50];
	
	for(int i=0;i<num_non;i++)
	{
		int in=index_check(f_list,non_term[i],num_non);
		int in1=index_check_fol(fol,non_term[i],num_non);

		for(int j=0;j<num_term;j++)
		{
			
			int res=prod_of_first(f_list[in],terminal[j]);
					
			if(res!=-1)
			{
				table[i][j].production[table[i][j].num++]=production[res];
			}
			
			if(exists_in_arr(f_list[in].flist,(char *)"epsilon",f_list[in].num) && follow_check(fol[in1],terminal[j]) )
				{
				prod p;
				strcpy(p.start,non_term[i]);
				strcpy(p.deriv[0],"epsilon");
				p.dcount=0;
				p.check=1;
				table[i][j].production[table[i][j].num++]=p;
				}
				
		}
	}

	cout<<endl<<endl;
	for(int i=0;i<num_non;i++)
	{
		for(int j=0;j<num_term;j++)
		{
			cout<<"M["<<non_term[i]<<"]["<<terminal[j]<<"] = ";
			for(int k=0;k<table[i][j].num;k++)
			{
				if(k>0)cout<<", ";
				display_prod(table[i][j].production[k]);
			}
			cout<<endl;

		}
		cout<<endl;
	}
	cout <<endl<<endl;
	return 0;
}

