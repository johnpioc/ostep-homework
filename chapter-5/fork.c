#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Question 1
void fork_and_add() {
  int x = 100;
  pid_t childPid = fork();
  if (childPid == 0) {
    x += 50;
    printf("Value of x after adding 50 in the child: %d\n", x);
    exit(0);
  } else {
    x += 50;
    printf("Value of x after adding 50 in the parent: %d\n", x);

    int status;
    wait(&status);
  }
}

// Question 2
void open_same_file() {
  int fileDescriptor = open("testfile", O_RDWR);

  pid_t childPid = fork();
  char message[] = "This message should be appended\n.";
  int messageLen = strlen(message);

  if (childPid == 0) {
    int writeStatus = write(fileDescriptor, message, messageLen);

    if (writeStatus == -1) {
      printf("Writing to the file in the child was unsucessful.\n");
    } else
      printf("Writing to the file in the child was sucessful.\n");

    exit(0);
  } else {
    int status;
    wait(&status);

    int writeStatus = write(fileDescriptor, message, messageLen);

    if (writeStatus == -1) {
      printf("Writing to the file in the parent was unsuccessful.\n");
    } else
      printf("Writing to the file in the parent was successful.\n");
  }

  fflush(stdout);
  close(fileDescriptor);
}

// Question 3
void print_hello_goodbye() {
  pid_t childPid = fork();

  if (childPid == 0) {
    printf("Hello\n");
    exit(0);
  } else {
    sleep(1);
    printf("Goodbye\n");

    int status;
    wait(&status);
  }
}

// Question 4
void fork_and_exec_bin_ls() {
  // execl()
  pid_t childPid1 = fork();

  if (childPid1 == 0) {
    execl("/bin/ls", "/bin/ls", (char *)NULL);
  } else {
    int status;
    wait(&status);
  }

  fflush(stdout);

  // execle()
  pid_t childPid2 = fork();

  if (childPid2 == 0) {
    char *newEnv[] = {"USER=johnpioc", "DEBUG=1", "NUMOFBURGERS=2",
                      "PATH=/usr/bin:/bin", NULL};
    execle("/bin/ls", "ls", (char *)NULL, newEnv);
    perror("execle");
    exit(1);
  } else {
    int status;
    wait(&status);
  }

  fflush(stdout);

  // execlp()
  pid_t childPid3 = fork();

  if (childPid3 == 0) {
    execlp("/bin/ls", "/bin/ls", (char *)NULL);
  } else {
    int status;
    wait(&status);
  }

  fflush(stdout);

  // execv()
  pid_t childPid4 = fork();

  if (childPid4 == 0) {
    char *args[] = {"/bin/ls", NULL};
    execv("/bin/ls", args);
  } else {
    int status;
    wait(&status);
  }

  fflush(stdout);

  // execvp()
  pid_t childPid5 = fork();

  if (childPid5 == 0) {
    char *args[] = {"/bin/ls", NULL};
    execvp(args[0], args);
  } else {
    int status;
    wait(&status);
  }

  fflush(stdout);
}

// Question 6
void close_stdout_then_print() {
  pid_t childPid = fork();

  if (childPid == 0) {
    close(STDOUT_FILENO);
    printf("This shouldn't print\n");
    exit(0);
  } else {
    int status;
    wait(&status);
  }
}

// Question 7
void pipe_two_children() {
  int fd[2];
  pipe(fd); // 0 for reading, 1 for Writing
  pid_t childPids[2];

  for (int i = 0; i < 2; i++) {
    pid_t childPid = fork();

    if (childPid == 0) {
      if (i == 0) {
        close(fd[0]); // Close reading file file descriptor
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        printf("This message should be printed in child 2\n");
        exit(0);
      } else {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        char *line = NULL;
        size_t size = 0;
        ssize_t lineLength;

        printf("Printing in child 2\n");
        while ((lineLength = getline(&line, &size, stdin)) != -1) {
          printf("Child 2 got: %s\n", line);
        }

        free(line);
        exit(0);
      }
    } else {
      childPids[i] = childPid;
      if (i == 1) {
        close(fd[0]);
        close(fd[1]);
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    int status;
    waitpid(childPids[i], &status, 0);
  }
}

int main(int argc, char *argv[]) {

  // Question 1
  fork_and_add();

  // Question 2
  open_same_file();

  // Question 3
  print_hello_goodbye();

  // Question 4
  fork_and_exec_bin_ls();

  // Question 6
  close_stdout_then_print();

  // Question 7
  pipe_two_children();

  return 0;
}
