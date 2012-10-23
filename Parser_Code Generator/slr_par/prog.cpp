
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

char terminals[10][5],nonterminals[10][5],productions[15][20];
int table[10][10],table_SLR[50][20];
int goto_SLR[50][20];
char action[50][20][2];
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

//--------------------------------------------------------SIMPLE LR-------------------------------------------------------------
typedef struct my_states
{
    int prod;
    int dot;
    my_states *next;
};

typedef struct complete_my_states
{
    my_states *state;
    int index;
    complete_my_states *next;
};

complete_my_states *complete_head,*complete_tail;
int global_index;

int check_present_production(int i,my_states *t)
{
    while(t!=NULL)
    {
        if((t->prod)==i && (t->dot)==0)
            return 1;

        t=t->next;
    }
    return 0;
}

my_states* closure(my_states *I)
{
    if(I==NULL)
        return NULL;

    my_states *temp_head,*temp_tail,*t1,*t2;

    temp_head=new my_states;
    temp_head->prod=I->prod;
    temp_head->dot=I->dot;
    temp_head->next=NULL;

    temp_tail=temp_head;

    t1=I->next;
    while(t1!=NULL)
    {
        t2=new my_states;
        t2->next=NULL;
        t2->prod=t1->prod;
        t2->dot=t1->dot;

        temp_tail->next=t2;
        temp_tail=t2;
        t1=t1->next;
    }

    int flag1=1,flag2=1,flag3=1,prod,dot,i,j,k;
    char NT;

    //cout<<"here1";
    //cin>>pj;

    while(flag1==1)
    {
        flag1=0;

        t1=temp_head;
        while(t1!=NULL)
        {
            prod=t1->prod;
            dot=t1->dot;
            NT=productions[prod][dot+5];

            if(is_non_terminal(NT)==1)
            {
                for(i=0;i<prods;i++)
                {
                    if(productions[i][0]==NT)
                    {
                        if(check_present_production(i,temp_head)==0)
                        {
                            t1=new my_states;
                            t1->next=NULL;
                            t1->prod=i;
                            t1->dot=0;
                            temp_tail->next=t1;
                            temp_tail=t1;
                            flag1=1;
                        }
                    }
                }
            }
            t1=t1->next;
        }
    }

    /*t1=close;
    while(t1!=NULL)
    {
        cout<<productions[(t1->prod)]<<"\n";
        t1=t1->next;
    }
    cout<<"Exit";
    cin>>pj;*/

    return temp_head;
}

my_states* get_goto(my_states *I,char NT)
{
    my_states *head,*tail,*t1,*t2,*result;

    t1=I;
    head=tail=NULL;

    while(t1!=NULL)
    {
        if(productions[(t1->prod)][(t1->dot)+5]==NT)
        {
            t2=new my_states;
            t2->next=NULL;
            t2->prod=t1->prod;
            t2->dot=(t1->dot)+1;

            if(head==NULL)
            {
                head=tail=t2;
            }
            else
            {
                tail->next=t2;
                tail=t2;
            }
        }
        t1=t1->next;
    }

    result = closure(head);

    return result;
}

int check_equal_item(my_states *state1,my_states *state2)
{
    my_states *t1,*t2;
    t1=state1;
    t2=state2;

    int count1=0,count2=0;

    while(t1!=NULL)
    {
        count1++;
        t1=t1->next;
    }

    while(t2!=NULL)
    {
        count2++;
        t2=t2->next;
    }

    if(count1!=count2)
        return 0;

    t1=state1;
    while(t1!=NULL)
    {
        int flag=0;

        t2=state2;
        while(t2!=NULL)
        {
            if(t1->prod==t2->prod && t1->dot==t2->dot)
                flag=1;
            t2=t2->next;
        }
        if(flag==0)
            return 0;
        t1=t1->next;
    }

    return 1;
}

int check_present_item(my_states *closed)
{
    complete_my_states *t1;
    t1=complete_head;
    while(t1!=NULL)
    {
        if(check_equal_item(t1->state,closed)==1)
        {
            return t1->index;
        }
        t1=t1->next;
    }
    return -1;
}

int get_all_states(my_states *I)
{
    my_states *closed;
    closed = closure(I);

    int val=check_present_item(closed);

    if(val==-1)
    {
        complete_my_states *temp;
        my_states *t1;
        temp=new complete_my_states;
        temp->index=global_index;
        global_index++;
        temp->next=NULL;
        temp->state=closed;

        if(complete_head==NULL)
        {
            complete_head=complete_tail=temp;
        }
        else
        {
            complete_tail->next=temp;
            complete_tail=temp;
        }

        int i,j,k;

        for(i=0;i<term_cnt;i++)
        {
            t1=get_goto(closed,terminals[i][0]);
            if(t1!=NULL)
            {
                k=get_all_states(t1);
                table_SLR[(temp->index)][i]=k;
            }
        }

        for(i=0;i<nonterm_cnt;i++)
        {
            t1=get_goto(closed,nonterminals[i][0]);
            if(t1!=NULL)
            {
                k=get_all_states(t1);
                table_SLR[(temp->index)][term_cnt+i]=k;
            }
        }
        return (temp->index);
    }
    else
        return val;
}

void get_ACTION_table()
{
    complete_my_states *t1;
    my_states *t2;
    t1=complete_head;

    char a;

    while(t1!=NULL)
    {
        t2=t1->state;

        while(t2!=NULL)
        {
            a=productions[(t2->prod)][(t2->dot)+5];
            if(is_terminal(a)==1)
            {
                int x=0;
                while(terminals[x][0]!=a)
                    x++;

                if(table_SLR[(t1->index)][x]!=-1)
                {
                    action[(t1->index)][x][0]='s';
                    action[(t1->index)][x][1]=table_SLR[(t1->index)][x];
                }
            }
            else if(a=='\0' || a=='@')
            {
                char NT=productions[(t2->prod)][0];

                list *temp;
                temp=head;
                while(temp->val!=NT)
                    temp=temp->next;

                char my_FOLLOW[20];
                int ind_t;
                strcpy(my_FOLLOW,temp->FOLLOW);
                int i,j;
                for(i=0;i<strlen(my_FOLLOW);i++)
                {
                    if(my_FOLLOW[i]=='$')
                    {
                        action[(t1->index)][term_cnt][0]='r';
                        action[(t1->index)][term_cnt][1]=(t2->prod);
                    }
                    else
                    {
                        ind_t=0;
                        while(terminals[ind_t][0]!=my_FOLLOW[i])
                            ind_t++;

                        action[(t1->index)][ind_t][0]='r';
                        action[(t1->index)][ind_t][1]=(t2->prod);
                    }
                }
            }

            if(t2->prod==0 && t2->dot==1)
            {
                action[(t1->index)][term_cnt][0]='a';
                action[(t1->index)][term_cnt][1]='c';
            }

            t2=t2->next;
        }

        t1=t1->next;
    }
}

void get_GOTO_table()
{
    int i,j,k;

    for(i=0;i<global_index;i++)
    {
        for(j=term_cnt;j<term_cnt+nonterm_cnt;j++)
        {
            goto_SLR[i][j-term_cnt]=table_SLR[i][j];
        }
    }
}

void parse_SLR(char *inp)
{
    char symbols[100],input[100],a;
    int stack[100],len=1,cnt=1,s,term_a;

    stack[0]=0;
    strcpy(symbols,"");
    strcpy(input,"$");

    int i,j,k;
    for(i=strlen(inp)-1;i>=0;i--)
    {
        input[cnt++]=inp[i];
    }
    input[cnt]='\0';

    cout.width(30);
    cout<<"STACK";

    cout.width(10);
    cout<<"SYMBOLS";

    cout.width(10);
    cout<<"INPUT";
    cout<<"\n";

    while(1)
    {
        for(i=0;i<len;i++)
        {
            cout.width(3);
            cout<<stack[i];
        }
        for(i=len;i<10;i++)
            cout<<"   ";

        cout.width(10);
        cout<<symbols;

        cout.width(10);
        cout<<input;

        cout<<"\n";

        s = stack[len-1];
        a = input[strlen(input)-1];

        term_a=0;

        if(a=='$')
            term_a=term_cnt;
        else
        {
            while(terminals[term_a][0]!=a)
                term_a++;
        }

        if(action[s][term_a][0]=='s')
        {
            stack[len]=(int)action[s][term_a][1];
            len++;

            int len2=strlen(symbols);
            symbols[len2]=a;
            symbols[len2+1]='\0';

            len2=strlen(input)-1;
            input[len2]='\0';
        }
        else if(action[s][term_a][0]=='r')
        {
            int prod_len=strlen(productions[(int)(action[s][term_a][1])])-5;
            if(productions[(int)(action[s][term_a][1])][5]=='@')
                prod_len=0;
            len=len-prod_len;

            int t=stack[len-1];
            char NT=productions[(int)(action[s][term_a][1])][0];
            int num=0;

            while(nonterminals[num][0]!=NT)
                num++;

            stack[len]=goto_SLR[t][num];
            len++;

            num=strlen(symbols);
            symbols[num-prod_len]=productions[(int)(action[s][term_a][1])][0];
            symbols[num-prod_len+1]='\0';
        }
        else if(action[s][term_a][0]=='a' && action[s][term_a][1]=='c')
        {
            cout<<"\nACCEPT";
            break;
        }
        else
        {
            cout<<"\nERROR. Cannot be parsed.";
            break;
        }
    }
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

    int pj;

    //PARSE THE CFG-GRAMMAR

    cnt = get_terminals();
    cout<<"\nTerminals:\n";
    for(i=0;i<term_cnt;i++)
    {
        cout<<terminals[i]<<"\n";
    }

    cnt++;
    cnt = get_non_terminals(cnt);
    cout<<"\nNon-Terminals:\n";
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

    /*//DRAW THE LL(1) TABLE
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
    }*/

    complete_head=NULL;
    complete_tail=NULL;

    global_index=0;

    for(i=0;i<50;i++)
    {
        for(j=0;j<20;j++)
        {
            table_SLR[i][j]=-1;
            strcpy(action[i][j],"");
            goto_SLR[i][j]=-1;
        }
    }

    cout<<"\n\nSTATES:\n";

    my_states *close,*I,*t1;

    I=new my_states;
    I->next=NULL;
    I->prod=0;
    I->dot=0;

    /*t1=new my_states;
    t1->next=NULL;
    t1->prod=1;
    t1->dot=1;
    I->next=t1;*/

    get_all_states(I);

    complete_my_states *t2;

    t2=complete_head;

    while(t2!=NULL)
    {
        t1=t2->state;
        int ind=t2->index;

        cout<<"\n\nState = "<<ind<<"\n";
        while(t1!=NULL)
        {
            for(i=0;i<5+(t1->dot);i++)
                cout<<productions[(t1->prod)][i];

            cout<<".";

            for(i=5+(t1->dot);i<strlen(productions[(t1->prod)]);i++)
                cout<<productions[(t1->prod)][i];

            cout<<"\n";
            t1=t1->next;
        }

        t2=t2->next;

        /*for(i=0;i<5+(close->dot);i++)
            cout<<productions[(close->prod)][i];

        cout<<".";

        for(i=5+(close->dot);i<strlen(productions[(close->prod)]);i++)
            cout<<productions[(close->prod)][i];

        cout<<"\n";
        close=close->next;*/
    }

    /*cout<<"\n\nSLR Table\n\n";

    cout<<" \t";
    for(i=0;i<term_cnt;i++)
    {
        cout<<terminals[i]<<"\t";
    }

    for(i=0;i<nonterm_cnt;i++)
    {
        cout<<nonterminals[i]<<"\t";
    }

    for(i=0;i<global_index;i++)
    {
        cout<<"\n";
        cout<<i<<"\t";
        for(j=0;j<term_cnt+nonterm_cnt;j++)
        {
            cout<<table_SLR[i][j]<<"\t";
        }
    }*/

    //GET THE ACTION AND GOTO TABLES
    get_ACTION_table();
    get_GOTO_table();

    cout<<"\n\nPRODUCTIONS\n\n";
    for(i=0;i<prods;i++)
    {
        cout<<productions[i]<<"\n";
    }

    cout<<"\n\nACTION TABLE------------------------------\n\n";

    cout<<"S\t";
    for(j=0;j<term_cnt;j++)
    {
        cout<<terminals[j]<<"\t";
    }
    cout<<"$\n";

    for(i=0;i<global_index;i++)
    {
        cout<<i<<"\t";
        for(j=0;j<=term_cnt;j++)
        {
            if(strcmp(action[i][j],"")==0)
                cout<<"\t";
            else if(action[i][j][0]!='a')
                cout<<action[i][j][0]<<(int)action[i][j][1]<<"\t";
            else
                cout<<"acc"<<"\t";
        }
        cout<<"\n";
    }

    cout<<"\n\nGOTO TABLE------------------------------\n\n";

    cout<<"S\t";
    for(j=0;j<=nonterm_cnt;j++)
    {
        cout<<nonterminals[j]<<"\t";
    }
    cout<<"\n";

    for(i=0;i<global_index;i++)
    {
        cout<<i<<"\t";
        for(j=0;j<nonterm_cnt;j++)
        {
            cout<<goto_SLR[i][j]<<"\t";
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
            parse_SLR(inp);
    }

    return 0;
}
