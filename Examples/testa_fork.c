#include <stdio.h> //para saida de texto para o terminal
#include <sys/types.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  pid = fork(); //o processo pai cria um processo filho

  if(pid == 0){  //processo filho
	while(1){
           printf("\nProcesso filho. Retorno de fork() = %d",pid);
           sleep(1.0);     
	}
  }else{
	while(1){  
          printf("\nProcesso pai. Retorno de fork() = %d",pid);
	  sleep(1.0);
	}
  }

}
