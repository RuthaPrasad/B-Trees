#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
#include<time.h>
#include "asgn3_header.h"
#define ALLOC(x) (x*)malloc(sizeof(x))
double time_elapsed(struct timespec *start,struct timespec *end)
{
	double time =0.0;
	time = (end->tv_sec - start->tv_sec )*1000;
	time += (end->tv_nsec - start->tv_nsec )*0.000001;
	return time;
}
tree_t* create_tree()
{
	tree_t *p;
	p = ALLOC(tree_t); 
	for(int i=0;i<size;i++)
	{
		p->node[i]=ALLOC(treenode_t);
		for(int m=0;m<2*t;m++)
		{
			p->node[i]->x[m]=ALLOC(rec_t);
		}
	}
	//initialize a root
	p->root = 0;
	p->next_pos = 1;
	treenode_t *roo=p->node[p->root];
	roo->n=0;
	roo->leaf=1;
	for(int i=0;i<2*t;++i)
	{
		roo->x[i]->key=-1;
		roo->c[i] = -1;
	}
	roo->c[2*t] = -1;
	return p;
}
void disp_node(treenode_t* temp)
{
	printf("keys :");
	for(int i = 0;i<=2*t-1;i++)
	{
		printf("%d ", temp->x[i]->key);
	}
	printf("\n");
	printf("links :");
	for(int i=0;i<=2*t; i++)
	{
		printf("%d ", temp->c[i]);
	}
	printf("\n");
}
void split_child(tree_t* ptr_tree,int x_pos, int i_pos)
{
	treenode_t *p=ptr_tree->node[x_pos];//parent
	treenode_t *y=ptr_tree->node[p->c[i_pos]];//child
	treenode_t *z=ptr_tree->node[ptr_tree->next_pos];//create sibling
	z->n=t-1;
	z->leaf=y->leaf;
	for(int i=0;i<2*t;++i)//initialize the new node
	{
		z->x[i]->key=1000000;
		z->c[i] = -1;
	}
	z->c[2*t] = -1;
	for(int j=1;j<=t-1; ++j)//split child into 2 siblings,copy keys
	{
		z->x[j]->key=y->x[j+t]->key;
		strcpy(z->x[j]->country,y->x[j+t]->country);
		strcpy(z->x[j]->grate,y->x[j+t]->grate);
		z->x[j]->score=y->x[j+t]->score;
		z->x[j]->rate=y->x[j+t]->rate;	
	}
	if(y->leaf!=1)
	{
		for(int j=1;j<=t; ++j)//split child into 2 siblings,copy links if not a leaf
		{
			z->c[j]=y->c[j+t];
		}
	}
	y->n=t-1;
	for(int j=p->n+1;j>=i_pos+1;--j)//push links of parent to accomodate new sibling
	{
		p->c[j+1]=p->c[j];
	}
	p->c[i_pos+1]=ptr_tree->next_pos;//add new sibling link in parent
	int j;
	for(j=p->n;j>=i_pos;--j)//push keys of parent to accomodate new key 
	{
		p->x[j+1]->key=p->x[j]->key;
		strcpy(p->x[j+1]->country,p->x[j]->country);
		strcpy(p->x[j+1]->grate,p->x[j]->grate);
		p->x[j+1]->score=p->x[j]->score;
		p->x[j+1]->rate=p->x[j]->rate;	
	}
	p->x[i_pos]->key=y->x[t]->key;//add new key in parent
	strcpy(p->x[j]->country,y->x[t]->country);
	strcpy(p->x[j]->grate,y->x[t]->grate);
	p->x[j]->score=y->x[t]->score;
	p->x[j]->rate=y->x[t]->rate;	
	p->n=p->n+1;
	ptr_tree->next_pos++;
		
}
void insert_non_full(tree_t* ptr_tree,int x_pos,rec_t* ptr_rec)
{
	treenode_t* p=ptr_tree->node[x_pos];//read node at pos
	if(p->leaf==1)//leaf
	{
		int j=p->n;
		while((1<=j)&&((p->x[j]->key)>ptr_rec->key))//push keys to accomodate new key
		{
			p->x[j+1]->key=p->x[j]->key;
			strcpy(p->x[j+1]->country,p->x[j]->country);
			strcpy(p->x[j+1]->grate,p->x[j]->grate);
			p->x[j+1]->score=p->x[j]->score;
			p->x[j+1]->rate=p->x[j]->rate;
			j--;	
		}
		p->x[j+1]->key=ptr_rec->key;//add new key in node
		strcpy(p->x[j+1]->country,ptr_rec->country);
		strcpy(p->x[j+1]->grate,ptr_rec->grate);
		p->x[j+1]->score=ptr_rec->score;
		p->x[j+1]->rate=ptr_rec->rate;	
		p->n=p->n+1;//increase size of node
	}
	else//internal node
	{
		int j=p->n;
		while((1<=j) && ((p->x[j]->key)>ptr_rec->key))//find child to insert key into
		{
			j--;	
		}
		j++;
		treenode_t *child=ptr_tree->node[p->c[j]];
		if(child->n==2*t-1)//if child node full
		{
			split_child(ptr_tree,x_pos,j);//split child
			if(p->x[j]->key<ptr_rec->key)
			{
				j++;			
			}
		}		
		insert_non_full(ptr_tree,p->c[j],ptr_rec);	
	}				
}
void insert(tree_t* ptr_tree,rec_t* ptr_rec)
{
	treenode_t *r=ptr_tree->node[ptr_tree->root];//start at root
	if(r->n==2*t-1)//root full
	{
		treenode_t *s=ptr_tree->node[ptr_tree->next_pos];//create new empty root
		s->leaf=0;
		s->n=0;
		for(int i=0;i<2*t;++i)
		{
			s->x[i]->key=1000000;
			s->c[i] = -1;
		}
		s->c[2*t] = -1;
		s->c[1]=ptr_tree->root;
		ptr_tree->root=ptr_tree->next_pos++;		
		split_child(ptr_tree,ptr_tree->root,1);//split filled root	
		insert_non_full(ptr_tree,ptr_tree->root,ptr_rec);//insert at new root
	}
	else//root is not full
	{
		insert_non_full(ptr_tree,ptr_tree->root,ptr_rec);
	}

}
void merge_child(tree_t* ptr_tree,int x_pos, int i_pos)
{
	treenode_t *p=ptr_tree->node[x_pos];//parent
	treenode_t *left=ptr_tree->node[p->c[i_pos-1]];//left sibling
	treenode_t *right=ptr_tree->node[p->c[i_pos]];//right sibling
	p->x[i_pos]->key=left->x[left->n]->key;
	left->n=left->n-1;//decrease number of keys of left 
	for(int j=1;j<=2*t-left->n;++j)//compact keys of left by merging keys of right
	{
		left->x[left->n+j]->key=right->x[j+1]->key;
		strcpy(left->x[left->n+j]->country,right->x[j+1]->country);
		strcpy(left->x[left->n+j]->grate,right->x[j+1]->grate);
		left->x[left->n+j]->score=right->x[j+1]->score;
		left->x[left->n+j]->rate=right->x[j+1]->rate;	
	}
	p->x[2*t-1]->key=1000000;
	p->c[i_pos]=-1;
	left->n=left->n+right->n;////merge number of keys			
	right->leaf=1;
	right->n=0;
	for(int i=0;i<2*t;++i)//free right
	{
		right->x[i]->key=1000000;
		right->c[i] = -1;
	}
	right->c[2*t] = -1;
}

void del(tree_t* ptr_tree,int key)
{
	bool found=false;
	int i;
	int parent=-1;
	int parent_parent=-1;
	treenode_t *p;
	if(ptr_tree->next_pos!=0)//non-empty tree
	{
		int pos=ptr_tree->root;//start searching from root
		while(pos!=-1 && !found)//search till leaf OR found
		{
			p=ptr_tree->node[pos];//read node at pos
			i=1;
			while((i<2*t) && ((p->x[i]->key)<key))//search full node
			{	i++;	}
			if((i<2*t)&&(p->x[i]->key)==key)//if key found in node
			{
				found=true;//stop searching
			}
			parent_parent=parent;
			parent=pos;
			pos=p->c[i];//go into appropriate subtree
		}
	}
	if(found==true)//if key found
	{
		p=ptr_tree->node[parent];
		if(p->leaf==1)//if leaf,directly delete key
		{
			for(int j=i;j<2*t;++j)//compact links of node
			{
				p->c[j]=p->c[j+1];
			}
			p->c[2*t]=-1;
			int j;
			for(j=i;j<2*t-1;++j)//compact keys of node
			{
				p->x[j]->key=p->x[j+1]->key;
				strcpy(p->x[j]->country,p->x[j+1]->country);
				strcpy(p->x[j]->grate,p->x[j+1]->grate);
				p->x[j]->score=p->x[j+1]->score;
				p->x[j]->rate=p->x[j+1]->rate;	
			}
			p->x[2*t-1]->key=-1;
			p->n=p->n-1;//decrease number of keys
			
			if(p->n<t)// if minimum degree is disturbed after delete
			{
				treenode_t* par=ptr_tree->node[parent_parent];//read parent of leaf
				i=1;
				while((i<2*t) && ((par->c[i])<parent))//search full parent found 
				{	i++;	}
				treenode_t*left=ptr_tree->node[par->c[i-1]];
				treenode_t*right=ptr_tree->node[par->c[i+1]];
				if(i!=1 && left->n>=t)//if left>t sibling exists
				{
					int temp=par->x[i-1]->key;
					par->x[i-1]->key=left->x[left->n]->key;
					left->x[left->n]->key=1000000;
					for(int j=p->n;j>=1;--j)//push keys of leaf to accomodate new key
					{
						p->x[j+1]->key=p->x[j]->key;
						strcpy(p->x[j+1]->country,p->x[j]->country);
						strcpy(p->x[j+1]->grate,p->x[j]->grate);
						p->x[j+1]->score=p->x[j]->score;
						p->x[j+1]->rate=p->x[j]->rate;	
					}
					p->n=p->n+1;//increase number of keys in leaf
					p->x[p->n]->key=temp;//add key to restore leaf>=t	
				
				}
				else if(right->n>=t)//if right>t sibling exists
				{
					int temp=par->x[i+1]->key;
					par->x[i+1]->key=right->x[1]->key;
					for(int j=1;j<2*t;++j)//compact links of node
					{
						right->c[j]=right->c[j+1];
					}
					right->c[2*t]=-1;
					for(int j=1;j<2*t-1;++j)//compact keys of node
					{
						right->x[j]->key=right->x[j+1]->key;
						strcpy(right->x[j]->country,right->x[j+1]->country);
						strcpy(right->x[j]->grate,right->x[j+1]->grate);
						right->x[j]->score=right->x[j+1]->score;
						right->x[j]->rate=right->x[j+1]->rate;	
					}
					right->x[2*t-1]->key=-1;
					right->n=right->n-1;//decrease number of keys 
					p->n=p->n+1;//increase number of keys in leaf
					p->x[p->n]->key=temp;//add key to restore leaf>=t	
				
				}
				else if(i!=1 && left->n<t)//if left<t sibling exists
				{
					left->n=left->n+1;//increase number of keys in left sibling
					left->x[left->n]->key=par->x[i-1]->key;//add parent key into left
					for(int j=1;j<p->n;++j)//merge keys of leaf into left sibling
					{
						left->x[left->n+j]->key=p->x[j]->key;
						strcpy(left->x[left->n+j]->country,p->x[j]->country);
						strcpy(left->x[left->n+j]->grate,p->x[j]->grate);
						left->x[left->n+j]->score=p->x[j]->score;
						left->x[left->n+j]->rate=p->x[j]->rate;	
					}
					par->x[i-1]->key=p->x[p->n]->key;//replace parent key by last key of leaf
					par->c[i]=-1;//delete leaf from parent
					left->n=left->n+p->n;//increase number of keys
				}
				else if(right->n<t)//if right<t sibling exists
				{
					for(int j=right->n;j>=1;--j)//push keys of right to accomodate leaf
					{
						right->x[p->n+1+j]->key=right->x[j]->key;
						strcpy(right->x[p->n+1+j]->country,right->x[j]->country);
						strcpy(right->x[p->n+1+j]->grate,right->x[j]->grate);
						right->x[p->n+1+j]->score=right->x[j]->score;
						right->x[p->n+1+j]->rate=right->x[j]->rate;	
					}
					right->x[right->n+p->n]->key=p->x[i+1]->key;
					for(int j=2;j<=p->n;++j)//merge keys of leaf into right sibling
					{
						right->x[j-1]->key=p->x[j]->key;
						strcpy(right->x[j-1]->country,p->x[j]->country);
						strcpy(right->x[j-1]->grate,p->x[j]->grate);
						right->x[j-1]->score=p->x[j]->score;
						right->x[j-1]->rate=p->x[j]->rate;	
					}
					par->x[i+1]->key=p->x[1]->key;//replace parent key by first of leaf
					par->c[i]=-1;//delete leaf from parent
					right->n=p->n+right->n;//increase number of keys
					
				}

			}
		}
		else//if internal node
		{
			treenode_t *right=ptr_tree->node[p->c[i]];//read node at right child
			treenode_t *left=ptr_tree->node[p->c[i-1]];//read node at left child
			
			if(p->c[i-1]!=-1 && left->n >0 && left->leaf==0)//push down into left child
			{
				int temp=p->x[i]->key;
				p->x[i]->key=left->x[left->n]->key;
				left->x[left->n]->key=temp;
				del(ptr_tree,key);//call delete again
			}
			else if(p->c[i-1]==-1 && right->leaf==0 && right->n>=t)//pull up key from right>=t if no left
			{
				p->x[i]->key=right->x[1]->key;
				p->c[i-1]=right->c[1];
				for(int j=1;j<2*t;++j)//compact links of node
				{
					right->c[j]=right->c[j+1];
				}
				right->c[2*t]=-1;
				
				for(int j=1;j<2*t-1;++j)//compact keys of node
				{
					right->x[j]->key=right->x[j+1]->key;
					strcpy(right->x[j]->country,right->x[j+1]->country);
					strcpy(right->x[j]->grate,right->x[j+1]->grate);
					right->x[j]->score=right->x[j+1]->score;
					right->x[j]->rate=right->x[j+1]->rate;	
				}
				right->x[2*t-1]->key=-1;
				right->n=right->n-1;//decrease number of keys 	
			}
			else if(p->c[i-1]==-1 && right->leaf==0 && right->n<t && p->n>=t)//compact parent>=t if right<t
			{
				for(int j=i-1;j<2*t;++j)//compact links of node
				{
					p->c[j]=p->c[j+1];
				}
				p->c[2*t]=-1;
				int j;
				for(j=i;j<2*t-1;++j)//compact keys of node
				{
					p->x[j]->key=p->x[j+1]->key;
					strcpy(p->x[j]->country,p->x[j+1]->country);
					strcpy(p->x[j]->grate,p->x[j+1]->grate);
					p->x[j]->score=p->x[j+1]->score;
					p->x[j]->rate=p->x[j+1]->rate;	
				}
				p->x[2*t-1]->key=-1;
				p->n=p->n-1;//decrease number of keys
			}
			else if(p->c[i-1]==-1 && right->leaf==0 && right->n<t && p->n<t)//if parent<t right<t,merge
			{
				for(int j=p->n+1;j>=p->c[i];--j)//push links of parent to accomodate right node
				{
					p->c[j+1+right->n]=p->c[j];
				}
				for(int j=1;j>=right->n+1;++j)//copy right nodes links
				{
					p->c[i-2+j]=right->c[j];
				}
				for(int j=1;j<=right->n;++j)//compact keys of node
				{
					p->x[i-1+j]->key=right->x[j]->key;
					strcpy(p->x[i-1+j]->country,right->x[j]->country);
					strcpy(p->x[i-1+j]->grate,right->x[j]->grate);
					p->x[i-1+j]->score=right->x[j]->score;
					p->x[i-1+j]->rate=right->x[j]->rate;	
				}
				p->n=p->n+right->n;//increase number of keys
			}
			
			else if(right->n >=t && right->leaf==1)//push down into right if right==leaf
			{
				p->x[i]->key=right->x[1]->key;
				for(int j=1;j<2*t;++j)//compact links of node
				{
					right->c[j]=right->c[j+1];
				}
				right->c[2*t]=-1;
				int j;
				for(j=1;j<2*t-1;++j)//compact keys of node
				{
					right->x[j]->key=right->x[j+1]->key;
					strcpy(right->x[j]->country,right->x[j+1]->country);
					strcpy(right->x[j]->grate,right->x[j+1]->grate);
					right->x[j]->score=right->x[j+1]->score;
					right->x[j]->rate=right->x[j+1]->rate;	
				}
				right->x[2*t-1]->key=-1;
				right->n=right->n-1;//decrease number of keys 			
			}
			else if(left->n >=t && left->leaf==1)//push down into left if left==leaf
			{
				p->x[i]->key=left->x[left->n]->key;
				left->x[left->n]->key=-1;
				left->c[left->n+1]=-1;
			}
			else if(left->n<t && right->n<t && left->leaf==1 && right->leaf==1)//merge if right<t left<t and both leaf, and delete key	
			{
				merge_child(ptr_tree,parent,i);
			}	
		
		}				

	}
	
};
int search(tree_t* ptr_tree,int key)
{
	int parent=-1;//initialize default return parent_node
	if(ptr_tree->next_pos!=0)//non-empty tree
	{
		int pos=ptr_tree->root;//start searching from root
		treenode_t *temp;
		int i;
		while(pos!=-1)//search till leaf OR found
		{
			temp=ptr_tree->node[pos];//read node at pos
			i=1;
			while((i<2*t) && ((temp->x[i]->key)<key))//search full node
			{	i++;	}
			if((i<2*t)&&(temp->x[i]->key)==key)//if key found in node
			{
				return pos;//stop searching,return node position
			}
			parent=pos;
			pos=temp->c[i];//go into appropriate subtree
					
		}
	}
	return parent;//if not found return closest node
}



