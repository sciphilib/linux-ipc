#include <unistd.h> // pid_t
#include <assert.h> // assert
#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdio.h> // perror
#include <sys/wait.h> // wait
#include <sys/stat.h> // mkfifo

#define FIFOPATH1 "fifo1"
#define FIFOPATH2 "fifo2"

int main(int argc, char** argv) {
  /* fifo file desciptor. */
  int num;
  FILE *rd_stream, *wr_stream;
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
    if ((wr_stream = fopen(FIFOPATH2, "w")) == NULL) {
      perror("In child process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }

    /* write random numbers to pipe. */
    for (int i = 0; i < atoi(argv[1]); i++) {
      num = rand() % 10;
      if (fwrite(&num, sizeof(int), 1, wr_stream) < 1) {
	perror("In child process:\nfwrite() error ");
	exit(EXIT_FAILURE);
      }
    }

    fclose(wr_stream);
    
    if ((rd_stream = fopen(FIFOPATH1, "r")) == NULL) {
      perror("In child process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }
    
    /* read rand number * 2 from pipe. */
    printf("Child process read: ");
    while (fread(&num, sizeof(int), 1, rd_stream) > 0) {
      printf("%d ", num);
    }
    printf("\n");

    fclose(rd_stream);

    exit(EXIT_SUCCESS);
    
  } else {
    if ((rd_stream = fopen(FIFOPATH2, "r")) == NULL) {
      perror("In parent process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }
    
    int nums[atoi(argv[1])];
    int counter = 0;
    
    /* read random number from pipe. */
    printf("Parent process read: ");
    while (fread(&num, sizeof(int), 1, rd_stream) > 0) {
      /* multiple number by 2 and write it to another pipe. */
      printf("%d ", num);
      nums[counter++] = num * 2;
    }
    printf("\n");

    fclose(rd_stream);

    if ((wr_stream = fopen(FIFOPATH1, "w")) == NULL) {
      perror("In parent process:\nfopen() error ");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < atoi(argv[1]); i++) {
      if (fwrite(&nums[i], sizeof(int), 1, wr_stream) < 1) {
	perror("In parent process:\nfwrite() error ");
	exit(EXIT_FAILURE);
      }
    }
    
    fclose(wr_stream);

    wait(NULL);            
    exit(EXIT_SUCCESS);
  }
  
  return 0;
}
