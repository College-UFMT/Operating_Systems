#include <stdio.h> //para saida de texto para o terminal
#include <sys/types.h> //para fork() e wait()
#include <unistd.h>   //para fork() 
#include <stdlib.h> //para exit()
#include <sys/wait.h> //para wait()
#include <unistd.h>

int main()
{
  pid_t pid;

  pid = fork(); //o processo pai cria um processo filho

  if(pid == 0){  //processo filho
     sleep(2.0);
     execlp("/bin/ls","ls",NULL);
     printf("\nAula de SO"); //menssagem nao impressa. Por que?
  }else{
    int status;
    int ret_wait; 
    ret_wait = wait(&status);	
    printf("\nProcesso pai. Retorno de fork() = %d",pid);
    printf("\n Retorno de wait() %d status do retorno %d\n",ret_wait, status);
  }

}
