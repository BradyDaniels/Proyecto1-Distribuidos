#ifndef SERIALIZACION_H
#define SERIALIZACION_H
//librerias
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio_ext.h>
#include <fcntl.h>
#include <pthread.h>
#define MAX_CHAR 255
#define BUF_SIZE    2048

char FL_UPDATE_FILE_NAME[BUF_SIZE], buf_as[BUF_SIZE]=" ", buf_as2[BUF_SIZE]=" ",
cadena[30], cmd[512], publickey[20], nombrearchivo[30];
//definicion de las estructuras
typedef struct direccion{
  char address[MAX_CHAR];
  struct direccion *next;
} Direccion;

typedef struct conexion{
  int PORT;
  char IP_SERVER[255];
  char message[2048]; // instruccion 
} Conexion;

Direccion *newDireccion(char *address){
  Direccion *ptr = (Direccion *)malloc(sizeof(Direccion));
  if (ptr != NULL)
  {
    strcpy(ptr->address, address);
    ptr->next = NULL;
  }
}

Direccion *addDireccion(Direccion *listp, Direccion *newp){
  Direccion *p;
  if (listp == NULL)
    return newp;
  for (p = listp; p->next != NULL; p = p->next)
    ;
  p->next = newp;
  return listp;
}

int cleanBuffer(){
  int fdflags;
  fdflags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, fdflags | O_NONBLOCK);
  while (getchar() != EOF);
  fcntl(STDIN_FILENO, F_SETFL, fdflags);
}

void pausa(){
  while (getchar() != '\n');
}

void encriptado_keypublic(char *message, char *publickey){
    
    FILE* fichero;
    fichero = fopen("archivooriginal.txt", "wt");
    fputs(message, fichero);
    fclose(fichero); 
    sprintf(cmd,"openssl rsautl -encrypt -in archivooriginal.txt -inkey %s -pubin -out archivooriginal.cifrado",strcat(publickey,".pem"));
    system(cmd); // encriptando el archivo original con la llave publica 
    system("openssl enc -base64 -in archivooriginal.cifrado -out archivooriginal.base64"); // codificacion del archivo encripado con base 64
    
    

}

// cambiar el nombre del archivo a recibir *file_cliente
char *file_buffer(char *file){
    FILE *pFile;
    pFile=fopen(file,"r");
    if(pFile!=NULL){      
        while(!feof(pFile)){
            fgets(cadena,30,pFile);
            if(feof(pFile)) break;          
            strcat(FL_UPDATE_FILE_NAME,cadena);
        }
    }
    fclose(pFile);

return FL_UPDATE_FILE_NAME; 
}
//hace una des-encriptacion simetrica solicitando el nombre de un archivo .txt a encriptar (s) y la palabra clave (p)
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
//hace una des-encriptacion simetrica solicitando el nombre de un archivo .txt a encriptar (s) y la palabra clave (p)
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
//funcion que dada dos marcas de tiempo retorna la -1 si la diferencia entre ambas supera el limite(limit)
int diff_time(time_t start,time_t end,int limit){

     double diff_t=difftime(end,start);
     if(diff_t>=limit){
         return -1;
     }
     else 
        return 0;
}

//Dado una variable time_t retorna un string con el formato HH:MM:SS
char *time_to_string(time_t t){
   char *aux;
   char s[100];
   
   strftime(s,100,"%H:%M:%S",localtime(&t));
   printf("bandera tal\n");
   aux=s;
   return aux;

}

//Dado un char con el formato HH:MM:SS retorna una variable time_t

time_t string_to_time(char *s){
    
    struct tm ts={0};
    
    s[2]=s[5]="\0";
  
    ts.tm_hour=atoi(&s[0]);
    ts.tm_min=atoi(&s[3]);
    ts.tm_sec=atoi(&s[6]);

    time_t t=mktime(&ts);

    return t;

}



//cambiar el nombre del archivo a recibir
void buffer_file(char *buff_rx_as){
    FILE *fichero2; 
    fichero2 =fopen("archivorecibido.base64","wt");
    strcpy(nombrearchivo,"archivorecibido.base64");
    fputs(buff_rx_as,fichero2); 
    fclose(fichero2);
}

void desencriptado_servidores(char *file2){

  sprintf(cmd,"openssl enc -base64 -d -in %s -out archivorecibido.cifrado",file2);
  system(cmd);
  system("openssl rsautl -decrypt -inkey as_privada.pem -in archivorecibido.cifrado -out archivorecibido.txt -passin pass:admin");
    // quizas se modifique el argumento de llave privada y la clave para acceder.   
}
// funcion para servidores 

void registro(char *message){
    FILE* fichero;
    fichero = fopen("archivoregistro.txt", "wt");
    fputs(message, fichero);
    fclose(fichero); 
    system("openssl enc -base64 -in archivoregistro.txt -out archivoregistro.codificado"); // codificacion del archivo encripado con base 64
    file_buffer("archivoregistro.codificado"); 
    //system("rm archivoregistro.txt");
   // system("rm archivoregistro.codificado");
    // printf("Esto es lo que tiene el buffer: %s",FL_UPDATE_FILE_NAME); 

}
#endif
