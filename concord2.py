
import sys


def read_each_line():
    max_word_length=0
    exclusion=None
    exclusion_list=[]
    for argument in sys.argv:
        if argument[0]=="i" and argument[1]=="n":
           file_name=argument
        if argument=="latin.txt" or argument=="english.txt" or argument=="deutsch.txt" or argument== "latin-2.txt"or argument=="english-2.txt" or argument=="deutsch-2.txt":
           exclusion=argument
    words_list= create_word_list(file_name)
    line_list= create_line_list(file_name)
    if exclusion!=None:
        exclusion_list=create_exclusion_list(exclusion)
    filtered_list=[]
    one_line_list=[]
    asterik_list=[]
    new_exclusion_list=[]
    for x in exclusion_list:
        x=x.upper()
        new_exclusion_list.append(x)
    for x,y in words_list:
        x=x.upper()
        if x not in new_exclusion_list:
            if (x,y) not in one_line_list:
                 filtered_list.append((x,y))
                 one_line_list.append((x,y))
            else:
                 asterik_list.append((x,y))
    filtered_list.sort()
    for x,y in filtered_list:
        if len(x)>max_word_length:
            max_word_length=len(x)
    max_word_length=max_word_length+2
    for x,y in filtered_list:
        print(x,end='')
        i=len(x)
        while(i<max_word_length):
             print(" ",end='')
             i=i+1
        print(line_list[y-1].rstrip(),end=' ')  
        if(x,y)in asterik_list:
             print("({index}*)".format(index=y))
        else:
             print("({index})".format(index=y))



    else: 
        return
   
    
def create_exclusion_list(file_name):
    word_list=[]
    word_file=open(file_name,'r')
    if word_file==None:
       sys.exit(1)
    for line in word_file:
       mini_list=line.split()
       word_list.extend(mini_list)
    return word_list

def create_word_list(file_name):
    word_list=[]
    word_file=open(file_name,'r')
    if word_file==None:
       sys.exit(1)
    i=1
    for line in word_file:
       mini_list=line.split()
       for words in mini_list:
           word_list.append((words,i))
       i=i+1
    return word_list

def create_line_list(file_name):
    line_list=[]
    word_file=open(file_name,'r')
    if word_file==None:
       sys.exit(1)
    for line in word_file:
       line_list.append(line)
    return line_list



def main():
    read_each_line()



if __name__ == "__main__":
    main()