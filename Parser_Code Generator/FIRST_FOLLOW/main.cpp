#include<iostream>
using namespace std;
#include "first.h"
#include "follow.h"
/*main function */

int main()
{
	follow_table fol[100];
	first f_list[100];
	prod production[100];
	char start[20];
	generate_follow((char *)"grammar.txt",fol,f_list,production,start);
	return 0;
}

