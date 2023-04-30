#include <stdio.h>
#include <unistd.h> // exec*
#include <stdlib.h> // exit
#include <sys/wait.h> // wait

#define BUFSIZE 256
#define ARGSCOUNT 16
#define CMDCOUNT 10

void parse(char* buf, char** args) {
  int begp = 0, endp = 0;
  int curarg = 0, cmdcount = 0;
  int isWord = 0, isFirstWord = 1;
  
  while (buf[endp] != '\n') {
    if (buf[endp] != ' ') {
      if (!isFirstWord && !isWord)
	begp = endp;
      isWord = 1;
    }
    if ((buf[endp] == ' ') && isWord) {
      buf[endp] = '\0';
      args[curarg] = &buf[begp];
      curarg++;
      isWord = isFirstWord = 0;
    }
    endp++;
  }
  
  /* check if there is unparsed last argument. */
  if (isWord) {
    buf[endp] = '\0';
    args[curarg++] = &buf[begp];
  }
  args[curarg] = NULL;
}

int parse_commands(char** args, char*** cmds) {
  int curarg = 0, curcmd = 0, cmdcnt = 0;
  int is_newcmd = 0, begcmd;
  
  while (*(args + curarg) != NULL) {
    /* check if it is a new command. */
    if ((**(args + curarg) != '|' && **(args + curarg) != ' ') && !is_newcmd) {
      is_newcmd = 1;
      begcmd = curarg;
    }
    if (**(args + curarg) == '|' && is_newcmd) {
      *(args + curarg) = NULL;
      *(cmds + curcmd) = args + begcmd;
      cmdcnt++;
      is_newcmd = 0;
      curcmd++;
    }
    curarg++;
  }
  
  /* check if there is unparsed last commands. */
  if (is_newcmd) {
    *(cmds + curcmd) = args + begcmd;
    cmdcnt++;
  }
  return cmdcnt;
}

int make_pipes(int* pipes, int cmdcount) {
  int curpipe = 0;
  for (int i = 0; i < cmdcount; i++) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
      perror("pipe() error ");
      exit(EXIT_FAILURE);
    }
    pipes[curpipe] = pipefd[0];
    pipes[curpipe + 1] = pipefd[1];
    curpipe = curpipe + 2;
  }
  return 0;
}

int main() {
  printf("Welcome! Enter the commands to run:\n");

  char buf[BUFSIZE];
  char* args[ARGSCOUNT];
  char** commands[CMDCOUNT];
  
  int cmdcount;
  fgets(buf, BUFSIZE, stdin);
  /* parse all words from user command. */
  parse(buf, args);
  /* parse all commands by "|". */
  cmdcount = parse_commands(args, commands);
  int pipefd[2 * cmdcount];
  make_pipes(pipefd, cmdcount);

  size_t pid;
  int status;
  for (int curcmd = 0; curcmd < cmdcount; curcmd++) {
    pid = fork();
    
    if (pid == -1) {
      perror("fork() error ");
      exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
      int res;
      /* first command needs input from stdin, so we do not dup2(). */
      if (curcmd != 0) {
	if (dup2(pipefd[(curcmd - 1) * 2], STDIN_FILENO) == -1) {
	  perror("dup2() (pipe input) error ");
	  exit(EXIT_FAILURE);
	}
      }
      /* last command needs output to stdout, so we do not dup2(). */
      if (curcmd != (cmdcount - 1)) {
	if (dup2(pipefd[curcmd * 2 + 1], STDOUT_FILENO) == -1) {
	  perror("dup2() (pipe output) error ");
	  exit(EXIT_FAILURE);
	}
      }
      close(pipefd[(curcmd - 1) * 2]);
      close(pipefd[curcmd * 2 + 1]);
      res = execvp(**(commands + curcmd), commands[curcmd]);
      if (res == -1) {
	perror("execvp() error ");
	exit(EXIT_FAILURE);
      }
      
    } else {
      close(pipefd[(curcmd - 1) * 2]);
      close(pipefd[curcmd * 2 + 1]);
      wait(&status);
      if (status == -1) {
	perror("wait() error ");
      }
    }
  }
  for (int i = 0; i < cmdcount * 2; i++) {
    close(pipefd[i]);
  }
  return 0;
}
