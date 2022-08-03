                                 
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

char public_user_key[20], privadakey[20], cmd[BUF_SIZE],  buff_rx[BUF_SIZE], clavesesion [50],id_tgs[20],
file_cliente[25], timestampC[50], lifetimeC[10], lifetimeCTGS[10],idprueba[20], ipuser[20], p1[BUF_SIZE], p2[10],
buf1tgs[BUF_SIZE], buf2tgs[BUF_SIZE], buf3tgs[BUF_SIZE], clavesesionTGS[50], nuevo[BUF_SIZE],
cadena20[300], cadena21[300], clavesesion_ss[50], busqueda[500], timestampC2[50];
//buff_as[BUF_SIZE],buff_as2[BUF_SIZE],
// time_t timestamp; 
int lifetime = 60; 
int lifetimeSS = 60;
int auxi=0; // 60 sgundos de tiempo d vida 
int port2 = 0, tgs =0; 
time_t timestamp, timestampTGS;



void *connection_handler(void *p_connfd){
  
    buff_rx[0]='\0'; 
    int connfd = *((int*)p_connfd);
    free(p_connfd); // ??
    int  len_rx, len_tx = 0;   
    strcpy(id_tgs,"TgsUcab"); 
          
if(tgs==0)
{
         
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
          // printf("\nvariable idprueba: %s",idprueba);

        if(validarExistenciaUsuario(idprueba)==1){ // aqui esta el error 
        
        auxi++; 
               

            printf("[CLIENT]:\n");
            printf("Mensaje encriptado: \n");
            system("cat archivorecibido.cifrado \n");
            printf("\nMensaje desencriptado: \n");
            system("\ncat archivorecibido.txt");
            strcpy(clavesesion,generate_key(8));

            // contruccion buffer 1 AS-CLIENTE
            
            time(&timestamp);
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

            write(connfd, buff_as, strlen(buff_as));
 
             id_tgs[0]='\0';

        }else{
        fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
        pausa();
        return NULL;
        }           


  
    system("rm archivorecibido.txt");
    system("rm archivorecibido.cifrado");
    system("rm archivorecibido.base64"); 

    printf("\n\n\n[SERVER AS]: Enviado\n");
    printf("\n%s",buff_as); 
    printf("\n\n[SERVER]: socket closed \n\n");
    buff_as[0]='\0';
    tgs = 1;
    close(connfd); 

    }
    }
}else{
    if (tgs == 1)
    {
       // printf("\nENTRA A LA CONDICION PARA LA RCEPCION DL SGUNDO MENSAJ?\n");
        system("clear");
        buff_rx[0]='\0';
        len_tx = read(connfd, buff_rx, sizeof(buff_rx)); 
        printf("\nBUFER RECIBIDO [CLIENTE-TGS]\n%s",buff_rx);
        buff_as[0]='\0';
        // OPRACIONES CON EL BUFER RECIBIDO 

       // printf("\nllega");
        buffer_file_tgs(buf_as,"mensajes.txt");
       

        //CONSTRUCCION DE MENSAJS 
        buf1tgs[0]='\0'; 
        buf2tgs[0]='\0';
         
        time(&timestampTGS);
        
        strcpy(timestampC2,time_to_string(timestampTGS));

/* brady al rescate 1
        if(diff_time(timestamp,timestampTGS,lifetime)==1){
            close(connfd); 
        }
*/     

        strcpy(clavesesionTGS,generate_key(4));
        strcpy(buf1tgs,formatoCadenaClienteTgs3(mcas->servicename,timestampC2,lifetimeC,clavesesionTGS)); 

        sprintf(lifetimeCTGS,"%d",lifetimeSS);
        strcpy(buf2tgs,formatoCadenaClienteTgs4(mcas->ID,mcas->servicename,timestampC2,mcas->pcliente,lifetimeCTGS,clavesesionTGS)); 
           
        printf("\nBufers antes de encriptar\n Buffer1:\n%s\n\nBufer2:\n%s\n\n",buf1tgs,buf2tgs);   
        // encriptacion simetrica
        FL_UPDATE_FILE_NAME[0]='\0';
        buffer_file(buf1tgs);
        buf1tgs[0]='\0'; 
        //char buffprueba[BUF_SIZE];
        cmd[0]='\0'; 
        FL_UPDATE_FILE_NAME[0]='\0';
        sprintf(cmd,"openssl enc -aes-256-cbc -md sha512 -pbkdf2 -iter 10000 -salt -in archivorecibido.base64 -out sesiontgs.cifrado -pass  pass:%s",clavesesion); 
        printf("\nEsta es la clave de sesion: %s",clavesesion);
        system(cmd);
        system("openssl enc -base64 -in sesiontgs.cifrado -out archivooriginal.base64");        
        strcpy(buf1tgs,file_buffer("archivooriginal.base64"));
       // printf("\encriptado con simetrica: \n%s",buf1tgs);

        //encriptacion asimetrica 
        FL_UPDATE_FILE_NAME[0]='\0';
       // buffer_file(buf2tgs);    
        file_cliente[0]='\0';
        system("rm archivooriginal.base64");
        strcpy(nuevo,buf2tgs);
        encriptado_keypublic(buf2tgs,"ss_publica"); //encriptado con la lalve del TGS
       // strcpy(file_cliente,"archivooriginal.base64");   
        buf2tgs[0]='\0';
        strcpy(buf2tgs,file_buffer("archivooriginal.base64"));
        buffer_file_prueba(buf2tgs); //prueba
       // printf("\nBffer SS encriptado: \n%s",buf2tgs);

        strcat(buf1tgs,"\n"); 
        strcat(buf1tgs,buf2tgs); 
        system("rm archivooriginal.base64");

        printf("\n[SERVER TGS]\nMENSAJE ENVIADO TGS-CLIENTE\n%s",buf1tgs);



        write(connfd, buf1tgs, strlen(buf1tgs));
       // printf("\n\n\n[SERVER TGS]: Enviado\n");
       // printf("\n%s",buff_as); 
        tgs++; 
        close(connfd); 

    }else{
        if(tgs==2)
        {

        system("clear");
        buff_rx[0]='\0';
        len_tx = read(connfd, buff_rx, sizeof(buff_rx)); 
        printf("\nBUFER RECIBIDO CLIENTE-SS\n%s",buff_rx);
       // system("rm archivorecibido.decifrado");
        buff_as[0]='\0';
        // OPRACIONES CON EL BUFER RECIBIDO 
        buffer_file_tgs(buff_rx,"mensajes.txt");
        separar_mensaje();
        FL_UPDATE_FILE_NAME[0]='\0';
        file_buffer("Mensaje1.txt"); 
        strcat(FL_UPDATE_FILE_NAME,"\n");
        system("rm archivorecibido.base64");
        buffer_file(FL_UPDATE_FILE_NAME);
        desencriptado_servidoresSS("Mensaje1.txt");
        FL_UPDATE_FILE_NAME[0]='\0';
        buff_as[0]='\0';
        buff_rx[0] = '\0';
        strcpy(buff_as,file_buffer("archivorecibido.txt"));
  
        printf("\n\nBufer1 desencriptado con llave ss: \n\n%s",nuevo);

        cmd[0]='\0';
        FL_UPDATE_FILE_NAME[0]='\0';
        system("openssl enc -base64 -d -in Mensaje2.txt -out archivoSS.cifrado");
        sprintf(cmd,"openssl enc -aes-256-cbc -md sha512 -pbkdf2 -in archivoSS.cifrado -out test.txt -d -pass pass:%s",clavesesionTGS);
        system(cmd);
        FL_UPDATE_FILE_NAME[0]='\0';

        FILE *pFilexx;
        pFilexx=fopen("test.txt","r");
        if(pFilexx!=NULL){      
            while(!feof(pFilexx)){
                fgets(cadena20,3,pFilexx);
                if(feof(pFilexx)) break;          
                strcat(cadena21,cadena20);
			
            }
        }
        fclose(pFilexx);
        strcpy(buff_rx,cadena21);
        printf("\n\nBuffer 2 desencriptado con clave de sesion %s\n%s",clavesesionTGS,buff_rx);
        


/* brady al rescate 2
        if(diff_time(timestamp,timestampTGS,lifetimeSS)==1){
            close(connfd); 
        }
*/  

        cmd[0]='\0';
        printf("\n");
        buf1tgs[0]='\0';
        sprintf(cmd,"find /home -iname %s ",mcas->instruccion);
        system(cmd);

        FL_UPDATE_FILE_NAME[0]='\0';
        buf1tgs[0]='\0';

        //CONSTRUCCION DE MENSAJS 
        
     
        printf("\n\n\n[SERVER SS]\nMENSAJE ENVIADO SS-CLIENTE\n");
        system(cmd);



        write(connfd, cmd, strlen(cmd));
  
        tgs++; 
        close(connfd); 
        }
        }

     }    

//printf("\n final d archivo"); 

}

void runServer_tcp_t(int PORT){ 
    port2 = PORT; 
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


//-----------------------------------------------

#endif
