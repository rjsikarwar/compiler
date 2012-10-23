#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

char terminals[10][5],nonterminals[10][5],productions[15][20];
int table[10][10];
int term_cnt=0,nonterm_cnt=0,prods=0;
char myFile[1000];
int mapping[256];

typedef struct list
{
    char val;
    char FIRST[20];
    char FOLLOW[20];
    int size;
    int size_FOLLOW;
    list *next;
};

list *head,*tail;

int is_terminal(char c)
{
    int i;
    for(i=0;i<term_cnt;i++)
    {
        if(c==terminals[i][0])
            return 1;
    }
    return 0;
}

int is_non_terminal(char c)
{
    int i;
    for(i=0;i<nonterm_cnt;i++)
    {
        if(c==nonterminals[i][0])
            return 1;
    }
    return 0;
}

int check_present(char *str,char c)
{
    int len=strlen(str);
    int i,j;
    for(i=0;i<len;i++)
    {
        if(str[i]==c)
            return 1;
    }
    return 0;
}

void get_FIRST()
{
    list *temp1,*temp2;
    int i,j,k;

    for(i=0;i<term_cnt;i++)
    {
        temp1 = new list;
        temp1->next=NULL;
        temp1->val=terminals[i][0];
        (temp1->FIRST)[0]=terminals[i][0];
        (temp1->FIRST)[1]='\0';
        temp1->size=1;

        if(head==NULL)
        {
            head=tail=temp1;
        }
        else
        {
            tail->next=temp1;
            tail=temp1;
        }
    }

    for(i=term_cnt;i<term_cnt+nonterm_cnt;i++)
    {
        temp1 = new list;
        temp1->next=NULL;
        temp1->val=nonterminals[i-term_cnt][0];
        strcpy(temp1->FIRST,"");
        temp1->size=0;

        if(head==NULL)
        {
            head=tail=temp1;
        }
        else
        {
            tail->next=temp1;
            tail=temp1;
        }
    }

    int flag1=1,flag2=1,n,m;
    char NT;

    while(flag1)
    {
        flag2=1;

        for(j=0;j<prods;j++)
        {
            char PR = productions[j][0];
            int flag3;

            temp2=head;
            while(temp2!=NULL && temp2->val != PR)
            {
                temp2 = temp2->next;
            }

            for(k=5;k<strlen(productions[j]);k++)
            {
                char NT=productions[j][k];

                if(NT=='@')
                {
                    if(check_present(temp2->FIRST,'@')==0)
                    {
                        int var_val=strlen(temp2->FIRST);
                        (temp2->FIRST)[var_val]='@';
                        (temp2->FIRST)[var_val+1]='\0';
                        temp2->size = temp2->size +1;
                        flag2=0;
                    }
                }
                else
                {
                    temp1=head;
                    while(temp1->val != NT)
                        temp1 = temp1->next;

                    flag3=0;

                    //ADD FIRST OF Yi in X
                    for(n=0;n<temp1->size;n++)
                    {
                        if(check_present(temp2->FIRST,(temp1->FIRST)[n])==0)
                        {
                            int var_val=strlen(temp2->FIRST);
                            (temp2->FIRST)[var_val]=(temp1->FIRST)[n];
                            (temp2->FIRST)[var_val+1]='\0';
                            temp2->size = temp2->size +1;
                            flag2=0;
                        }
                        if((temp1->FIRST)[n]=='@')
                            flag3=1;
                    }
                    if(flag3==0)
                        break;
                }
            }
            if(k==strlen(productions[j]) && flag3==1)
            {
                if(check_present(temp2->FIRST,'@')==0)
                {
                    (temp2->FIRST)[strlen(temp2->FIRST)]='@';
                    (temp2->FIRST)[strlen(temp2->FIRST)+1]='\0';
                    temp2->size = temp2->size +1;
                    flag2=0;
                }
            }
        }
        if(flag2==1)
            flag1=0;
    }
}

void get_FIRST_string(char *first_rem,char *left_str)
{
    if(left_str[0]=='@')
    {
        strcpy(first_rem,"@");
    }

    else
    {
        int i,j,k,flag=0;
        strcpy(first_rem,"");
        list *temp;

        for(i=0;i<strlen(left_str);i++)
        {
            flag=0;

            temp=head;
            char ch=left_str[i];

            while(temp->val != ch)
                temp=temp->next;

            for(j=0;j<strlen(temp->FIRST);j++)
            {
                if(check_present(first_rem,(temp->FIRST)[j])==0)
                {
                    if((temp->FIRST)[j]!='@')
                    {
                        int old_val=strlen(first_rem);
                        first_rem[old_val]=(temp->FIRST)[j];
                        first_rem[old_val+1]='\0';
                    }
                }
            }
            if(check_present(temp->FIRST,'@')==0)
            {
                i=strlen(left_str);
                flag=1;
            }
        }
        if(flag==0 && i>=strlen(left_str))
        {
            int old_val=strlen(first_rem);
            first_rem[old_val]='@';
            first_rem[old_val+1]='\0';
        }
    }
}

void get_FOLLOW()
{
    list *temp1,*temp2;
    int i,j,k;

    temp1=head;
    while(temp1!=NULL)
    {
        if(temp1->val=='S')
        {
            strcpy(temp1->FOLLOW,"$");
            temp1->size_FOLLOW = 1;
        }
        else
        {
            strcpy(temp1->FOLLOW,"");
            temp1->size_FOLLOW = 0;
        }
        temp1 = temp1->next;
    }

    int flag1=1,flag2=1,n,m;
    char NT;

    int pj;

    while(flag1)
    {
        flag2=1;

        for(j=0;j<prods;j++)
        {
            int size = 5;

            for(m=size;m<strlen(productions[j])-1;m++)
            {
                char left_str[20],first_rem[100];
                strcpy(left_str,productions[j]+m+1);
                NT=productions[j][m];

                temp1=head;
                while(temp1!=NULL && temp1->val != NT)
                    temp1 = temp1->next;


                if(NT != '@')
                {
                    get_FIRST_string(first_rem,left_str);

                    for(n=0;n<strlen(first_rem);n++)
                    {
                        if(check_present(temp1->FOLLOW,first_rem[n])==0)
                        {
                            if(first_rem[n]!='@')
                            {
                                int old_val=strlen(temp1->FOLLOW);
                                (temp1->FOLLOW)[old_val]=first_rem[n];
                                (temp1->FOLLOW)[old_val+1]='\0';
                                temp1->size_FOLLOW = temp1->size_FOLLOW +1;

                                flag2=0;
                            }
                        }
                    }
                }
                //cout<<"\nNT= "<<NT<<",left_str="<<left_str<<",first_rem="<<first_rem<<"\n";
            }
            /*cin>>pj;

            temp1=head;

            cout<<"\nPROD1: "<<productions[j]<<"\n";

            while(temp1!=NULL)
            {
                cout<<temp1->val<<": "<<temp1->FOLLOW<<"\n";
                temp1=temp1->next;
            }
            cin>>pj;*/


            temp1=head;
            while(temp1->val != productions[j][0])
                temp1=temp1->next;

            for(m=strlen(productions[j])-1;m>=5;m--)
            {
                if(productions[j][m] != '@')
                {
                    temp2=head;
                    while(temp2->val != productions[j][m])
                        temp2=temp2->next;

                    for(n=0;n<strlen(temp1->FOLLOW);n++)
                    {
                        if(check_present(temp2->FOLLOW,(temp1->FOLLOW)[n])==0)
                        {
                            int old_val=strlen(temp2->FOLLOW);
                            temp2->FOLLOW[old_val]=(temp1->FOLLOW)[n];
                            temp2->FOLLOW[old_val+1]='\0';
                            temp2->size_FOLLOW = temp2->size_FOLLOW +1;
                            flag2=0;
                        }
                    }
                    if(check_present(temp2->FIRST,'@')==0)
                    {
                        m=4;
                    }
                }
            }
            /*cin>>pj;

            temp1=head;

            cout<<"\nPROD2: "<<productions[j]<<"\n";

            while(temp1!=NULL)
            {
                cout<<temp1->val<<": "<<temp1->FOLLOW<<"("<<temp1->size_FOLLOW<<")"<<"\n";
                temp1=temp1->next;
            }
            cin>>pj;*/
        }

        if(flag2==1)
            flag1=0;
    }
}

int get_terminals()
{
    term_cnt=0;
    int i,j,k;
    i=0;
    k=0;
    if(myFile[0]=='T' && myFile[1]=='e' && myFile[2]=='r' && myFile[3]=='m' && myFile[4]=='i' && myFile[5]=='n' && myFile[6]=='a' && myFile[7]=='l' && myFile[8]=='s')
    {
        i = 12;
        while(myFile[i]!='\n')
        {
            if(myFile[i]!=',')
            {
                terminals[term_cnt][k]=myFile[i];
                k++;
            }
            else
            {
                terminals[term_cnt][k]='\0';
                k=0;
                term_cnt++;
                mapping[(int)(terminals[term_cnt-1][0])]=term_cnt-1;
            }
            i++;
        }
        terminals[term_cnt][k]='\0';
        term_cnt++;
    }

    return i;
}

int get_non_terminals(int i)
{
    nonterm_cnt=0;
    int k;
    k=0;
    if(myFile[i]=='N' && myFile[i+1]=='o' && myFile[i+2]=='n' && myFile[i+3]=='-' && myFile[i+4]=='t' && myFile[i+5]=='e' && myFile[i+6]=='r' && myFile[i+7]=='m' && myFile[i+8]=='i')
    {
        i = i + 16;
        while(myFile[i]!='\n')
        {
            if(myFile[i]!=',')
            {
                nonterminals[nonterm_cnt][k]=myFile[i];
                k++;
            }
            else
            {
                nonterminals[nonterm_cnt][k]='\0';
                k=0;
                nonterm_cnt++;
            }
            i++;
        }
        nonterminals[nonterm_cnt][k]='\0';
        nonterm_cnt++;
    }

    return i;
}

int get_productions(int i)
{
    int k;
    k=0;
    prods=0;

    while(myFile[i]!='~')
    {
        if(myFile[i]!='\n')
        {
            productions[prods][k]=myFile[i];
            k++;
        }
        else
        {
            productions[prods][k]='\0';
            k=0;
            prods++;

        }
        i++;
    }
    productions[prods][k]='\0';

    return i;
}

void draw_table()
{
    int i,j,k,m,n;

    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            table[i][j]=-1;
        }
    }

    char left_str[15],first_str[15];
    char NT,f_T;
    int pj;

    for(i=0;i<prods;i++)
    {
        //cout<<"here="<<i;
        //cin>>pj;

        strcpy(left_str,"");
        strcpy(left_str,productions[i]+5);
        strcpy(first_str,"");

        f_T=productions[i][0];
        n=0;
        while(nonterminals[n][0]!=f_T)
            n++;

        //cout<<"here2,productions="<<productions[i]<<",f_T"<<f_T;
        //cin>>pj;

        list *temp;
        temp=head;
        while(temp->val != f_T)
        {
            //cout<<"\nf_T="<<f_T<<",temp->val="<<(temp->val)<<"AAA";
            temp=temp->next;
            //cin>>pj;
        }

        //cout<<"here3";
        //cin>>pj;

        strcpy(left_str,productions[i]+5);
        get_FIRST_string(first_str,left_str);

        for(j=0;j<strlen(first_str);j++)
        {
            NT=first_str[j];
            if(NT != '@')
            {
                m=0;
                while(terminals[m][0]!=NT)
                    m++;

                table[n][m]=i;
            }
        }

        //cout<<"here4";
        //cin>>pj;

        if(check_present(first_str,'@')==1)
        {
            strcpy(first_str,temp->FOLLOW);

            for(j=0;j<strlen(first_str);j++)
            {
                NT=first_str[j];
                if(NT != '$')
                {
                    m=0;
                    while(terminals[m][0]!=NT)
                        m++;

                    table[n][m]=i;
                }
                else
                    table[n][term_cnt]=i;
            }
        }
    }
}


void parse_LL1(char *input)
{
    char stack[100],temp[100];
    strcpy(temp,"$");
    int i,j,k,cnt=1;

    for(i=strlen(input)-1;i>=0;i--)
    {
        temp[cnt++]=input[i];
    }
    temp[cnt]='\0';
    strcpy(input,temp);

    strcpy(stack,"$S");
    char X,a;
    int m,n;

    //cout.width(10);
    //cout<<"\n";

    cout.width(30);
    cout<<"STACK";

    cout.width(30);
    cout<<"INPUT";
    cout<<"\n";

    while(!(strcmp(stack,"$")==0 && strcmp(input,"$")==0))
    {
        //cout<<"\n";
        cout.width(30);
        cout<<stack;

        cout.width(30);
        cout<<input<<"\n";

        //int pj;
        //cin>>pj;

        X=stack[strlen(stack)-1];
        a=input[strlen(input)-1];
        m=0;
        n=0;
        if(a!='$')
        {
            while(terminals[m][0]!=a)
                m++;
        }
        else
            m=term_cnt;

        if(is_non_terminal(X)==1)
        {
            while(nonterminals[n][0]!=X)
                n++;
        }

        if(X==a)
        {
            stack[strlen(stack)-1]='\0';
            input[strlen(input)-1]='\0';
        }
        else if(is_terminal(X)==1)
        {
            cout<<"\nERROR. Cannot be parsed.";
            break;
        }
        else if(table[n][m]==-1)
        {
            cout<<"\nERROR. Cannot be parsed.";
            break;
        }
        else
        {
            int my_prod=table[n][m];
            cnt=strlen(stack)-1;

            if(productions[my_prod][5]=='@')
            {
                int temp_len=strlen(stack)-1;
                stack[temp_len]='\0';
            }

            else
            {
                //CHECK FOR EPSILON PRODUCTIONS
                for(i=strlen(productions[my_prod])-1;i>=5;i--)
                {
                    stack[cnt]=productions[my_prod][i];
                    cnt++;
                }
                stack[cnt]='\0';
            }
        }
    }

    if(strcmp(stack,"$")==0 && strcmp(input,"$")==0)
        cout<<"\nACCEPTED";
}

int main()
{
    char line[256],c;
    int cnt=0;
    int i,j,k;

    //READ THE CFG-GRAMMAR FROM FILE
    FILE  *fp;
    fp = fopen("grammar.txt","r");
    if(fp!= NULL)
    {
        c = getc(fp);
        do
        {
            myFile[cnt++]=c;
            c = getc(fp);
        }
        while(c!=EOF);

        myFile[cnt]='~';
    }

    else
        cout << "Unable to open file";

    fclose(fp);

    //PARSE THE CFG-GRAMMAR

    cnt = get_terminals();
    cout<<"\nTerminals:\n";
    for(i=0;i<term_cnt;i++)
    {
        cout<<terminals[i]<<"\n";
    }

    cnt++;
    cnt = get_non_terminals(cnt);
    cout<<"\nTerminals:\n";
    for(i=0;i<nonterm_cnt;i++)
    {
        cout<<nonterminals[i]<<"\n";
    }

    cnt+=11;
    cnt = get_productions(cnt);

    cout<<"PRODUCTIONS:\n";
    for(i=0;i<prods;i++)
        cout<<productions[i]<<"\n";

    head=tail=NULL;

    get_FIRST();
    get_FOLLOW();

    list *temp;
    temp=head;

    cout<<"\nFIRST:\n";

    while(temp!=NULL)
    {
        cout<<temp->val<<": "<<temp->FIRST<<"\n";
        temp=temp->next;
    }

    temp=head;

    cout<<"\nFOLLOW:\n";

    while(temp!=NULL)
    {
        cout<<temp->val<<": "<<temp->FOLLOW<<"\n";
        temp=temp->next;
    }

    //DRAW THE LL(1) TABLE
    draw_table();

    cout<<"\n\n----------------------------LL(1) TABLE-----------------------------------\n";

    cout.width(10);
    cout<<"Nonterm";

    for(i=0;i<term_cnt;i++)
    {
        cout.width(10);
        cout<<terminals[i];
    }
    cout.width(10);
    cout<<"$\n";

    for(i=0;i<nonterm_cnt;i++)
    {
        cout.width(10);
        cout<<nonterminals[i];

        for(j=0;j<term_cnt+1;j++)
        {
            if(table[i][j]!=-1)
            {
                cout.width(10);
                cout<<productions[table[i][j]];
            }
            else
            {
                cout.width(10);
                cout<<"";
            }
        }
        cout<<"\n";
    }

    cout<<"\n\n-------------------------------------------------------------------------\n";

    char inp[100];
    cout<<"\n";
    int op=1;

    while(op)
    {
        cout<<"\nEnter a string to parse($ to exit): ";
        cin>>inp;

        if(strcmp(inp,"$")==0)
            op=0;
        else
            parse_LL1(inp);
    }

    return 0;
}
