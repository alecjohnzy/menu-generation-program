#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Node{
	char val[1025];
	struct Node *kid;
	struct Node *sibling;
}node;

static void precode();
static char* printabs(int level);
static int tab_count(char *str);
static void tabs_strip(char *s);
static node* new_node(char *str);
static void add_parent(node *root, node *n,int tabs);
static void add_sibling(node *root, node *n);
static void add_kid(node *root, node *n);
static void code(node *cur, int level, int index);

int main()
{
	node *root=new_node("root");	//setting the root node
	char s[1025];
	int i=0;
	int p_tabs=-1;			//keeps track of the tabs of the previous line
	int n_tabs;			//keeps track of the tabs of the current line
	int max_tab=0;			//keeps track of the max tabs of the file i.e. the total levels present
	while((s[i]=getchar())!=EOF)	
	{
		i++;
		while((s[i]=getchar())!='\n')	//reads the word in that line and stores it in string s
		{
			i++;
		}
		s[i]='\0';
		i=0;
		n_tabs=tab_count(s);		//counts the number of tabs before the string which indicates which level it is at
		node *n=new_node(s);		
		if(p_tabs<n_tabs)
		{
			add_kid(root,n);	//adds a kid if the previous tabs is lesser than the current tabs
		}
		else if(p_tabs==n_tabs)
		{
			add_sibling(root,n);	//adds a sibling if the previous tabs is equal to the current tabs
		}
		else if(p_tabs>n_tabs)
		{
			add_parent(root,n,n_tabs);	//adds a parent if the previous tabs is greater than the current tabs
		}
		if(max_tab<n_tabs)
		{
			max_tab=n_tabs;
		}
		p_tabs=n_tabs;
	}
	precode(max_tab);		//prints the dictionaries 
	code(root->kid,0,1);		//prints the nested switch statements
	printf("return 0;\n");
	printf("}\n");
	return 0;
}
	
int tab_count(char *str)
{
	int count=0;
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i]=='\t')
		{
			count+=1;
		}
	}
	return(count);
}

void tabs_strip(char *s)		//removes the tabs before storing it in the tree structure
{
	int count=tab_count(s);
	int len=strlen(s);
	for(int i=0; i<(len-count); i++)
	{
		s[i]=s[i+count];
	}	
	s[len-count]='\0';
}

node* new_node(char *str)		//creates a new node
{
	node *n=(node *)malloc(sizeof(node));
	strcpy(n->val,str);
	tabs_strip(n->val);
	n->kid=NULL;
	n->sibling=NULL;
	return(n);
}

void add_parent(node *root, node *n,int tabs)
{
	node *cur=root;
	cur=cur->kid;
	int i=0;
	while(1)
	{
		if(cur->sibling==NULL)
		{
			if(i==tabs)
				break;
			cur=cur->kid;
			i++;
		}
		else
		{
			cur=cur->sibling;
		}
	}
	cur->sibling=n;
}

void add_sibling(node *root, node *n)
{
	node *cur=root;
	cur=cur->kid;
	while(1)
	{
		if(cur->sibling==NULL)
		{
			if(cur->kid==NULL)
				break;
			cur=cur->kid;
		}
		else
		{
			cur=cur->sibling;
		}
	}
	cur->sibling=n;
}

void add_kid(node *root, node *n)
{
	node *cur=root;
	if(cur->kid==NULL)
	{
		cur->kid=n;
	}
	else
	{
		cur=cur->kid;
		while(1)
		{
			if(cur->sibling==NULL)
			{
				if(cur->kid==NULL)
					break;
				cur=cur->kid;
			}
			else
			{
				cur=cur->sibling;
			}
		}
		cur->kid=n;
	}
}

void precode(int tabs)
{
	printf("#include<stdio.h>\n");
	printf("#include<stdlib.h>\n");
	printf("#include<string.h>\n");
	printf("int main()\n");
	printf("{\n");
	printf("int a[%d];\n",tabs+1);
	printf("int inp;\n");
}

void code(node *cur, int level,int index)	//preorder traversal through the structure and prints the nested switch statements
{
	if(cur==NULL)
		return;
	if(index==1)				//when index==1 that means the previous node has a child
	{
		printf("a[%d]=1;\n",level);
		printf("while(a[%d])\n{\nscanf(\"%%d\",&inp);\n",level);
		printf("switch(inp)\n{\ncase 0:\na[%d]=0;\nbreak;\n",level);
		printf("case %d:\nprintf(\"%s\\n\");\n",index,cur->val);
	}
	if(index!=1)				//adding siblings
	{
		printf("case %d:\nprintf(\"%s\\n\");\n",index,cur->val);
	}
	if(cur->kid==NULL)
	{
		printf("break;\n");
	}
		 
	code(cur->kid,level+1,1);
	if(cur->sibling==NULL)
	{
		printf("default:\nbreak;\n");
		printf("}\n}\n");
		if(level!=0)
			printf("break;\n");
	}
	code(cur->sibling,level,index+1);
}

	
