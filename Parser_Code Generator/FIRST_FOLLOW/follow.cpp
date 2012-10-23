#include<iostream>
#include<cstring>
using namespace std;
#include "first.h"
#include "follow.h"


void generate_follow(char *filename,follow_table *fol,first *f_list,prod *production,char *start)
{


	int num_non,num_prod;
	char non_term [100][20];
	generate_first(filename,f_list,production,&num_non,&num_prod,non_term,start);
	
	follow_chart fol_c[100];
	int fol_c_iter=0,fol_iter=0;
	cout<<endl;
/*
	for(int i=1;i<num_prod;i++)
	{
		cout<<production[i].start<<" -> ";
		for(int j=1;j<=production[i].dcount;j++)
			cout<<production[i].deriv[j]<<" ";
		cout<<endl;
	}
*/
/*	for(int k=0;k<100;k++)
		cout<<non_term[k]<<endl;
*/ 
	for(int i=1;i<num_prod;i++)
	{
		for(int j=1;j<=production[i].dcount;j++)
		{
			if(check_non_term(non_term,production[i].deriv[j],num_non))
				add_fol_c(fol_c,production[i].deriv[j],i,j,&fol_c_iter);
		}
	}	
				
/*	
	for(int i=0;i<fol_c_iter;i++)
	{
		cout<<fol_c[i].str<<" occurs in ";
		for(int j=0;j<fol_c[i].num;j++)
		{
			cout<<fol_c[i].pos[j].p_no<<" : "<<fol_c[i].pos[j].d_no<<" , ";
		}
		cout<<endl;
	}
*/
	add_fol(fol,start,(char *)"$",&fol_iter);
	
	int num=2;
	while(num--){
	for(int i=0;i<fol_c_iter;i++)
	{
		for(int j=0;j<fol_c[i].num;j++)
		{
			int p_no=fol_c[i].pos[j].p_no;
			int d_no=fol_c[i].pos[j].d_no;
			
	//		cout<<"fol_c[i].str = "<<fol_c[i].str<<endl;
			rec :
			if(!check_non_term(non_term,production[p_no].deriv[d_no+1],num_non))
				add_fol(fol,fol_c[i].str,(char *)production[p_no].deriv[d_no+1],&fol_iter);
			else {
				
				if(d_no==production[p_no].dcount)
				{
					int in=index_check_fol(fol,production[p_no].start,fol_iter);
					for(int k=0;k<fol[in].num;k++)
						add_fol(fol,fol_c[i].str,(char *)fol[in].flist[k],&fol_iter);
				}
				else{


					int in=index_check(f_list,production[p_no].deriv[d_no+1],num_non);
					for(int k=0;k<f_list[in].num;k++)
					{	
						if(strcmp(f_list[in].flist[j],"epsilon"))
							add_fol(fol,fol_c[i].str,(char *)f_list[in].flist[j],&fol_iter);
					}
					if(exists_in_arr(f_list[in].flist,(char *)"epsilon",num_non))
					{
						d_no++;
						goto rec;
					}
				}
			}
		}
	}
	}		


	cout<<"Follow table \n";
	for(int i=0;i<fol_iter;i++)
	{
		cout<<fol[i].str<<" : ";
		for(int j=0;j<fol[i].num;j++)
			cout<<fol[i].flist[j]<<" , ";
		cout<<endl;
	}

}

int index_check_fol(follow_table fol[],char term[],int n)
{
	for(int i=0;i<n;i++)
		if(!strcmp(fol[i].str,term))
			return i;
	return -1;
}

void add_fol(follow_table fol[],char term[],char str[],int *fol_iter)
{
	int iter=index_check_fol(fol,term,*fol_iter);
	if(iter==-1)
	{
		strcpy(fol[*fol_iter].str,term);
		iter=(*fol_iter)++;
	}
	
	for(int i=0;i<fol[iter].num;i++)
		if(!(strcmp(fol[iter].flist[i],str)))
			return;
	strcpy(fol[iter].flist[fol[iter].num++],str);
}



int  index_check_folc(follow_chart fol_c[],char term[],int n)
{
	for(int i=0;i<n;i++)
		if(!strcmp(fol_c[i].str,term))
			return i;
	return -1;
}
void add_fol_c(follow_chart fol_c[],char term[],int p_no,int d_no,int *fol_c_iter)
{
	int iter=index_check_folc(fol_c,term,*fol_c_iter);
	
	if(iter==-1)
	{
		strcpy(fol_c[*fol_c_iter].str,term);
		iter=(*fol_c_iter)++;
	}
	fol_c[iter].pos[fol_c[iter].num].p_no=p_no;
	fol_c[iter].pos[fol_c[iter].num++].d_no=d_no;
}
		
