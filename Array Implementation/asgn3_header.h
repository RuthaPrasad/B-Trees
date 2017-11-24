#include<time.h>
#include <stdio.h>
#include <stdlib.h>
struct rec
{
	int key;
	// rest
	char country[4];
	char grate[5];
	int score;
	int rate;
};
// 1 , AFG , DIST , 93 , 18
typedef struct rec rec_t;
struct treenode
{
	int leaf;
	int n;
	rec_t* x[2*t];//keys
	int c[2*t+1]; // links
};
typedef struct treenode treenode_t;
struct tree
{
	int root;
	int next_pos;	
	treenode_t* node[size];
};
typedef struct tree tree_t;

tree_t* create_tree();
void insert(tree_t* ptr_tree,rec_t* ptr_rec);
void del(tree_t* ptr_tree,int key);
int search(tree_t* ptr_tree,int key);
void disp_node(treenode_t* p);

void insert_non_full(tree_t* ptr_tree,int x_pos,rec_t* ptr_rec);
void split_child(tree_t* ptr_tree,int x_pos, int i_pos);
void merge_child(tree_t* ptr_tree,int x_pos, int i_pos);

double time_elapsed(struct timespec*,struct timespec*);


