#include"red_black_tree.h"
#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAX_STRING_LEN  32

/*  this file has functions to test a red-black tree of integers */

void StrDest(void* a) {
  free((int*)a);
}



int StrComp(const void* a,const void* b) {
  return  strncmp(a, b, MAX_STRING_LEN);	
}

void StrPrint(const void* a) {
  printf("%s", (char *)a);
}

void InfoPrint(void* a) {
  ;
}

void InfoDest(void *a){
  ;
}

int main() {
  stk_stack* enumResult;
  int option=0;
  
  char newKey[MAX_STRING_LEN],newKey2[MAX_STRING_LEN];
  char* newStr;
  rb_red_blk_node* newNode;
  rb_red_blk_tree* tree;

  tree=RBTreeCreate(StrComp,StrDest, InfoDest,StrPrint,InfoPrint);
  while(option!=8) {
    printf("choose one of the following:\n");
    printf("(1) add to tree\n(2) delete from tree\n(3) query\n");
    printf("(4) find predecessor\n(5) find sucessor\n(6) enumerate\n");
    printf("(7) print tree\n(8) quit\n");
    do option=fgetc(stdin); while(-1 != option && isspace(option));
    option-='0';
    switch(option)
      {
      case 1:
	{
	  printf("type key for new node\n");
	  scanf("%s",newKey);
	  newStr=(char*) malloc(sizeof(char) * MAX_STRING_LEN);
	  strncpy(newStr, newKey, MAX_STRING_LEN);
/*	  *newInt=newKey;    */
	  RBTreeInsert(tree,newStr,0);
	}
	break;
	
      case 3:
	{
	  printf("type key of node to query for\n");
	  scanf("%s",newKey);
	  if ( ( newNode = RBExactQuery(tree,newKey) ) ) {/*assignment*/
	    printf("data found in tree at location %s\n",(char*) (newNode->key));
	  } else {
	    printf("data not in tree\n");
	  }
	}
	break;
      case 7:
	{
	  RBTreePrint(tree);
	}
	break;
      case 8:
	{
	  RBTreeDestroy(tree);
	  return 0;
	}
	break;
      default:
	printf("Invalid input; Please try again.\n");
      }
  }
  return 0;
}




