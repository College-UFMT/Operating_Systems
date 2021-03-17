#include <stdio.h>
#include <stdlib.h>
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

  fd = shm_open(name, O_RDONLY, 0666);
  if(fd < 0){
    printf("\nErro 1");
    exit(0);
  }
  
  ptr = (char*)mmap(0,size,PROT_READ, MAP_SHARED, fd,0);
  if(ptr == MAP_FAILED){
    printf("\nErro 2");
    exit(0);
  }
   
  for(int i=0; i<10; i++){
    printf("\nConsumindo %s",ptr);
    ptr += 6;
    sleep(1.0);
  } 
  shm_unlink(name);
}
