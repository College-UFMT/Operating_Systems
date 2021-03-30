#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
  //processo pai cria um pipe()
  int fd[2];
  if (pipe(fd) < 0)
  {
    printf("\nErro na criacao do pipe!");
    exit(0);
  }

  //processo pai cria o primeiro processo filho
  pid_t pid1;
  if ((pid1 = fork()) == -1)
  {
    printf("\nErro no primeiro fork!");
    exit(0);
  }

  if (pid1 == 0)
  {                 //filho 1
    close(fd[0]);   //fecha o descritor de leitura do pipe
    dup2(fd[1], 1); //redireciona a saida padrao para o pipe
    if (execl("/bin/ls", "ls", "-l", "-a", NULL) < 0)
    {
      printf("\nErro exec() processo 1");
      exit(0);
    }
  }
  else
  { //pai
    pid_t pid2;
    if ((pid2 = fork()) == -1)
    {
      printf("\nErro no segundo fork!");
      exit(0);
    }

    if (pid2 == 0)
    {                 //filho 2
      close(fd[1]);   //fecha o descritor de escrita do pipe
      dup2(fd[0], 0); //redireciona a entrada padrao para o pipe
      if (execl("/bin/grep", "grep", "Mar", NULL) < 0)
      {
        printf("\nErro exec() processo 2");
        exit(0);
      }
    }
    else
    { //pai
      close(fd[0]);
      close(fd[1]);
      int status;
      for (int i = 0; i < 15; i++)
      {
        wait(&status);
      }
    }
  }
  sleep(30.0);
  return 0;
}
