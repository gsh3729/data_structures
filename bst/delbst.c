/*
	PROGRAMMER'S NAME : G SRIHARSHA
	ROLLNO:111601005
	PROGRAM DESCRIPTION : 
	PROGRAM NAME : btree.c
	DATE : 2/3/18
	*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


typedef struct node
{
	int t;
	int size;
	int leaf;
	int *keys;
	struct node **child;
}node;

typedef struct queue
{
	int data;
	struct queue *next;
}queue;

void enQueue(queue **front,queue **rear,int value);//declaring function prototypes
int deQueue(queue **front,queue **rear);
int isEmpty(queue **front);
void printQueue(queue **front);

void insert(node **root,int item);
void insertNonFull(node **x,int item);
void splitChild(node **x,int i);
void traversal(node *root);
void printBTree(node *root,int tab);
node* createNewNode();
node* search(node *root,int item,queue **front,queue **rear);

void print_spaces(int spaces);
void main()
{
	int n,i,key;
	scanf("%d",&n);

	node *root=NULL;
	queue *front=NULL,*rear=NULL;
	for (i = 0; i < n; i++)
	{
		scanf("%d",&key);
		insert(&root,key);
		printBT(root,0);		
	}
	traversal(root);
	printf("\n");
	while(1)
	{
		scanf("%d",&key);
		if(key==-1)
		{
			break;
		}
		if(search(root,key,&front,&rear))
		{
			printf("Found\n");
			printQueue(&front);
			front=NULL;
			rear=NULL;			
		}
		else
		{
			printf("Not found\n");
		}
	}


	while(1)
	{
		printf(" Menu\n 1.Insert");
		printf("\n 2.DeleteKey");
		printf("\n 3.Searching");		
		printf("\n 4.PrintTree");
		printf("\n 5.Exit\n");

		printf("Enter the option : ");
		scanf("%d",&option);
		if (option==5)
		{
			break;
		}
		switch(option)
		{
			case  1 :

				printf("Enter the key value to insert : \n");
				scanf("%d",& item);
				insert(&root,item);
				break;

			case  2 :

				printf("Enter the key value to delete : \n");
				scanf("%d",&item);
				deleteNode(&root,item);
				break;

			case  3 :

				printf("Enter the key value to search : \n");
				scanf("%d",&item );
				if(search(root,key,&front,&rear))
				{
					printf("Found\n");
					printQueue(&front);
					front=NULL;
					rear=NULL;			
				}
				break;	

			case  4 :

				printTree(root,0);
				// printf("%d\n",root->key);;
				break;				
		}
	}
}
void printBTree(node *root,int tab)
{
	int i;
	
	if (root==NULL)
	{
		return;
	}
	int x=tab;
	tab=tab+1;
	printBTree(root->child[0],tab);
	print_spaces(x*10);

	for (i = 0; i < root->size; i++)
	{
		if (i== root->size-1)
		{
			printf("%d",root->keys[i]);
		}
		else
		{
			printf("%d-",root->keys[i]);
		}
		printBTree(root->child[i+1],tab);
	}
	printf("\n\n");
	//printf("%d\n\n",root->key);
	//printBT(root->leftptr,tab);	
}

void print_spaces(int spaces)
{
    while(spaces>0)
    {
        printf(" ");
        spaces--;
    }
}

void traversal(node *root)
{
	if (root==NULL)
	{
		return;
	}
	int i=0;
	traversal(root->child[0]);
	
	while(i < (root->size))
	{
		printf("%d ",root->keys[i]);
		traversal(root->child[i+1]);
		i++;
	}

}
node* search(node *root,int item,queue **front,queue **rear)
{
    int i = 0;
    while (i < root->size && root->keys[i] < item)
    {
        i++;
    }
 
    if(root->keys[i] == item && i<=(root->size))
    {
    	enQueue(front,rear,i);	
        return root;
    }
 
    if (root->leaf == 1)
    {
    	return NULL;
    }   
    else
	{	
		enQueue(front,rear,i+1);	
  		return search(root->child[i],item,front,rear);	
  	}
}

void insert(node **root,int item)
{
	if ((*root)==NULL)
	{
		(*root)=createNewNode();
		(*root)->leaf=1;
		(*root)->keys[0]=item;
		(*root)->size=1;
		return;
	}

	if((*root)->size == (2*((*root)->t)-1))
	{
		node *newnode=createNewNode();
		newnode->leaf=0;
		newnode->size=0;
		newnode->child[0]=*root;
		(*root) = newnode;
		splitChild(root,0);

	}

	insertNonFull(root,item);
}
void insertNonFull(node **x,int item)
{
	int i;
	if ((*x)->leaf==1)
	{
		i=(*x)->size-1;
		while (i >= 0 && (*x)->keys[i] > item)
        {
            (*x)->keys[i+1] = (*x)->keys[i];
            i--;
        }
        (*x)->keys[i+1] = item;
        (*x)->size = (*x)->size +1;
	}
	else
	{
		i=(*x)->size-1;
		while (i >= 0 && (*x)->keys[i] > item)
		{
            i--;
		}
 
        if (((*x)->child[i+1])->size == 2*((*x)->t)-1)
        {
            //splitChild(&((*x)->child[i+1]),i+1);
        	splitChild(x,i+1);
            if ((*x)->keys[i+1] < item)
            {
                i++;
            }
        }
        insertNonFull(&(*x)->child[i+1],item);
	}
}


void splitChild(node **x,int i)
{
	int j;
	node *y;
	y=(*x)->child[i];
	node *z=createNewNode();
	z->size=(z->t)-1;
	z->leaf=(y->leaf);
	for (j = 0; j < (y->t)-1; j++)//copying last t-1 keys of y to z
	{
		z->keys[j]=y->keys[j+(y->t)];
	}

	if(y->leaf==0)
	{
		for (j = 0; j < (y->t); j++)//copying last t children of y to z
		{
			z->child[j]=y->child[j+(y->t)];
		}
	}

	y->size=(y->t)-1;


	for(j = ((*x)->size)-1; j >= i; j--)
	{
        (*x)->keys[j+1] = (*x)->keys[j];
	}

    (*x)->keys[i] = y->keys[(y->t)-1];


    for (j = (*x)->size; j >= i+1; j--)
    {
    	(*x)->child[j+1] = (*x)->child[j];
    }
 
    (*x)->child[i+1] = z;
 
    (*x)->size = (*x)->size + 1;

}
node* createNewNode()
{
	node *newnode = (node*)malloc(sizeof(node));
	newnode->t=3;
	newnode->child=(node**)malloc((2*(newnode->t))*sizeof(node *));
	newnode->keys=(int*)malloc((2*(newnode->t)-1)*sizeof(int));
	newnode->size=0;
	newnode->leaf=0;
	return newnode;
}



void enQueue(queue **front,queue **rear,int value)//enqueue function
{
	queue *newnode;
	newnode=(queue *)malloc(sizeof(queue));
	newnode->data=value;
	newnode->next=NULL;
	if(isEmpty(front))
	{
		(*rear)->next=newnode;
	}
	else
	{
		*front=newnode;	
	}
	*rear=newnode;
}
int deQueue(queue **front,queue **rear)//dequeue function
{
	int value;
	if(!isEmpty(front))
	{
		//printf("\n queue is empty");
		return -1;
	}
	else if(*front==*rear)
	{
		value=(*front)->data;
		*front = NULL;
		*rear = NULL;
		return value;
	}
	else
	{
		value=(*front)->data;
		*front=(*front)->next;
		return value;
	}
}
void printQueue(queue **front)
{
	queue *temp=*front;
	if (temp->next==NULL)
	{
		printf("root");
	}
	else
	{
		printf("root->");
	}	
	while(temp!=NULL)
	{
		if (temp->next==NULL)
		{
			printf(",index%d",temp->data);
			break;
		}
		if (temp->next->next==NULL)
		{
			printf("child%d",temp->data);
			temp=temp->next;
			continue;
		}
		printf("child%d->",temp->data);
		temp=temp->next;
		
	}
	printf("\n");
}
int isEmpty(queue **front)//checking whether queue is empty or not
{
	if (*front == NULL)
	{
	return 0;
	}
	else
	{
	return 1;
	}
}