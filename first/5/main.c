#include <stdio.h>
#include <unistd.h> // exec*

#define BUFSIZE 256
#define ARGSCOUNT 10

void parse(char* buf, char** args) {
  int begp = 0, endp = 0;
  int curarg = 0;
  int isWord = 0, isFirstWord = 1;
  
  while (buf[endp] != '\n') {
    if (buf[endp] != ' ') {
      if (!isFirstWord && !isWord)
	begp = endp;
      isWord = 1;
    }
    if (buf[endp] == ' ' && isWord) {
      buf[endp] = '\0';
      args[curarg] = &buf[begp];
      curarg++;
      isWord = isFirstWord = 0;
    }
    endp++;
  }
  /* check if last argument don't have whitespace after his end. */
  if (isWord) {
    buf[endp] = '\0';
    args[curarg++] = &buf[begp];
  }
  args[curarg] = NULL;
}

int main() {
  printf("Welcome! Enter the program name to run and its arguments:\n");
  printf("Usage: program_name arg1 arg2 arg3 ... argn\n");

  char buf[BUFSIZE];
  char* args[ARGSCOUNT];
  fgets(buf, BUFSIZE, stdin);
  parse(buf, args);

  int res;
  res = execvp(args[0], args);
  if (res == -1) {
    perror("execvp() failed!\n");
  }
  
  return 0;
}
