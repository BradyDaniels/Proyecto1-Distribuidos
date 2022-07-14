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
