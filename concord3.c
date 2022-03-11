/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Summer 2021
 */
 #include <ctype.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"


//print correct format given node and the longest word length
void print_word(struct node_and_length print_value, void *arg)
{
    char *format = (char *)arg;
	node_t *node=print_value.node;
	char space[40];
	//pad out spacing
	for(int i=0;i<40;i++){
		space[i]='\0';
	}
	for(int i=0;i<print_value.length-strlen(node->text);i++){
		space[i]=' ';
	}
	//if there is repeating word in line
    if(print_value.node->line_num<0){
		printf(format, node->text,space,node->line,node->line_num*(-1),"*)");
	}
	//no repeating word in line
	else{
        printf(format, node->text,space,node->line,node->line_num,")");
	}
}


//delete matching words in exclusion file
node_t *delete_words(char* exp_file,node_t *head){
	if(exp_file==NULL){
		exit(1);
	}
	FILE *f;
	f = fopen(exp_file, "r");
	char *line = NULL;
	size_t len= 0; 
    char* line_words=NULL;
	ssize_t read;
	//get everyline from file
	while ((read = getline(&line, &len, f)) != -1) {
		 line[strlen(line)-1]='\0';
		 for(int i=0;i<strlen(line);i++){
			 line[i]=toupper(line[i]);
		 }
		 //split line into words
		 line_words = strtok(line, " ");
		  while(line_words!=NULL){
			  for(int a=0;a<100;a++){
			//delete every matching word in line
			  head= remove_node(head,line_words);
		  }
		   line_words=strtok (NULL, " ");	
	}
}
    return head;
}

//given a text file make it into nodes
node_t *text_to_list(char* file)
{   
    FILE *fp;
	node_t *head = NULL;
    fp = fopen(file, "r");
	char* line_words=NULL;
	char line[100];
	//line to be assigned as line property of node
	char temp_line[100];
   //int to be assigned as line_num property of node
	int i=0;
	node_t *temp_node = NULL;
	//go through the file, 100 is the max char for each line
    while(fgets(line,100, fp) != NULL){
		   line[strlen(line)-1]='\0';
		   strncpy(temp_line, line, 100);
	      line_words = strtok(line, " ");
		  //keep spliting line into words
		  while(line_words!=NULL){
			   for(int x=0;x<strlen(line_words);x++){
			 line_words[x]=toupper(line_words[x]);
		 }
		 //making nodes with current word, line number,and line
		   temp_node = new_node(line_words,i,temp_line);
		   head=add_inorder(head, temp_node);
		  line_words=strtok (NULL, " ");
	   }
		  i++;
}
   return head;
}
//#ifdef DEBUG

/*
 * Just showing the use of the linked-list routines.
 */




int main(int argc, char *argv[])
{
    char *exception_file = NULL;
    char *input_file = NULL;
    int i;  
	node_t *head=NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i+i <= argc) {
            exception_file = argv[++i];
        } else {
            input_file = argv[i];
        }
    }
	node_and_length print_value;
	head=text_to_list(input_file);
	if(exception_file!=NULL){
	    head=delete_words(exception_file,head);
	}
	//getting the longest unique word length
	int longest=longest_word(head);
    print_value.node=head;
	print_value.length=longest;
	apply(print_value, print_word, "%s %s %s (%d%s\n");
	 

	
}
