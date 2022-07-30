                                 
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
#include <time.h>
#define BUF_SIZE    2048    
#define BACKLOG     5      

char public_user_key[20], privadakey[20], cmd[512],  buff_rx[BUF_SIZE], clavesesion [50],id_tgs[20],
file_cliente[25], timestampC[50], lifetimeC[10],idprueba[20], ipuser[20], p1[BUF_SIZE], p2[10]; 
//buff_as[BUF_SIZE],buff_as2[BUF_SIZE],
time_t timestamp; 
int lifetime = 60; 
int auxi=0; // 60 sgundos de tiempo d vida 


void *connection_handler(void *p_connfd){
  
    buff_rx[0]='\0'; 
    int connfd = *((int*)p_connfd);
    free(p_connfd); // ??
    int  len_rx, len_tx = 0;   
    strcpy(id_tgs,"TgsUcab"); 



                     	          
    len_rx = read(connfd, buff_rx, sizeof(buff_rx));  // recibo 


    if(buscarDatos("publica",buff_rx,NULL,0) != NULL)
    { 
        printf ("ENtra n el srvidor pq encontro publica"); 
        //pausa(); 
        registrarUsuario(buff_rx); 
        buff_rx[0] = '\0';
    }
    else 
    { 

    
    buffer_file(buff_rx);   
    desencriptado_servidores(nombrearchivo); 
 
    buff_rx[0] = '\0';
    file_buffer("archivorecibido.txt"); // FL_UPDATE_FILE_NAME
    // no se debe vaciar la estructura ? 
    copiarMensajeClienteAS(FL_UPDATE_FILE_NAME); // m dvulv una structura con los paramtros

      
    if(len_rx == -1){
        fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
        pausa();
        return NULL;
    }
    else{ 
          
           
           if (auxi ==0){
            strcpy(idprueba,mcas->ID);
            strcpy(ipuser,mcas->pcliente); 
           }
           printf("\nvariable idprueba: %s",idprueba);

        if(validarExistenciaUsuario(idprueba)==1){ // aqui esta el error 
        
        auxi++; 
               

            printf("[CLIENT]:\n");
            printf("Mensaje encriptado: \n");
            system("cat archivorecibido.cifrado \n");
            printf("\nMensaje desencriptado: \n");
            system("\ncat archivorecibido.txt");
            strcpy(clavesesion,generate_key(8));

            // contruccion buffer 1 AS-CLIENTE
            time_t timestamp; 
            strcpy(timestampC,time_to_string(timestamp)); // conv time a char
            sprintf(lifetimeC,"%d",lifetime); // conv int a char
            strcpy(buff_as,asCliente1(id_tgs,timestampC,lifetimeC,clavesesion));  
            printf("\n\nbuffer antes de encriptar: %s",buff_as); 
            encriptado_keypublic(buff_as,idprueba); //mcas->ID
            strcpy(file_cliente,"archivooriginal.base64");
            FL_UPDATE_FILE_NAME[0]='\0';
            buff_as[0]='\0'; 
            file_buffer(file_cliente); 

            strcpy(buff_as,FL_UPDATE_FILE_NAME); // BUFER 1 CONSTRUIDO 
          //  write(connfd, buff_as, strlen(buff_as)); //E nviado 
            FL_UPDATE_FILE_NAME[0]='\0';
            
            // construccion buffr 2 AS-CLIENTE 
            buff_as2[0]='\0';
            strcpy(buff_as2,asCliente2(idprueba,id_tgs,timestampC,ipuser,lifetimeC,clavesesion));  
            printf("\n\nBuffer 2 antes de encriptar: %s",buff_as2); 
            encriptado_keypublic(buff_as2,"tgs_publica"); //encriptado con la lalve del TGS
            strcpy(file_cliente,"archivooriginal.base64");
            FL_UPDATE_FILE_NAME[0]='\0';
            buff_as2[0]='\0'; 
            file_buffer(file_cliente); 

            strcpy(buff_as2,FL_UPDATE_FILE_NAME); // BUFER 2 CONSTRUIDO 

            printf("\nB1:\n%s\n\nB2:\n%s",buff_as,buff_as2);

            strcat(buff_as,"\n"); 
            strcat(buff_as,buff_as2); 


           // strcat(buff_as,"\n");
           // strcat(buff_as, buff_as2); 
           
            write(connfd, buff_as, strlen(buff_as));
           // write(connfd2, buff_as2, strlen(buff_as2)); //E nviado 

           // para rcibir CLIENT - TGS 
           len_rx =0; 
           buff_rx[0]='\0';
           len_rx = read(connfd, buff_rx, sizeof(buff_rx)); // Rcibo cliente - tgs 
           // funcion para guardar en 3 cadenas 

           // desncriptar cadena 1, guardar en variables.. 
          

             id_tgs[0]='\0';

        }else{
        fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
        pausa();
        return NULL;
        }           





//----------------------------------------------------

  
    system("rm archivorecibido.txt");
    system("rm archivorecibido.cifrado");
    system("rm archivorecibido.base64"); 

    printf("\n\n\n[SERVER AS]: Enviado\n");
   // printf("%s \n %s",buff_as,buff_as2); 
    printf("\n%s",buff_as); 
   // printf("\n\nBuffer 2:\n%s",buff_as2); 
    printf("\n\n[SERVER]: socket closed \n\n");
    buff_as[0]='\0';
    close(connfd); 



    }


    }
 
   // int connfd2 = *((int*)p_connfd);
  //  free(p_connfd); // ??
   // int  len_rx2, len_tx2 = 0; 
   // write(connfd2, buff_as2, strlen(buff_as2));
   // close(connfd2);

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
