                                 
#ifndef _tcp_thead_server_h
#define _tcp_thead_server_h
#include <unistd.h>  
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include "finder.h"
#include "herramientas.h"
#include <errno.h>
#include <stdio.h> 
#include <string.h> 
#define BUF_SIZE    2048    
#define BACKLOG     5      

char public_user_key[20], privadakey[20], cmd[512], buff_as[BUF_SIZE],buff_as2[BUF_SIZE], 
buff_rx[BUF_SIZE];

void *connection_handler(void *p_connfd){
    int connfd = *((int*)p_connfd);
    free(p_connfd);
    int  len_rx, len_tx = 0;                    	          
    len_rx = read(connfd, buff_rx, sizeof(buff_rx));  // recibo 

    buffer_file(buff_rx); 
    desencriptado_servidores(nombrearchivo); 
    // aqui inserto funcion para extraer del archivo: ID, OPC, MENSAJE. 
      
    if(len_rx == -1){
        fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
        pausa();
        return NULL;
    }
    else{              

    printf("[CLIENT]:\n");
    printf("Mensaje encriptado: \n");
    system("cat archivorecibido.cifrado \n");
    printf("\nMensaje desencriptado: \n");
    system("\ncat archivorecibido.txt");

    strcpy(buff_as,"Hola1"); 
    strcpy(buff_as2,"\nhola2"); 
    write(connfd, buff_as, strlen(buff_as));     // envia
    write(connfd, buff_as2, strlen(buff_as2));

    // buff_tx[0]  = '\0'; // limpia buffer 
    system("rm archivorecibido.txt");
    system("rm archivorecibido.cifrado");
    system("rm archivorecibido.base64"); 

    printf("\n\n\n[SERVER]: Resultado\n");
    printf("%s \n %s",buff_as,buff_as2);
    printf("\n\n[SERVER]: socket closed \n\n");
    close(connfd); 
    }  
}

void runServer_tcp_t(int PORT){ 
    int sockfd, connfd;                         // descriptor para escucha y conxion 
    unsigned int len;                           // Tamaño de la direccion del cliente
    struct sockaddr_in servaddr, client;        // Socket address format for server and client
                            
    // creacion del socket del servidor 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1){ 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        pausa();
        return; 
    } 
    else{
        printf("\n[SERVER]: Socket successfully created\n"); 
    }
    // vinculo la direccion IP y puerto local al socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port        = htons(PORT); 
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0){ 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
    	pausa();
        return;
    } 
    else{
        printf("[SERVER]: Socket successfully binded \n");
    }
    // escucha de clientes 
    if ((listen(sockfd, BACKLOG)) != 0){ 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        pausa();
        return; 
    } 
    else{
        printf("[SERVER]: Listening on SERV_PORT %d \n\n", ntohs(servaddr.sin_port) ); 
    }
    // ciclo de espera de clientes
    while(1){
        // aceptar  cliente
        len = sizeof(client); 
        connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
        if (connfd < 0){ 
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
            pausa();
            return; 
        } 
        else{
            // manejador de conexiones    
            pthread_t hilo;
            int *p_connfd = malloc(sizeof(int));
            *p_connfd = connfd;
            pthread_create(&hilo, NULL, connection_handler, p_connfd);                                         
        }                  
    }  
}

#endif
