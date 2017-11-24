#include <stdio.h>
#include <fcntl.h>
#include<time.h>
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
	char fname[20];
	FILE *fp;
	int root;
	int next_pos;	
};
typedef struct tree tree_t;

tree_t* create_tree(char*);
void insert(tree_t* ptr_tree,rec_t* ptr_rec);
void del(tree_t* ptr_tree,int key);
int search(tree_t* ptr_tree,int key);
void disp_node(treenode_t* p);

void write_file(tree_t* ptr_tree, treenode_t* p, int pos);
void read_file(tree_t* ptr_tree, treenode_t* p, int pos);
double time_elapsed(struct timespec *,struct timespec *);

void insert_non_full(tree_t* ptr_tree,int x_pos,rec_t* ptr_rec);
void split_child(tree_t* ptr_tree,int x_pos, int i_pos);
void merge_child(tree_t* ptr_tree,int x_pos, int i_pos);

