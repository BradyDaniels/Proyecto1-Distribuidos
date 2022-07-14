                                 
#ifndef _tcp_thead_client_h
#define _tcp_thead_client_h
#include <unistd.h>  
#include <stdlib.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <stdio.h> 
#include <string.h> 
#include "finder.h"
#include "herramientas.h"
#include <pthread.h>
#include <errno.h>
#define BUF_SIZE    2048
#define KEY_SIZE    20

char FL_UPDATE_FILE_NAME[BUF_SIZE], private_user_key[KEY_SIZE], public_user_key[KEY_SIZE], 
publickey[KEY_SIZE],  cadena[30], cmd[512], file_cliente[30], publickey2[KEY_SIZE], file_cliente2[30];


void *client_tcp_t(void *args){
                       
    Conexion *conexion = (Conexion *)args;
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1){ 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        pausa();
        return NULL;
    } 
    else{
        printf("[CLIENT]: Socket successfully created\n"); 
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(conexion->IP_SERVER);
    servaddr.sin_port        = htons(conexion->PORT); 
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){ 
        printf("[SERVER-error] connection with the server failed\n");  
        pausa();
        return NULL;
    } 
    else{
    	printf("[CLIENT]: connected to SERVER %s\n",conexion->IP_SERVER); 
    }

    printf(" Este es el archivo que se envia al servidor: \n"); 
    printf("%s",conexion->message);
    write(sockfd, conexion->message, sizeof(conexion->message));  //envia
    pausa();
    int n = read(sockfd, buf_as, BUF_SIZE); // recibe
    int n2 = read(sockfd, buf_as2, BUF_SIZE); 
    
   // buffer_file();
    
    buf_as[n]='\0'; 
    if (n==0){
        printf("\nNo se hallaron resultados en el servidor de IP: %s",conexion->IP_SERVER);
    }
    else {
        system("clear");
        printf("\n[SERVER %s] resultado: \n%s",conexion->IP_SERVER,buf_as);
    }

    printf("\n[CLIENT]: socket cerrado \n\n");
    close(sockfd); 
}


void runClient_tcp_t(int port, char *address, char *message){
	Conexion conexion; 
	pthread_t hilo;
	conexion.PORT = port;
	strcpy(conexion.IP_SERVER, address);

    // si message tiene publica { registro(message); }
    //

    strcpy(publickey,"as_publica"); 
    encriptado_keypublic(message,publickey); 
    system("clear");

    printf("Comunicacion CLIENTE ---> AS \n\nMensaje original:\n");
    system("cat archivooriginal.txt");

	printf("\nEste es el mensaje encriptado con la llave publica:  \n");
    system("cat archivooriginal.cifrado \n");

	printf("\n\nEste es el mensaje codificado con base 64:  \n");
    system("cat archivooriginal.base64 \n"); // este es el que se debe guardar en el buffer 

    pausa();
    printf("\n\nPresiona enter para continuar..\n");
    pausa();
    
    strcpy(file_cliente,"archivooriginal.base64"); 

    file_buffer(file_cliente);
    strcpy(conexion.message, FL_UPDATE_FILE_NAME); 
	pthread_create(&hilo, NULL, client_tcp_t, (void *)&conexion);
	pthread_join(hilo, NULL);

    FL_UPDATE_FILE_NAME[0] = '\0'; // para eliminar contenido de variable 
    publickey[0] = '\0'; 
    system("rm archivooriginal.txt");
    system("rm archivooriginal.cifrado");
    system("rm archivooriginal.base64"); 


}


char *generar_llaves(){
    char cmd[512];
    char cmd2[512];
        printf("\nIngrese nombre de llave publica : ");
        scanf("%s", &public_user_key);       
        printf("\nIngrese nombre de llave privada : ");
        scanf("%s", &private_user_key);
       
        sprintf(cmd,"openssl rsa -in semilla.pem -des3 -out %s", strcat(private_user_key,".pem"));
        sprintf(cmd2,"openssl rsa -in semilla.pem -pubout -out %s", strcat(public_user_key,".pem"));

        system("openssl genrsa -out semilla.pem 2048"); //genera semilla 
        system(cmd);
        system(cmd2);
        system("rm -rf semilla.pem");
return public_user_key;        
}



#endif
