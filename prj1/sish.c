// 10/08 2:12am writing runExec() unsure of things like waitpid() and what the
// parent process should do 10/13 seg fault core dump (most likey in parse
// for-loop)

// 10/16 "arr" is an array containing pointers to first char in string. access
// elements via arr[i] for cd maybe do something similar to github link

// 10/20 finished part 1, exit and cd
// need to: free memory, clean up code, test

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> //waitpid()
#include <unistd.h>   //execvp()
#define MAX_ARGS 15
// read function
#define BUFFERSIZE 200
int histCount = 0;

char *readLine() {
  char *line;
  size_t bufsize = 1024;
  // dynamic allocation
  // bufsize = (int *)malloc(bufsize * sizeof(int));
  line = malloc(bufsize * sizeof(char));
  printf("sish> ");
  ssize_t chars = getline(&line, &bufsize, stdin);
  // remove \n
  if (line[chars - 1] == '\n') {
    line[chars - 1] = '\0';
    --chars;
  }

  return line;
}

int runExec(char **arr) {
  pid_t pid;
  int i;                         // index
  char *exec = arr[0];           // exectuable file name
  char *arguments[MAX_ARGS + 1]; // max args + null terminated
  int status = -1;               // execvp status

  // copy arguments from parse array

  arguments[0] = arr[0];
  for (i = 0; arr[i] != NULL; i++) {
    arguments[i + 1] = arr[i + 1];
  }

  // null terminate
  arguments[i + 1] = NULL;

  pid = fork();
  // child process not created
  if (pid == -1) {
    perror("fork error\n");
    exit(EXIT_FAILURE);
  }
  // child process created
  if (pid == 0) {
    status = execvp(exec, arguments);

    // execvp failed
    if (status == -1) {
      perror("execvp failed\n");
    }
  }
  // parent process
  else {
    waitpid(pid, NULL, 0);
  }
  return 1;
}
// parse function, also either calls runExec or runCommand
#define BUFSIZE 64
char **parse(char *line) {
  size_t i = 0;
  int len = strlen(line);
  char **arr = malloc(BUFSIZE * sizeof(char *));
  // loop to get all tokens and put into  arr
  for (char *p = strtok(line, " "); p; p = strtok(NULL, " ")) {
    // max amt of tokens
    if (i > MAX_ARGS) {
      break;
    }
    arr[i++] = p;
  }
  /*test print
  for (size_t n = 0; n != i; ++n)
{
     printf("Token %zu is '%s'.\n", n, arr[n]);
}
*/
  return arr;
}
int exitBI() {
  // if first token is exit
  exit(0);
}
// pass in arr
// get arr[1] which is the path
// get cwd
// chdir according to cwd and arr[1]
// check for errors (invalid inputs)
// print the path
// find a way to test whats in arr[1]
// find a way to test function
int cd(char **arr) {
  char cwd[BUFFERSIZE];
  if (arr[1] == NULL) {
    printf("Error no path: \n");
    return 0;
  }
  chdir(arr[1]);
  getcwd(cwd, BUFFERSIZE);
  printf("Current working dir: %s\n", cwd);

  return 0;
}

void historyFunction(char *history[], int histCount, int start) {
if(start > 100){
  for (int i = 0; i < 100; i++) {
    // print 1 history[i]
    //&& strcmp(history[i], "\0"
    if (history[i] != NULL && history[i] !="\0") {
      printf("%d ", i);
      printf("%s\n", history[i]);
    } 
}
  }
  else
  for (int i = 0; i < histCount; i++) {
    // print 1 history[i]
    //&& strcmp(history[i], "\0"
    if (history[i] != NULL && history[i] !="\0") {
      printf("%d ", i);
      printf("%s\n", history[i]);
    } 
      //printf("%s", "test");
    //histCount++;
    /*
    if (histCount >= 100) {
      histCount = 0;
    }
*/

    // history[i] = (char*)malloc(50*sizeof(char));
  }
}
void historyClearFunction(char *history[], int histCount) {
  for (int i = 0; i < histCount; i++) {
    //free(history[i]);
    history[i] = "\0";
  }
   // free(history);
}



int runCommand(char **arr) {
  // exit
  if (strcmp(arr[0], "exit") == 0) {
    // free memory first
    exitBI();
  }
  // cd
  if (strcmp(arr[0], "cd") == 0) {
    cd(arr);
  }
  return 1;
}
int main(void) {
  int b = 0;
  size_t bufsize = 1024;
  char **arr;
  int start = 0;
   int histCount = 0;
  // char **history = (char**)malloc(100*sizeof(char*));
  // init history
  //char ** history = malloc(100 * sizeof(char*));
  char *history[100];
  //char history[100][20];
  
  for (int i = 0; i < 100; i++) {
    // history[i] = NULL;
    history[i] = (char*)malloc(sizeof(char*) * 100);
    //history[i] = NULL;
  }

  while (1) {

    char *line = readLine();
    char * line2 = malloc(bufsize * sizeof(char));
    strcpy(line2, line);
    arr = parse(line);
    

    if (strcmp(arr[0], "exit") == 0) {
      b++;
      runCommand(arr);
    
    }
    if (strcmp(arr[0], "cd") == 0) {
      runCommand(arr);
      b++;
    }
    
    /*
    if (history[histCount] != NULL) {
      free(history[histCount]);
    }
    */
    

    if(history[histCount] != NULL){
      history[histCount] = strdup(line2);
      histCount = (histCount + 1) % 100;
      start++;
    //histCount++;
    }
   if (strcmp(line2, "history") == 0) {
      historyFunction(history, histCount, start);
     b++;
      // print arr[0]
    }
    
   if (strcmp(line2, "history -c") == 0) {
      historyClearFunction(history, histCount);
     b++;
     histCount = 0;
    }
    
     if((strcmp(arr[0], "history") == 0) && (arr[1] != NULL) && (strcmp(arr[1], "-c") != 0)){
    //printf("test");
        //historyOffset(arr, history, histCount);
      
       int x = atoi(arr[1]);
  
  if(x >= histCount){
    printf("error: offset out of bounds \n");
    //return 0;
  }
  if (history[x] != NULL && history[x] !="\0"){
    arr = parse(history[x]);
    runExec(arr);
  }
  
       b++;
     }
      
      

    //else
      if(b == 0) {
        runExec(arr);
      }
        b = 0;
    
    
    //strcpy(history[histCount], line);
      //histCount++;
      //history[histCount] = line;
     //history[histCount] = strdup(line);
    // histCount = (histCount + 1) % 100;
  
  }
}
