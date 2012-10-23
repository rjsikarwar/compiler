#include<iostream>
#include<iostram>
#include"first.h"
#include"follow.h"
using namespace std;
follow_table fol[100];
first f_list[100];
prod production[100];
char start[20];

void create_table(){
	generate_follow((char *)"grammar.txt",fol,f_list,production,start);
	
