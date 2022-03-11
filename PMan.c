#include <unistd.h>          
#include <string.h>          
#include <ctype.h>             
#include <stdio.h>             
#include <stdlib.h>           
#include <sys/types.h>        
#include <sys/wait.h>          
#include <signal.h>          
#include <readline/readline.h> 


typedef struct node {
	pid_t pid;
	int isRunning;
	char* process;
	struct node* next;
} node;
node* backgroundProcess = NULL;


/*
	s: a string
s a node with a given pid to the list of processes
*/
node* getNodeFromList(pid_t pid) {
	node* current = backgroundProcess;
	while (current != NULL) {
		if (current->pid == pid) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

int processExist(pid_t pid) {
	node* theProcess = backgroundProcess;
	while (theProcess != NULL) {
		if (theProcess->pid == pid) {
			return 1;
		}
		theProcess = theProcess->next;
	}
	return 0;
}

void removeProcess(pid_t pid)
{
    node *current;
    node *previous;
    current=backgroundProcess;
	if (current->pid == pid){
        backgroundProcess =backgroundProcess->next;
        free(current);
		return;
    }
    while(current!=NULL){
      if (current->pid == pid){
		  previous->next=current->next;
		  free(current);
	       return;
    }
	previous=current;
	current=current->next;
}
}
void bg(char** userInput) {
	pid_t pid = fork();
	if (pid == 0) {    // child
		char* command = userInput[1];
		execvp(command, &userInput[1]);
		printf("cannot execute command %s\n", command);
		exit(1);
	} else if (pid > 0) {		// parent
	    node* newProcess = (node*)malloc(sizeof(node));
	    printf("starting background process %d\n", pid);
		newProcess->process = userInput[1];
	    newProcess->pid = pid;
	    newProcess->isRunning = 1;
	    newProcess->next = NULL;
	    if (backgroundProcess == NULL) {
		backgroundProcess = newProcess;
	  } else {
		node* theProcess = backgroundProcess;
		while (theProcess->next != NULL) {
			theProcess= theProcess->next;
		}
		theProcess->next = newProcess;
	}
		sleep(1);
	} else {
		perror("fork failed");
		exit(1);
	}
}
void bgkill(pid_t pid) {
	if (!processExist(pid)) {
		printf("Error: process %d does not exist",pid);
		return;
	}
	int success = kill(pid, SIGTERM);
	if (success==0) {
	    removeProcess(pid);
		printf("Background process %d was killed.\n", pid);
		sleep(1);
	} else {
		printf("Error: cannot execute command\n");
	}
}

/*
	pid: a process id
	sends the STOP signal to a process pid to temporarily stop it
*/
void bgstop(pid_t pid) {
	if (!processExist(pid)) {
		printf("Error: process %d does not exist",pid);
		return;
	}
	int success = kill(pid, SIGSTOP);
	if (success==0) {
		node* n = getNodeFromList(pid);
		n->isRunning = 0;
		printf("Background process %d was stopped.\n", pid);
		sleep(1);
	} else {
		printf("Error: cannot to execute command\n");
	}
}

/*
	pid: a process id
	sends the CONT signal to a stopped process pid to restart it
*/
void bgstart(pid_t pid) {
	if (!processExist(pid)) {
		printf("Error: process %d does not exist",pid);
		return;
	}
	int success = kill(pid, SIGCONT);
	if (success==0) {
		printf("Background process %d was started.\n", pid);
		node* n = getNodeFromList(pid);
		n->isRunning = 1;
		sleep(1);
	} else {
		printf("Error: cannot to execute command\n");
	}
}

/*
	displays a list of all programs currently executing in the background
*/
void bglist() {
	int count = 0;
	node* head = backgroundProcess;

	while (head != NULL) {
		count++;
		if (!head->isRunning) {
		printf("%d:\t %s (stopped)\n", head->pid, head->process);
		}
		else{
		printf("%d:\t %s \n", head->pid, head->process);
		}
		head = head->next;
	}
	printf("Total background jobs:\t%d\n", count);
}

/*
	pid: a process id
	lists information relevant to a process pid
*/
void readFile(char* filePath, char fileContents[200][200] ) {
	FILE *fp = fopen(filePath, "r");
	char filestream[1024];
	if (fp != NULL) {
		int i = 0;
		while (fgets(filestream, sizeof(filestream)-1, fp) != NULL) {
			char* token;
			token = strtok(filestream, " ");
			strncpy(fileContents[i], token,200);
			while (token != NULL) {
				strncpy(fileContents[i], token,200);
				token = strtok(NULL, " ");
				i++;
			}
		}
		fclose(fp);
	} else {
		printf("Error: could not read file\n");
	}
}

void pstat(pid_t pid) {
	if (processExist(pid)) {
		char procstat[200];
		char procstatus[200];
		sprintf(procstat, "/proc/%d/stat", pid);
		sprintf(procstatus, "/proc/%d/status", pid);
		char statlist[200][200];
		readFile(procstat, statlist);       
		char statuslist[200][200];
		readFile(procstatus,statuslist);
		char* a;
		long  int utime = strtoul(statlist[13], &a, 10) / sysconf(_SC_CLK_TCK);
		long int stime = strtoul(statlist[14], &a, 10) / sysconf(_SC_CLK_TCK);
		printf("comm:\t%s\n", statlist[1]);
		printf("state:\t%s\n", statlist[2]);
		printf("utime:\t%lu\n", utime);
		printf("stime:\t%lu\n", stime);
		printf("rss:\t%s\n", statlist[23]);
		printf("%s", statuslist[88]);
		printf("%s", statuslist[89]);
		
	} else {
		printf(" Process does not exist.\n");
	}
}

int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

void InputToCommand(char** userInput) {
	if(strcmp(userInput[0],"bglist")==0){
		bglist();
	}
	else if(strcmp(userInput[0],"bg")==0&&userInput[1]!=NULL){
	   bg(userInput);
	}
	else if(userInput[1]==NULL){
		printf("%s","pid is needed");
	}
	
	else if(strcmp(userInput[0],"bgkill")==0&&isNumber(userInput[1])){
	   pid_t pid = atoi(userInput[1]);
	   bgkill(pid);
	}
	   
	else if(strcmp(userInput[0],"bgstop")==0&&isNumber(userInput[1])){
	   pid_t pid = atoi(userInput[1]);
	   bgstop(pid);
	}
	   
	else if(strcmp(userInput[0],"bgstart")==0&&isNumber(userInput[1])){
	   pid_t pid = atoi(userInput[1]);
	   bgstart(pid);
	}
	
	else if(strcmp(userInput[0],"pstat")==0&&isNumber(userInput[1])){
	   pid_t pid = atoi(userInput[1]);
	   pstat(pid);
	}
	else if(strcmp(userInput[0],"bgstart")==0||strcmp(userInput[0],"bgstop")==0||strcmp(userInput[0],"bgkill")==0&&!isNumber(userInput[1])){
		printf("%s","invalid pid");
		return;		
	}
	else{printf("PMan: > %s:\tcommand not found\n", userInput[0]);
	}
	
}

/*
	updates process list running statuses
*/

void check_zombieProcess() {
	pid_t pid;
	int	status;
	while (1) {
		pid = waitpid(-1, &status,  WNOHANG );
		if (pid > 0) {
			if (WIFEXITED(status)) {
				printf("Background process %d terminated.\n", pid);
				removeProcess(pid);
			}
		}
		else  {
			break;
		}
	}
}

/* ---------- Main ---------- */

/*
	An interactive prompt that can run processes in the background on Unix
*/

int main() {
	while (1) {
		char* userInput[128];
		char* lineinput=readline("PMan: > ");
		char* token = strtok(lineinput, " ");
     	for (int i = 0; i < 128; i++) {
		    userInput[i] = token;
		    token = strtok(NULL, " ");
	}
		if (lineinput!=NULL) {
			InputToCommand(userInput);
		}
		check_zombieProcess();
	}

	return 0;
}