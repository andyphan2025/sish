#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> //waitpid()
#include <unistd.h>   //execvp()
#define MAX_ARGS 15
#define BUFFERSIZE 200
int histCount = 0;

char *readLine() {
  char *line;
  size_t bufsize = 1024;
  // dynamic allocation
  line = malloc(bufsize * sizeof(char));
  printf("sish> ");
  ssize_t chars = getline(&line, &bufsize, stdin);
  // remove \n replace with \0
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
  return arr;
}
int exitBI() {
  // if first token is exit
  exit(0);
}

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
  //if more than 100 commands entered without clearing
  int i;
  if (start >= 100) {
    //print 100 commands
    for (i = 0; i < 100; i++) {
      if (history[i] != NULL && history[i] != "\0") {
        printf("%d ", i);
        printf("%s\n", history[i]);
      }
    }
    //otherwise print only histCount amount of commands
  } else
    for (i = 0; i < histCount; i++) {
      if (history[i] != NULL && history[i] != "\0") {
        printf("%d ", i);
        printf("%s\n", history[i]);
      }
    }
}
void historyClearFunction(char *history[], int histCount, int start) {
  //clear all commands
  int i;
  if(start >= 100){
    for ( i = 0; i < 100; i++) {
    history[i] = "\0";
  }
    }
  else
    for ( i = 0; i < histCount; i++) {
    history[i] = "\0";
  }
    
}

int runCommand(char **arr) {
  // exit
  if (strcmp(arr[0], "exit") == 0) {
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
  int start = 0; //total commands
  int histCount = 0; //current command count

  char *history[100];

  for (int i = 0; i < 100; i++) {
  //allocate mem for each index
    history[i] = (char *)malloc(sizeof(char *) * 100);
  }

  while (1) {

    char *line = readLine();
    char *line2 = malloc(bufsize * sizeof(char));
    //make copy of input before parsing
    strcpy(line2, line);
    //parse input in arr
    arr = parse(line);

    if (strcmp(arr[0], "exit") == 0) {
      exit(0);
      b++;
      runCommand(arr);
    }
    if (strcmp(arr[0], "cd") == 0) {
      runCommand(arr);
      b++;
    }

    if (history[histCount] != NULL) {
      //assign history indexes and increment commands counts
      history[histCount] = strdup(line2);
      histCount = (histCount + 1) % 100;
      start++;
    }
    if (strcmp(line2, "history") == 0) {
      //print history
      historyFunction(history, histCount, start);
      b++;
    }

    if (strcmp(line2, "history -c") == 0) {
      //clear history and reset counts
      historyClearFunction(history, histCount, start);
      b++;
      histCount = 0;
      start = 0;
    }

    if ((strcmp(arr[0], "history") == 0) && (arr[1] != NULL) && (strcmp(arr[1], "-c") != 0)) {
      //history with offset
      //get int value from arg
      int x = atoi(arr[1]);
      
      if (x >= histCount) {
        printf("error: offset out of bounds \n");
      }
      //run command at specified index
      if (history[x] != NULL && history[x] != 0) {
        arr = parse(history[x]);
        runExec(arr);
      }

      b++;
    }
    //if no built-in commands are run, runExec()
    if (b == 0) {
      runExec(arr);
    }
    b = 0;
  }
}

