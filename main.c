#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>//to use "bzero"
#include <time.h>//to estimate the runing time

#include "bisection.h"
#include "struct.h"

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
adjlist* readedgelist(char* input){
	unsigned long e1=NLINKS;
	adjlist *g=malloc(sizeof(adjlist));
	FILE *file;

	g->n=0;
	g->e=0;
	file=fopen(input,"r");
	g->edges=malloc(e1*sizeof(edge));
	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (g->e++==e1) {
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//building the adjacency matrix
void mkadjlist(adjlist* g){
	unsigned long i,u,v;
	g->map=malloc(g->n*sizeof(unsigned long));
	unsigned long *new=malloc(g->n*sizeof(unsigned long)),*d;
	bool *b=calloc(g->n,sizeof(bool));

	g->n=0;
	for (i=0;i<g->e;i++) {
		if (b[g->edges[i].s]==0){
			b[g->edges[i].s]=1;
			g->map[g->n]=g->edges[i].s;
			new[g->edges[i].s]=g->n++;
		}
		if (b[g->edges[i].t]==0){
			b[g->edges[i].t]=1;
			g->map[g->n]=g->edges[i].t;
			new[g->edges[i].t]=g->n++;
		}
		g->edges[i].s=new[g->edges[i].s];
		g->edges[i].t=new[g->edges[i].t];
	}
	g->map=realloc(g->map,g->n*sizeof(unsigned long));

	bzero(new,g->n*sizeof(unsigned long));
	d=new;

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(2*g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(new);
	free(b);
}

//freeing memory
void free_adjlist(adjlist *g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g->map);
	free(g);
}

//printing line (corresponding to one tree's node and associated set of nodes) in file
void printres(adjlist* g, bool *lab, char* output){
	unsigned long u;
	FILE* file=fopen(output,"w");
	for (u=0;u<g->n;u++){
		fprintf(file,"%lu %d\n",g->map[u],(int)(lab[u]));
	}
	fclose(file);
}

unsigned long sizecut(adjlist* g, bool* lab){
	unsigned long i,cut=0;
	for (i=0;i<g->e;i++) {
		if (lab[g->edges[i].s]!=lab[g->edges[i].t]){
			cut++;
		}
	}
	return cut;
}

unsigned long sizecut2(adjlist* g, bool* lab){
	unsigned long u,s=0;
	for (u=0;u<g->n;u++) {
		if (lab[u]==0){
			s++;
		}
	}
	return s;
}



//main function
int main(int argc,char** argv){
	adjlist* g;
	bool* lab;
	unsigned long cut,s;
	bisection bisec;
	time_t 	t1=time(NULL),t2;

	srand(time(NULL));

	if (argc==3)
		bisec=choosebisection("1");
	else if (argc==4)
		bisec=choosebisection(argv[3]);
	else{
		exit(1);
	}

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building adjacency array\n");
	mkadjlist(g);

	printf("Computing bisections\n");
	lab=bisec(g);

	printf("Prints resulting bisection in file %s\n",argv[2]);
	printres(g, lab, argv[2]);

	cut=sizecut(g,lab);
	s=sizecut2(g,lab);
	printf("Size of the cut = %lu\n",cut);
	printf("Size of the sets = %lu %lu\n",s,g->n-s);
	printf("Value of normalised cut = %le\n",((double)cut) / ((double)(s*(g->n-s))));
	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}

