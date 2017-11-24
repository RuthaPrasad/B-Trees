#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <fcntl.h>
#include <string.h>
#include<time.h>
#include "asgn3_header.h"
#define ALLOC(x) (x*)malloc(sizeof(x))
struct timespec start, end;
double time_elapsed(struct timespec *start,struct timespec *end)
{
	double time =0.0;
	time = (end->tv_sec - start->tv_sec )*1000;
	time += (end->tv_nsec - start->tv_nsec )*0.000001;
	return time;
}
tree_t* create_tree(char* fname)
{
	tree_t *p=ALLOC(tree_t); 
	strcpy(p->fname,fname);	
	p->fp = fopen(fname, "w+");//reading and writing
	p->root = 0;
	p->next_pos = 0;
	return p;
}
void disp_node(treenode_t* temp)
{
	printf("keys\n");
	for(int i = 0;i<=2*t-1;i++)
	{
		printf("%d ", temp->x[i]->key);
	}
	printf("\n");
	printf("links\n");
	for(int i=0;i<=2*t; i++)
	{
		printf("%d ", temp->c[i]);
	}
	printf("\n");
}

void split_child(tree_t* ptr_tree,int x_pos, int i_pos)
{
	treenode_t* p=ALLOC(treenode_t);//parent
	for(int m=0;m<2*t;m++)
	{
		p->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,p,x_pos);
	treenode_t* y=ALLOC(treenode_t);//child
	for(int m=0;m<2*t;m++)
	{
		y->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,y,p->c[i_pos]);
	treenode_t* z=ALLOC(treenode_t);//create sibling
	z->n=t-1;
	z->leaf=y->leaf;
	for(int m=0;m<2*t;m++)//initialize the new node
	{
		z->x[m]=ALLOC(rec_t);
	}
	for(int i=0;i<2*t;++i)
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
	if(y->leaf!=1)//split child into 2 siblings,copy links if not a leaf
	{
		for(int j=1;j<=t; ++j)
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

	write_file(ptr_tree, p,x_pos);//write back into parent
	write_file(ptr_tree, z,ptr_tree->next_pos);//add new sibling into tree
	write_file(ptr_tree, y,p->c[i_pos]);//write back to child
	ptr_tree->next_pos++;
}
void insert_non_full(tree_t* ptr_tree,int x_pos,rec_t* ptr_rec)
{
	treenode_t* p=ALLOC(treenode_t);
	for(int m=0;m<2*t;m++)
	{
		p->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,p,x_pos);//read node at pos
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
		write_file(ptr_tree,p,x_pos);
	}
	else//internal node
	{
		int j=p->n;
		while((1<=j) && ((p->x[j]->key)>ptr_rec->key))//find child to insert key into
		{
			j--;	
		}
		j++;
		treenode_t *child=ALLOC(treenode_t);
		for(int m=0;m<2*t;m++)
		{
			child->x[m]=ALLOC(rec_t);
		}
		read_file(ptr_tree,child,p->c[j]);
		if(child->n==2*t-1)//if child node full
		{
			split_child(ptr_tree,x_pos,j);//split child
			read_file(ptr_tree,p,x_pos);
			if(p->x[j]->key<ptr_rec->key)
			{
				j++;			
			}
		}
		int x_pos=p->c[j];
		insert_non_full(ptr_tree,x_pos,ptr_rec);
	}				

}
void insert(tree_t* ptr_tree,rec_t* ptr_rec)
{
	if(ptr_tree->root==0 && ptr_tree->next_pos==0)//initialize a root if tree empty
	{
		treenode_t *roo=ALLOC(treenode_t);
		roo->n=0;
		roo->leaf=1;
		for(int m=0;m<2*t;m++)
		{
			roo->x[m]=ALLOC(rec_t);
		}
		for(int i=0;i<2*t;++i)
		{
			roo->x[i]->key=1000000;
			roo->c[i] = -1;
		}
		roo->c[2*t]=-1;
		write_file(ptr_tree,roo,0);
		ptr_tree->next_pos++;
		insert(ptr_tree,ptr_rec);	
	}
	treenode_t *r=ALLOC(treenode_t);
	for(int m=0;m<2*t;m++)
	{
		r->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,r,ptr_tree->root);//start at root
	if(r->n==2*t-1)//root is full
	{
		treenode_t *s=ALLOC(treenode_t);
		for(int m=0;m<2*t;m++)
		{
			s->x[m]=ALLOC(rec_t);
		}
		s->leaf=0;
		s->n=0;
		for(int i=0;i<2*t;++i)
		{
			s->x[i]->key=1000000;
			s->c[i] = -1;
		}
		s->c[2*t] = -1;
		s->c[1]=ptr_tree->root;
		write_file(ptr_tree,s,ptr_tree->next_pos);//create new empty root
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
	treenode_t *p=ALLOC(treenode_t);//parent
	for(int m=0;m<2*t;m++)
	{
		p->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,p,x_pos);
	treenode_t *left=ALLOC(treenode_t);
	for(int m=0;m<2*t;m++)
	{
		left->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,left,p->c[i_pos-1]);//left sibling
	treenode_t *right=ALLOC(treenode_t);
	for(int m=0;m<2*t;m++)
	{
		right->x[m]=ALLOC(rec_t);
	}
	read_file(ptr_tree,right,p->c[i_pos]);//right sibling
	p->x[i_pos]->key=left->x[left->n]->key;//replace key in parent
	left->n=left->n-1;//decrease number of keys in left
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
	left->n=left->n+right->n;//merge number of keys 			
	right->leaf=1;
	right->n=0;
	for(int i=0;i<2*t;++i)//free right sibling
	{
		right->x[i]->key=1000000;
		right->c[i] = -1;
	}
	right->c[2*t] = -1;
	write_file(ptr_tree,p,x_pos);//write back into parent
	write_file(ptr_tree,left,p->c[i_pos-1]);//write back into left
	write_file(ptr_tree,right,p->c[i_pos]);//write back to right
}

void del(tree_t* ptr_tree,int key)
{
	bool found=false;
	int i;
	int parent=-1;
	int parent_parent=-1;
	treenode_t *p=ALLOC(treenode_t);
	for(int m=0;m<2*t;m++)
	{
		p->x[m]=ALLOC(rec_t);
	}
	if(ptr_tree->next_pos!=0)//non-empty tree
	{
		int pos=ptr_tree->root;//start searching from root
		while(pos!=-1 && !found)//search till leaf OR found
		{
			read_file(ptr_tree,p,pos);//read node at pos
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
		read_file(ptr_tree,p,parent);//read node 
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
			p->x[2*t-1]->key=1000000;
			p->n=p->n-1;//decrease number of keys
			write_file(ptr_tree,p,parent);
			if(p->n<t)// if minimum degree is disturbed after delete
			{
				treenode_t *par=ALLOC(treenode_t);
				for(int m=0;m<2*t;m++)
				{
					par->x[m]=ALLOC(rec_t);
				}
				read_file(ptr_tree,par,parent_parent);//read parent of leaf
				i=1;
				while((i<2*t) && ((par->c[i])<parent))//search full parent found 
				{	i++;	}
				treenode_t*left=ALLOC(treenode_t);
				for(int m=0;m<2*t;m++)
				{
					left->x[m]=ALLOC(rec_t);
				}
				//read_file(ptr_tree,right,par->c[i+1]);//read right sibling
				treenode_t*right=ALLOC(treenode_t);
				for(int m=0;m<2*t;m++)
				{
					right->x[m]=ALLOC(rec_t);
				}
				if(i!=1 && left->n>=t)//if left>t sibling exists
				{
					read_file(ptr_tree,left,par->c[i-1]);//read left sibling
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
					read_file(ptr_tree,left,par->c[i-1]);//write back left sibling
					read_file(ptr_tree,par,parent_parent);//write back parent
					read_file(ptr_tree,p,parent);//write back to leaf
					
				}
				else if(right->n>=t)//if right>t sibling exists
				{
					read_file(ptr_tree,right,par->c[i+1]);//read right sibling
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
					read_file(ptr_tree,right,par->c[i+1]);//write back right sibling
					read_file(ptr_tree,par,parent_parent);//write back parent
					read_file(ptr_tree,p,parent);//write back to leaf
				
				}
				else if(i!=1 && left->n<t)//if left<t sibling exists
				{
					read_file(ptr_tree,left,par->c[i-1]);//read left sibling
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
					read_file(ptr_tree,left,par->c[i-1]);//write back left sibling
					read_file(ptr_tree,par,parent_parent);//write back parent
					read_file(ptr_tree,p,parent);//write back to leaf
				}
				else if(right->n<t)//if right<t sibling exists
				{
					read_file(ptr_tree,right,par->c[i+1]);//read right sibling
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
					read_file(ptr_tree,right,par->c[i+1]);//write back right sibling
					read_file(ptr_tree,par,parent_parent);//write back parent
					read_file(ptr_tree,p,parent);//write back to leaf
					
				}

			}
		}
		else//if internal node
		{
			treenode_t *right=ALLOC(treenode_t);
			for(int m=0;m<2*t;m++)
			{
				right->x[m]=ALLOC(rec_t);
			}
			read_file(ptr_tree,right,p->c[i]);//read node at right child
			treenode_t *left=ALLOC(treenode_t);
			for(int m=0;m<2*t;m++)
			{
				left->x[m]=ALLOC(rec_t);
			}
			read_file(ptr_tree,left,p->c[i-1]);//read node at left child
			
			if(p->c[i-1]!=-1 && left->n >0 && left->leaf==0)//push down into left child
			{
				int temp=p->x[i]->key;
				p->x[i]->key=left->x[left->n]->key;
				left->x[left->n]->key=temp;
				write_file(ptr_tree,p,parent);
				write_file(ptr_tree,left,p->c[i-1]);
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
				write_file(ptr_tree,p,parent);
				write_file(ptr_tree,right,p->c[i]);
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
				write_file(ptr_tree,p,parent);
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
				write_file(ptr_tree,p,parent);
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
				write_file(ptr_tree,p,parent);
				write_file(ptr_tree,right,p->c[i]);
			}
			else if(left->n >=t && left->leaf==1)//push down into left if left==leaf
			{
				p->x[i]->key=left->x[left->n]->key;
				left->x[left->n]->key=-1;
				left->c[left->n+1]=-1;
				write_file(ptr_tree,p,parent);
				write_file(ptr_tree,left,p->c[i-1]);
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
		treenode_t *temp=ALLOC(treenode_t);
		for(int m=0;m<2*t;m++)
		{
			temp->x[m]=ALLOC(rec_t);
		}
		int i;
		while(pos!=-1)//search till leaf OR found
		{
			read_file(ptr_tree,temp,pos);//read node at pos
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
void write_file(tree_t* ptr_tree, treenode_t* p, int pos)
{
	fseek(ptr_tree->fp, pos * sizeof(treenode_t), 0);
	fwrite(p, sizeof(treenode_t), 1, ptr_tree->fp);
}
void read_file(tree_t* ptr_tree, treenode_t* p, int pos)
{
	fseek(ptr_tree->fp, pos * sizeof(treenode_t), 0);
	fread(p, sizeof(treenode_t), 1, ptr_tree->fp);
}




