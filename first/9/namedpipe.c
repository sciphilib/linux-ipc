#include <unistd.h> // pid_t
#include <assert.h> // assert
#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdio.h> // perror
#include <sys/wait.h> // wait
#include <sys/stat.h> // mkfifo
#include <fcntl.h> // O_WRONLY, O_RDONLY

#define FIFOPATH1 "fifo1"
#define FIFOPATH2 "fifo2"

int main(int argc, char** argv) {
  /* fifo file desciptor. */
  int wr_fifo, rd_fifo, num;
  pid_t pid;

  if (argc < 2) {
    printf("Usage: ./program random_num_count\n");
    exit(EXIT_FAILURE);
  }

  /* delete if already exist. */
  unlink(FIFOPATH1);
  unlink(FIFOPATH2);
  
  /* create pipes. */
  if (mkfifo(FIFOPATH1, S_IRWXU) == -1) {
    perror("mkfifo() error ");
    exit(EXIT_FAILURE);
  }  
  if (mkfifo(FIFOPATH2, S_IRWXU) == -1) {
    perror("mkfifo() error ");
    exit(EXIT_FAILURE);
  }
  
  pid = fork();
  if (pid == -1) {
    perror("fork() error ");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    if ((wr_fifo = open(FIFOPATH2, O_WRONLY)) == -1) {
      perror("In child process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }

    /* write random numbers to pipe. */
    for (int i = 0; i < atoi(argv[1]); i++) {
      num = rand() % 10;
      if (write(wr_fifo, &num, sizeof(int)) < 1) {
	perror("In child process:\nfwrite() error ");
	exit(EXIT_FAILURE);
      }
    }

    if (close(wr_fifo) == -1) {
      perror("In child process:\n close() error");
    }
    
    if ((rd_fifo = open(FIFOPATH1, O_RDONLY)) == -1) {
      perror("In child process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }

    /* read rand number * 2 from pipe. */
    printf("Child procces read: ");
    while (read(rd_fifo, &num, sizeof(int)) > 0) {
      printf("%d ", num);
    }
    printf("\n");
    
    if (close(rd_fifo) == -1) {
      perror("In child process:\n close() error");
    }
    
    exit(EXIT_SUCCESS);
    
  } else {

    if ((rd_fifo = open(FIFOPATH2, O_RDONLY)) == -1) {
      perror("In parent process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }

    int nums[atoi(argv[1])];
    int counter = 0;
    /* read random number from pipe. */
    printf("Parent process read: ");
    while (read(rd_fifo, &num, sizeof(int)) > 0) {
      /* multiple number by 2 and write it to an another pipe. */
      printf("%d ", num);
      num = num * 2;
      nums[counter++] = num;
    }
    printf("\n");

    if(close(rd_fifo) == -1) {
      perror("In parent process:\n close() error");
    }

    if ((wr_fifo = open(FIFOPATH1, O_WRONLY)) == -1) {
      perror("In parent process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < atoi(argv[1]); i++) {
      if (write(wr_fifo, &nums[i], sizeof(int)) < 1) {
	perror("In parent process:\nfwrite() error ");
	exit(EXIT_FAILURE);
      }
    }
    
    if(close(wr_fifo) == -1) {
      perror("In parent process:\n close() error");
    }

    wait(NULL);
    exit(EXIT_SUCCESS);
  }
  
  return 0;
}
