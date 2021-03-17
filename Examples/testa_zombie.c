#include <stdio.h> //para saida de texto para o terminal
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  pid_t pid;

  pid = fork(); //o processo pai cria um processo filho

  if(pid == 0){  //processo filho
    printf("\nProcesso filho. Retorno de fork() = %d",pid);
    exit(0);    
  }else{
    for(int i=0; i<60; i++){  
       printf("\nProcesso pai. Retorno de fork() = %d",pid);
       sleep(1.0);
   }
   int status;
   wait(&status);
  }
}
