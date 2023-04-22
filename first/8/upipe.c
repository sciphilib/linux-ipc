#include <unistd.h> // pid_t
#include <assert.h> // assert
#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdio.h> // perror
#include <sys/wait.h> // wait

int main(int argc, char** argv) {
  int pipefd[2];
  pid_t pid;
  char buf;

  if (argc < 2) {
    printf("Usage: ./program random_num_count\n");
    exit(EXIT_FAILURE);
  }

  if (pipe(pipefd) == -1) {
    perror("pipe() error ");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork() error ");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    /* close unused read end. */
    close(pipefd[0]);
    int rnum = rand();

    for (int i = 0; i < atoi(argv[1]); i++) {
      write(pipefd[1], &rnum, sizeof(int));
    }
    
    /* parent will see EOF. */
    close(pipefd[1]);
    exit(EXIT_SUCCESS);
    
  } else {
    /* close unused write end. */
    close(pipefd[1]);
    int rnum;
    
    while(read(pipefd[0], &rnum, sizeof(int)) > 0) {
      printf("%d ", rnum);
    }
    printf("\n");
    
    close(pipefd[0]);
    wait(NULL);
    exit(EXIT_SUCCESS);
  }
  
  return 0;
}
