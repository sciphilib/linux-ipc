#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  // get process id 
  pid_t pid = getpid();
  printf("pid: %d\n", pid); // 4681

  // get parent process id
  pid_t ppid = getppid();
  printf("ppid: %d\n", ppid); // 4681

  // get user id
  pid_t uid = getuid();
  printf("uid: %d\n", uid); // 4681

  // get effective user id
  pid_t euid = geteuid();
  printf("euid: %d\n", euid); // 4681

  // get group id
  pid_t gid = getgid();
  printf("gid: %d\n", gid);

  // get session id
  pid_t sid = getsid(0);
  if (sid == -1) {
    perror("getsid() failed\n");
    exit(EXIT_FAILURE);
  } else {
    printf("sid: %d\n", sid);
  }

  return 0;
}
