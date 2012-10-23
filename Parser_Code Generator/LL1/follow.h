typedef struct _pos_follow{
	int p_no;
	int d_no;
	}pos_follow;

typedef struct _follow_chart{
	char str[20];
	pos_follow pos[100];
	int num;
	}follow_chart;

typedef struct _follow_table{
	char str[20];
	char flist[100][20];
	int num;
	}follow_table;

void add_fol_c(follow_chart [],char [],int,int,int *);
int index_check_folc(follow_chart [], char[],int);
int index_check_fol(follow_table [],char [],int);
void add_fol(follow_table [],char [],char [],int *);
void generate_follow(char *,follow_table *,first *,prod *,char *,char [][20],char [][20],int *,int *,int* );

