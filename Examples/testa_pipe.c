#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  int fd[2];

  //cria um pipe
  if(pipe(fd) == -1){
    printf("\nErro na criacao do pipe");
    exit(1);
  }

  //cria o processo filho, que herda os decritores de arquivo do proc pai
  pid_t pid;
  if((pid = fork()) == -1){
    printf("\nErro no fork()");
    exit(1);
  }

  if(pid == 0){ //processo filho entra aqui
    char buff[20];
    close(fd[1]); //fecha o descritor de escrita
    while(1){
      read(fd[0], buff, 11);
      printf("\nProcesso filho leu: %s",buff);
      sleep(0.5);
    }
  }else{ //processo pai entra aqui
    close(fd[0]); //fecha o descritor de leitura
    char* msg = "Ola mundo!";
    while(1){
      write(fd[1], msg, 11);
      printf("\nProcesso pai escreveu no pipe");    
      sleep(1.0);
    }
  }
}
