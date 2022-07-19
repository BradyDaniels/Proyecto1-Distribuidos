#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int symetric_encript(char *s,char *p) {
  char *sen="openssl enc -aes-256-cbc -md sha512 -pbkdf2 -iter 10000 -salt -in ";
  char *sen2=".txt -out ";
  char *sen3=".enc -pass  pass:";
  char buffer[100],buffer2[100],buffer3[100],buffer4[100],buffer5[100];

  strcat(strcpy(buffer,sen),s);
  strcat(strcpy(buffer2,buffer),sen2);
  strcat(strcpy(buffer3,buffer2),s);
  strcat(strcpy(buffer4,buffer3),sen3);
  strcat(strcpy(buffer5,buffer4),p);

  //system("openssl enc -aes-256-cbc -md sha512 -pbkdf2 -iter 10000 -salt -in test.txt -out test.enc -pass  pass:admin");
  system(buffer5);
  return 0;
}

int symetric_decript(char *s,char *p){
  char *sen="openssl enc -aes-256-cbc -md sha512 -pbkdf2 -in ";
  char *sen2=".enc -out ";
  char *sen3="_clear.txt -d -pass  pass:";
  char buffer[100],buffer2[100],buffer3[100],buffer4[100],buffer5[100];

  strcat(strcpy(buffer,sen),s);
  strcat(strcpy(buffer2,buffer),sen2);
  strcat(strcpy(buffer3,buffer2),s);
  strcat(strcpy(buffer4,buffer3),sen3);
  strcat(strcpy(buffer5,buffer4),p);
  
  //system("openssl enc -aes-256-cbc -md sha512 -pbkdf2 -in test.enc -out test_clear.txt -d -pass pass:admin");
  system(buffer5);
   return 0;
}

int diff_time(time_t start,time_t end,int limit){

     double diff_t=difftime(end,start);
     if(diff_t>=limit){
         return -1;
     }
     else 
        return 0;
}


int main(){
    /*
    time_t start,end;
    printf("inicio...\n");
    time(&start);

    printf("Espera 5 segundos");
    sleep(5);

    time(&end);

    double diff_t=difftime(end,start);
    printf("Tiempo de ejecucion: %f\n",diff_t);
    */
    symetric_encript("test","admin");
    symetric_decript("test","admin");

    return 0;

}