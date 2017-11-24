#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include "asgn3_header.h"

int main()
{

	tree_t *tt;
	tt = create_tree();
	struct timespec start, end;
	
	
	//insert the records 
	FILE *fp = fopen("dataset.csv","r");
	rec_t rec;
	
	clock_gettime(CLOCK_REALTIME,&start);//start timer to track insert time of 1M records
	for(int i=0;i<1000000;i++)
	{
		fscanf(fp,"%d,%3s,%4s,%d,%d",&(rec.key),rec.country,rec.grate,&(rec.score),&(rec.rate));	
		insert(tt,&rec);
	}
	clock_gettime(CLOCK_REALTIME,&end);//stop timer
	printf("TIME TO INSERT 1M RECORDS: %lf ms\n\n",time_elapsed(&start,&end));
					
	
	//perform insert/delete/search
			
	int key=90;
	
	//search
	clock_gettime(CLOCK_REALTIME,&start);//start timer to track search time of key
	int pos=search(tt,key);//searches and returns closest/exact range in which the key exists
	clock_gettime(CLOCK_REALTIME,&end);//stop timer
	printf("closest key value found in node with\n");
	disp_node(tt->node[pos]);
	printf("TIME TO SEARCH FOR KEY(%d): %lf ms\n\n",key,time_elapsed(&start,&end));
	
	
	//delete
	clock_gettime(CLOCK_REALTIME,&start);//start timer
	del(tt,key);
	clock_gettime(CLOCK_REALTIME,&end);//stop timer
	pos=search(tt,key);//search again to see if key still exists
	printf("after delete, closest key value found in node with\n");
	disp_node(tt->node[pos]);
	printf("TIME TO DELETE KEY(%d): %lf ms\n\n",key,time_elapsed(&start,&end));
	
	
	//insert
	rec.key=key;
	strcpy(rec.country,"AFG");
	strcpy(rec.grate,"DIST");
	rec.score=93;
	rec.rate=18;
	clock_gettime(CLOCK_REALTIME,&start);//start timer
	insert(tt,&rec);//insert
	clock_gettime(CLOCK_REALTIME,&end);//stop timer
	pos=search(tt,key);//search again to see if key exists
	printf("after insert,closest key value found in node with\n");
	disp_node(tt->node[pos]);
	printf("TIME TO INSERT KEY(%d): %lf ms\n\n",key,time_elapsed(&start,&end));
	
	return 0;
}
