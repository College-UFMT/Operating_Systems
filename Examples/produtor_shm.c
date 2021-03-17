#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
  const int size = 4096;
  const char *name = "OS";
  int fd;
  char *ptr;
 
  const char *msg_0 = "Hello";

  fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if(fd < 0){
   printf("\nErro 1");
   exit(1); 
  }
  
  if(ftruncate(fd, size) < 0){
    printf("\nErro 2");
    exit(1);
  }

  ptr = (char*)mmap(0,size,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
  if(ptr == MAP_FAILED){
    printf("\nErro 3");
    exit(1);
  }  

  for(int i=0; i<10; i++) {
  	printf("%s\n","produzindo...");
 	sprintf(ptr,"%s",msg_0);
  	ptr += strlen(msg_0)+1;
  }
  
  sleep(30.0);
  printf("\n%s","produtor finalizando...");
  return 0;
}
