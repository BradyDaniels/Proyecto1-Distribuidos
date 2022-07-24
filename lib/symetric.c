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
     printf("la diferencia es de %f",diff_t);
     if(diff_t>=limit){
         return -1;
     }
     else 
        return 0;
}

char *time_to_string(time_t t){
   char *aux;
   char s[100];
   
   strftime(s,100,"%H:%M:%S",localtime(&t));
   printf("bandera tal\n");
   aux=s;
   return aux;

}

time_t string_to_time(char *s){
    
    struct tm ts={0};
    
    s[2]=s[5]="\0";
  
    ts.tm_hour=atoi(&s[0]);
    ts.tm_min=atoi(&s[3]);
    ts.tm_sec=atoi(&s[6]);

    time_t t=mktime(&ts);

    return t;

}



int main(){
    
    time_t start,end;
    struct tm tm_end={0},tm_start={0};
   
    int rc;
    printf("inicio...\n");
    time(&start);
    char *s=time_to_string(start);
    time_t t=string_to_time(s);
    
    /*
    strftime(s,100,"%H:%M:%S",localtime(&start));
    printf("El tiempo de inicio es: %s \n",s);
    s[2]=s[5]="\0";

    tm_end.tm_hour=atoi(&s[0])-1900;
    tm_end.tm_min=atoi(&s[3])-1;
    tm_end.tm_sec=atoi(&s[6]);

    time_t t=mktime(&tm_end);
    */
    sleep(5);
    time(&end);

  
    /*
    strftime(s,100,"%H:%M:%S",localtime(&end));
    printf("El tiempo de fin es: %s \n",s);

    s[2]=s[5]="\0";

    tm_start.tm_hour=atoi(&s[0])-1900;
    tm_start.tm_min=atoi(&s[3])-1;
    tm_start.tm_sec=atoi(&s[6]);

    time_t t2=mktime(&tm_start);
    */
    diff_time(t,end,1800);
  

    /*
      symetric_encript("test","admin");
      symetric_decript("test","admin");
    */
    return 0;

}