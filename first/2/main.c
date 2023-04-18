#include <sys/types.h> // fork
#include <unistd.h> // fork, pid_t, getpid, getppid
#include <stdlib.h> // EXIT_FAILURE, atexit, exit
#include <stdio.h> // perror
#include <sys/wait.h> // wait

long atexit_max;

void print_args(int argc, char** argv);
void normal_exit_callback(void);
void a_normal_exit_callback(void);

int main(int argc, char** argv)
{
  atexit_max = sysconf(_SC_ATEXIT_MAX);
  pid_t pid;
  int exitval;
  
  switch (pid = fork()) {
  case -1:
    perror("fork() failed!\n");
    exit(EXIT_FAILURE);
  case 0:
    print_args(argc, argv);
    exit(EXIT_SUCCESS);
  default:
    print_args(argc, argv);
    wait(&exitval);
    if (atexit(normal_exit_callback)) {
      printf("atexit() failed!\n");
    }
    if (atexit(a_normal_exit_callback)) {
      printf("atexit() failed!\n");
    }
  }
  exit(EXIT_SUCCESS);
}

void print_args(int argc, char** argv) {
  for (int i = 0; i < argc; i++) {
    int j = 0;
    while (argv[i][j]) {
      putchar(argv[i][j]);
      ++j;
    }
    printf(" ");
  }
  printf("\n");
}

void normal_exit_callback(void) {
  printf("Registered function \"normal_exit_callback\". You have %ld functions left.\n", --atexit_max);
  printf("The child process was normally terminated via exit() or a return from main.\n");
}

void a_normal_exit_callback(void) {
  printf("Registered function \"a_normal_exit_callback\". You have %ld functions left.\n", --atexit_max);
  printf("The child process was normally terminated via exit() or a return from main.\n");
}
