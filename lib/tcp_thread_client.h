                                 
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

char miguel[20], FL_UPDATE_FILE_NAME[BUF_SIZE], private_user_key[KEY_SIZE], 
public_user_key[KEY_SIZE], idcliente[10], cadena[200], cmd[512], file_cliente[30], 
publickey2[KEY_SIZE], file_cliente2[30], prueba20[200],idcliente_tgs[BUF_SIZE],servicename_tgs[30],
timestamp_tgs[30], lifetime_tgs[30], clavesesion_tgs[30],
buf_as1[BUF_SIZE], buf_as2[BUF_SIZE], buf1[BUF_SIZE], buf2[BUF_SIZE], buf3[BUF_SIZE], tgs_tgs[50]; 

int auxiliar, n, n2; 



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
        printf(" Este es el archivo que se envia al servidor: \n"); 
        printf("%s",conexion->message);
        write(sockfd, conexion->message, sizeof(conexion->message));  //envia
        auxiliar = 1;
        pausa();
    }


    if(auxiliar ==0){
        printf("\nRegistrado\n");
    } else{
            n = read(sockfd, buf_as, BUF_SIZE); // recibe

    }

   
   // buffer_file();
    
    buf_as[n]='\0'; 
    if (n==0 || auxiliar==0){
        printf("\nNo se hallaron resultados en el servidor de IP: %s",conexion->IP_SERVER);
    }
    else {
        system("clear");
        FL_UPDATE_FILE_NAME[0]='\0';
        printf("\n[SERVER AS%s] resultado: \n%s\n",conexion->IP_SERVER,buf_as); 
    
        // llamado a funcion que me va a separar en 2 cadenas. 
        buffer_file_tgs(buf_as,"mensajes.txt"); 
        separar_mensaje();
       // strcpy(buf_as1,file_buffer("Mensaje1.txt")); // est s pud dsncriptar
        FL_UPDATE_FILE_NAME[0]='\0'; 
        strcpy(buf_as2,file_buffer("Mensaje2.txt"));
        FL_UPDATE_FILE_NAME[0]='\0'; 

        // buf 2 = buf 1 sera l q no se pued dsncriptar

        buffer_file(buf_as1); // pasar nuevo bufer el que s pude desncriptar

        printf("\nIngrese nombre de llave privada : ");
        scanf("%s", &idcliente);
        strcat(idcliente,".pem"); 
        desencriptado_clinte("Mensaje1.txt",idcliente); 
        buf_as[0] = '\0';
        strcpy(buf_as,file_buffer2("archivorecibido.txt")); 
        printf("\n\nBuffer 1 desencriptado: \n%s",buf_as); 
/*
        strcpy(tgs_tgs,buscarDatos("TGS",buf_as,NULL,0));
        strcpy(timestamp_tgs,buscarDatos("Timestamp",buf_as,NULL,0));
        strcpy(lifetime_tgs,buscarDatos("Lifetime",buf_as,NULL,0));
        strcpy(clavesesion_tgs,buscarDatos("Clavesesion",buf_as,NULL,0));
        */
        pausa(); 
        
        //aqui viene la comunicacion CLiente - TGS 
        // Cnstruccion de mensaje 2: servicename y lifetime 
        strcpy(buf2,formatoCadenaClienteTgs1(servicename_tgs,lifetime_tgs));

        //construccion del mensaje 3: username y timestamp 
        strcpy(buf3,formatoCadenaClienteTgs1(idcliente_tgs,timestamp_tgs));
        system("rm archivorecibido.base64"); 
        buffer_file(buf3);
        buf3[0]='\0'; 
        cmd[0]='\0'; 
        sprintf(cmd,"openssl enc -aes-256-cbc -md sha512 -pbkdf2 -iter 10000 -salt -in archivorecibido.base64 -out sesiontgs.cifrado -pass  pass:%s",clavesesion_tgs); 
        system(cmd);
        strcpy(buf3,file_buffer("sesiontgs.cifrado"));

        // proceso para unir los 3 bufr qu s nviaran al TGS
         strcat(buf1, "\n"); // buf1= msj encriptado, buf2 y buf3 msj creados 
         strcat(buf1,buf2); 
         strcat(buf1, "\n");
         strcat(buf1, buf3); 

        printf("BUFER CLINT TGS: \n%s",buf1); 
//------------------------COMUNIC CLINT-TGS -------------
        write(sockfd, buf1, sizeof(buf1)); // SI STO NO FUNCIONA CREO UNA CONEXION 2 O LLAMO D NUEVI
        // SI NO FUNCIONA, N L MISMO MAIN PUDO HACR OTRO LLAMADO A RUN_CLINTE Y VALIDO QUE TNGA
        // UNA BANDRA NCNENDIDA PARA LA SEGUNDA COMUNICACION 

        //system("cat archivorecibido.txt");
        //strcat(prueba20,file_buffer2("archivorecibido.txt"));       
       // printf("\n[SERVER %s] resultado2: \n%s",conexion->IP_SERVER,buf_as2);
    

    }
   
    printf("\n\n\n[CLIENT]: socket cerrado \n\n");
    //printf("Mensaje desencriptado: \n%s",prueba20); 
  //  printf("BUFFER %s",file_buffer2("archivorecibido.txt")); 
  //  pausa(); 
    close(sockfd); 

    
}


void runClient_tcp_t(int port, char *address, char *message){
  
	Conexion conexion; 
	pthread_t hilo;
    conexion.PORT = port; 
    strcpy(conexion.IP_SERVER,address);
    


    if(buscarDatos("publica",message,NULL,0) != NULL){ // SI ENCUENTRA A PUBLICA 
    auxiliar = 0;
    strcpy(conexion.message, message);
    pthread_create(&hilo, NULL, client_tcp_t, (void *)&conexion);
	pthread_join(hilo, NULL);  
    conexion.message[0] = '\0';
    FL_UPDATE_FILE_NAME[0] = '\0';
    } else {
    /*  
        printf("\nLLEGA3 aqui si"); 
        printf("\nLLEGA4"); 
        printf("\nLLEGA5"); 
        strcpy(idcliente_tgs,buscarDatos("ID",buf_as,NULL,0));
    printf("\nLLEGA5"); 
    strcpy(servicename_tgs,buscarDatos("Servicename",buf_as,NULL,0));
    printf("\nLLEGA6"); 
    */
    encriptado_keypublic(message,"as_publica"); 
    system("clear");

    printf("Comunicacion CLIENTE ---> \n\nMensaje original:\n");
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
