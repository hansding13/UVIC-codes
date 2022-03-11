#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max_file_words = 500;
int input_line_length=500;
int max_word_length=20;
int max_key_word=500;



int main(int argc, char **argv){
 
    FILE *language;
	FILE *f ;

   if(argc==4){
	  f = fopen(argv[1], "r");
	  language = fopen(argv[3], "r");
}
   else{
	 printf("%s","wrong input1");
	 return 0;	 
   }
   int word_to_line[500];
   char line[input_line_length];
   char words[max_file_words][max_word_length];
   char exist_words[max_file_words][max_word_length];
   char language_words[max_file_words][max_word_length];
   char total_lines[500][input_line_length];
   char *line_words;
   char* temp="";
   char temp_word [max_word_length];
   int i=0;
   int word_found=0;
   for (i= 0; i< max_file_words; i++) {
	   words[i][0] = '\0';  
	   }
   for (i= 0; i< max_file_words; i++) {
	   language_words[i][0] = '\0';  
	   }
   int language_words_length=0;
   while(fgets(line,input_line_length, language) != NULL){
	  line[strlen(line)-1]='\0';
	  line_words = strtok(line, " ");
	  while(line_words!=NULL){
	    strncpy(language_words[language_words_length],line_words,max_word_length);
        language_words_length++; 
	    line_words=strtok (NULL, " ");
	   }
   }
      i=0;
	  int repeat[max_file_words];
	  for (int y=0;y<max_file_words;y++){
		  repeat[y]=0;
	  }
	  int lines_length=0;
	 int  word_length=0;
	 int longest_word_length=0;
   while(fgets(line,input_line_length, f) != NULL){
	  line[strlen(line)-1]='\0';
	  strncpy(total_lines[lines_length],line,input_line_length);
	  lines_length ++;
	  line_words = strtok(line, " ");
	  while(line_words!=NULL){
	    word_found=0;
		strncpy(exist_words[word_length],line_words,max_word_length);
		for(int x=0;x<=language_words_length;x++){
		   if(strcmp(line_words,language_words[x])==0){
				word_found=1;
			}
		}
		if(word_found==0){
	       strncpy(words[word_length],line_words,max_word_length);
		   word_to_line[word_length]=lines_length;
		   if(strlen(line_words)>longest_word_length){
			   longest_word_length=strlen(line_words);
		   }
		   word_length++;
		}
	    line_words=strtok (NULL, " ");
	   }
   }
       for(i=0;i<=word_length;i++){
		   for(int x=i+1; x<word_length;x++){
           if(strcmp(words[i], words[x])> 0) {
			    int temp= word_to_line[i];
				word_to_line[i]=word_to_line[x];
				word_to_line[x]=temp;
                strncpy(temp_word, words[i],max_word_length);
                strncpy(words[i],words[x],max_word_length);
				strncpy(words[x], temp_word,max_word_length);
           }
		   }
	   }
	   int equal=0;
		for(i=0;i<word_length;i++){
			 for(int x=i+1; x<word_length;x++){
			 if(strcmp(words[i], words[x])== 0) {	 
				  if(word_to_line[i]==word_to_line[x]){
					 repeat[i]=1;
					 for(int a=i;a<word_length;a++){
					strncpy(words[a],words[a+1],max_word_length);
				
				}
					for(int b=i;b<word_length;b++){
						word_to_line[b]=word_to_line[b+1];
				 }
				   word_length=word_length-1;
					} 
					if(word_to_line[i]>word_to_line[x]){
				   if(strcmp(words[i], words[x])== 0) {	
					 int temp_num=word_to_line[x];
			     	 word_to_line[x]=word_to_line[i];
					 word_to_line[i]=temp_num;
				   }
				 }
				 
			}
					 
				 }
			 
		}
		
		
		for(i=0;i<word_length;i++){
			for(int x=0;x<strlen(words[i]);x++){
				words[i][x]=words[i][x]-32;
			}
		}
	
		longest_word_length=longest_word_length+2;
        for(i=0;i<word_length;i++){
			if(repeat[i]==0){
			printf("%s",words[i]);
			for(int x=strlen(words[i]);x<longest_word_length;x++){
				printf("%s"," ");
			}		
		    printf("%s ",total_lines[word_to_line[i]-1]);
			printf("(%d)\n",word_to_line[i]);
			 }
			if(repeat[i]==1){
				printf("%s",words[i]);
			for(int x=strlen(words[i]);x<longest_word_length;x++){
				printf("%s"," ");
			}		
		    printf("%s ",total_lines[word_to_line[i]-1]);
			printf("(%d*)\n",word_to_line[i]);
				
			}
		}


   fclose(f);  
   fclose(language); 

}
