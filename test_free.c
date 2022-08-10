#include <stdio.h>
#include <stdlib.h>
#include "alloc.c"
int main(int argc, char **argv){
	char **c;
	c = c_alloc_mat(3,5);
	c_free_mat(c);
	//free(c); //double free
	return(0);
}
