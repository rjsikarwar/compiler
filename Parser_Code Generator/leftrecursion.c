#include <stdio.h>
#include <ctype.h>
char recurserem(char samplearr[], int leng);

int main(){
	FILE *fp;
	int c;
	char production[100];
	int prodindex=0;
	fp=fopen("cfg.txt","r");
	c=getc(fp);
	while(c!=EOF){
		production[prodindex]=c;
		prodindex++;
		c=getc(fp);
		}
	fclose(fp);
	printf("Array is\n%s, with length %d\n",production,prodindex);
	char pp;
	
	pp=recurserem(production,prodindex);
	printf("\n Left recurion removed\n");
	return 0;
	}

char recurserem(char samplearr[], int leng){
	static char newprod[20][40];
	
	int p=2,q=0,r=0;
	newprod[q][r]='|';
	r++;
	for(;p<leng;p++){
		if(samplearr[p]=='\n'){
			newprod[q][r]='$';
			q+=1;
			p+=3;
			r=0;
			newprod[q][r]='|';
			r++;	
			}
		newprod[q][r]=samplearr[p];
		r++;
		}
	
	int j=1,l=0;
	char nonterminals[20]; //list of non-terminals in an order.
	for(;j<leng;j++){
		if(samplearr[j]==' '){
			nonterminals[l]=samplearr[j-1];
			l++;
			}
		}
	
	j=0;
	int k=0, keeprecurtrack=0;
	char store=' ',store1=' ';
	for(;j<l;j++){
		for(;k<j;k++){
			char nontermk=nonterminals[k];
			char tempor[40];
			int cohen=0;
			for(;cohen<40;cohen++){
				tempor[cohen]=' ';
				}
			int scan=0,scan1=0;//transfer things into tempor with changed production...                	
			int counting=0;	
			while((newprod[j][scan]!='$')&&(counting<30)){
				counting++;
				if((newprod[j][scan]==nontermk)&&((newprod[j][scan-1])=='|')){
					char saverest[5];
					int savevar=1;
					while((newprod[j][scan+savevar]!='|')&&(newprod[j][scan+savevar]!='$')){//Save variables after the non-terminal. 
						saverest[savevar-1]=newprod[j][scan+savevar];
						savevar++;	
						}
					int saveleng=savevar-1;
					int scan2=1;
					while(1){
						if((newprod[k][scan2]!='|')&&(newprod[k][scan2]!='$')){
							tempor[scan1]=newprod[k][scan2];
							scan1++;
							}
						if(newprod[k][scan2]=='|'){
							int w=0;//Freaky little one character variables.
							for(;w<saveleng;w++){								
								tempor[scan1]=saverest[w];
								scan1++;
								}
							tempor[scan1]='|';
							scan1++;
							}
						else if(newprod[k][scan2]=='$'){
							int w=0;//Freaky little one character variables.
							for(;w<saveleng;w++){								
								tempor[scan1]=saverest[w];
								scan1++;
								}
							tempor[scan1]='|';
							scan1++;
							
							break;
							}
						scan2++;						
						}
					scan+=saveleng+1;//flyover rest of things and next '|'...to next production rule.
					}
				else if(newprod[j][scan-1]!='$'){
					tempor[scan1]=newprod[j][scan];
					scan1++;					
					}
				else{
					break;
					}
				scan++;
				}
			//Time to change newprod.
			int scan4=0;//recycling a variable.
			for(;scan4<scan1;scan4++){
				newprod[j][scan4]=tempor[scan4];
				}
			if(newprod[j][scan4-1]=='|'){
				newprod[j][scan4-1]='$';
				}
			else{
				newprod[j][scan4]='$';
				}	
			}
		//removing left recursion
		int istherealeftrecursion=0,scann=0;
		while(newprod[j][scann]!='$'){
			if((newprod[j][scann+1]==nonterminals[j])&&(newprod[j][scann]=='|')){
				istherealeftrecursion+=1;
				break;
				}
			scann++;
			}
		if(istherealeftrecursion==1){	
			keeprecurtrack++;
			char newnonterm=tolower(nonterminals[j]);
			nonterminals[l+keeprecurtrack-1]=newnonterm;
			char tempprod[40];//will scan through newprod and decide its fate.
			int godel=0;
			for(;godel<40;godel++){
				tempprod[godel]=' ';
				}
			char newproduction[40];//production for new nonterminal
			godel=0;
			for(;godel<40;godel++){
				newproduction[godel]=' ';
				}
			scann=0;int scann1=1,scann2=1;
			newproduction[0]='|';
			tempprod[0]='|';
			while(newprod[j][scann]!='$'){
                                if((newprod[j][scann]=='|')&&(newprod[j][scann+1]==nonterminals[j])){
					scann+=2;
					while((newprod[j][scann]!='|')&&(newprod[j][scann]!='$')){
						newproduction[scann2]=newprod[j][scann];
						scann2++;
						scann++;
						}
					if(newprod[j][scann]=='|'){
						newproduction[scann2]=newnonterm;
						scann2++;
						newproduction[scann2]='|';
						scann2++;
						}
					else if(newprod[j][scann]=='$'){
						newproduction[scann2]=newnonterm;
						scann2++;						
						}
					}
				if((newprod[j][scann]=='|')&&(newprod[j][scann+1]!=nonterminals[j])){
					scann+=1;
					while((newprod[j][scann]!='|')&&(newprod[j][scann]!='$')){
						tempprod[scann1]=newprod[j][scann];
						scann1++;
						scann++;
						}
					if(newprod[j][scann]=='|'){
						tempprod[scann1]=newnonterm;
						scann1++;
						tempprod[scann1]='|';
						scann1++;
						}
					else if(newprod[j][scann]=='$'){
						tempprod[scann1]=newnonterm;
						scann1++;						
						}
					}
				}
			newproduction[scann2]='#';//# is epsilon transition.
			scann2++;
			newproduction[scann2]='$';
			tempprod[scann1]='$';
			int kkk=0;
			while(newproduction[kkk]!='$'){
				newprod[l+keeprecurtrack-1][kkk]=newproduction[kkk];
				kkk++;
				}
			newprod[l+keeprecurtrack-1][kkk]='$';
			int gauss=0;
			for(;gauss<20;gauss++){
				newprod[j][gauss]=' ';
				}
//			For some reason, above for loop doesnt work well for gauss<40..
			int kkkk=0;
			while(tempprod[kkkk]!='$'){
				newprod[j][kkkk]=tempprod[kkkk];
				kkkk++;
				}
			newprod[j][kkkk]='$';
			}
		}	
                k=0;
	printf("\nTotal number of non terminals is%d\n",l+keeprecurtrack);
	int totallen=l+keeprecurtrack;
	int kolmogorov=0;
	for(;kolmogorov<totallen;kolmogorov++){		
		printf("Value of production for non terminal %c is %s\n",nonterminals[kolmogorov],newprod[kolmogorov]);	
		}
	//change in a 'standard format'
	FILE *fp;
	int ww;
	char charlist[]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	fp=fopen("cfg.txt","r");
	ww=getc(fp);
	int lookinlist=0;
	while(ww!=EOF){
		if(islower(ww)){
			toupper(ww);
			}
		for(;lookinlist<26;lookinlist++){
			if(charlist[lookinlist]==ww){
				charlist[lookinlist]='$';
				break;
				}
			}
		lookinlist=0;
		ww=getc(fp);
		}
	fclose(fp);
	char alllowers[15];
	int alllowersindex=0;
//	printf("Charlist is %s and lookinlist is %d\n",charlist,lookinlist);
	for(;lookinlist<totallen;lookinlist++){
		if(islower(nonterminals[lookinlist])){
			alllowers[alllowersindex]=nonterminals[lookinlist];
			alllowersindex+=1;	
			}
		}
	char corrnonterm[15];
	lookinlist=0;
	int scanthrough=0;
	for(;lookinlist<alllowersindex;lookinlist++){
		while(charlist[scanthrough]=='$'){
			scanthrough++;
			}
		corrnonterm[lookinlist]=charlist[scanthrough];
		scanthrough++;
		}
	printf("\nOld list of lowers is %s new list of corresponding uppers is %s and size of alllowers is %d",alllowers,corrnonterm,alllowersindex);
	lookinlist=0;
	int prodindex=0;
	int lookinlower=0;
	for(;lookinlower<alllowersindex;lookinlower++){
		for(;lookinlist<totallen;lookinlist++){	
			if(nonterminals[lookinlist]==alllowers[lookinlower]){
				nonterminals[lookinlist]=corrnonterm[lookinlower];
				}
			while(newprod[lookinlist][prodindex]!='$'){
				if(newprod[lookinlist][prodindex]==alllowers[lookinlower]){
					newprod[lookinlist][prodindex]=corrnonterm[lookinlower];
					}
				prodindex+=1;
				}
			prodindex=0;
			}
		lookinlist=0;
		}
	printf("\n");
	kolmogorov=0;
	for(;kolmogorov<totallen;kolmogorov++){		
		printf("Value of production for non terminal %c is %s\n",nonterminals[kolmogorov],newprod[kolmogorov]);	
		}
			
	return 'g';		
	}


