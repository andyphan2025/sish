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
    perror("can't fork, error occurred\n");
    exit(EXIT_FAILURE);
  }
  // child process created
  if (pid == 0) {
    status = execvp(exec, arguments);

    // execvp failed
    if (status == -1) {
      perror("Terminated Incorrectly\n");
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

int count = 0;
char history[100][BUFFERSIZE];
void historyFunction() {
  int i;
  int j = 0;
  int histCount = count;

  for (i = 0; i < 100; i++) {
    printf("%d. ", histCount);
    while (history[i][j] != '\n' && history[i][j] != '\0') {
      // printing command
      printf("%c", history[i][j]);
      j++;
    }
    printf("\n");
    j = 0;
    histCount--;
    if (histCount == 0)
      break;
  }
  printf("\n");
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
  char **arr;
  while (1) {

    char *line = readLine();
    arr = parse(line);
    if (strcmp(arr[0], "exit") == 0) {
      runCommand(arr);
      printf("%s", "hi2");
    }
    if (strcmp(arr[0], "cd") == 0) {
      runCommand(arr);
    }
    if (strcmp(arr[0], "history") == 0) {
      historyFunction();
    } else {
      runExec(arr);
    }

    // return 0;
  }
}
