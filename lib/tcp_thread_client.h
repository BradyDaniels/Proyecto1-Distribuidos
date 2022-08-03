                                 
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
public_user_key[KEY_SIZE], idcliente[10], cadena[200], cmd[BUF_SIZE], file_cliente[30], 
publickey2[KEY_SIZE], file_cliente2[30], prueba20[200],idcliente_tgs[BUF_SIZE],servicename_tgs[30],
timestamp_tgs[10], lifetime_tgs[30], clavesesion_tgs[30], clavesesion_ss[30], cadena10[300], cadena11[300],
buf_as1[BUF_SIZE], buf_as2[BUF_SIZE], buf1[BUF_SIZE], buf2[BUF_SIZE], buf3[BUF_SIZE], tgs_tgs[50],
timestamp_ss[10]; 

int auxiliar, n, n2; 

char *buscarD(char clave[], char registro[], char valor[], int id_flag);
void buffer_file_tgs2(char *buff_rx_as, char nombre_archivo[100]);


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
            system("rm mensajes.txt"); 
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
        timestamp_tgs[0]='\0';
        FL_UPDATE_FILE_NAME[0]='\0'; 
        buf_as[0]='\0';
        file_buffer("Mensaje1.txt");
        strcpy(buf_as,FL_UPDATE_FILE_NAME);

        pausa();

        FL_UPDATE_FILE_NAME[0]='\0'; 
        strcpy(buf1,file_buffer("Mensaje2.txt"));
        FL_UPDATE_FILE_NAME[0]='\0'; 



        printf("\nIngrese nombre de llave privada : ");
        scanf("%s", &idcliente);
        strcat(idcliente,".pem"); 
        desencriptado_clinte("Mensaje1.txt",idcliente); 

        buf_as[0] = '\0';
        FL_UPDATE_FILE_NAME[0]='\0';
        strcpy(buf_as,file_buffer2("archivorecibido.txt")); 
        printf("\n\nBuffer 1 desencriptado: \n%s\n",buf_as); 

        strcpy(tgs_tgs,buscarDatos("TGS",buf_as,NULL,0));
        strcpy(timestamp_tgs,buscarDatos("Timestamp",buf_as,NULL,0));
        strcpy(clavesesion_tgs,buscarDatos("Clavesesion",buf_as,NULL,0)); 
        strcpy(lifetime_tgs,buscarDatos("Lifetime",buf_as,NULL,0));
        
       // file_buffer("archivorecibido.txt");       
       // pausa(); 
        
        //aqui viene la comunicacion CLiente - TGS 
        // Cnstruccion de mensaje 2: servicename y lifetime 
        strcpy(buf2,formatoCadenaClienteTgs1(tgs_tgs,lifetime_tgs));

        //construccion del mensaje 3: username y timestamp 
        strcpy(buf3,formatoCadenaClienteTgs1(idcliente_tgs,timestamp_tgs));
       // system("rm archivorecibido.base64"); 
        buffer_file(buf3);
        buf3[0]='\0'; 
        cmd[0]='\0'; 
       // system("rm archivooriginal.base64");
        sprintf(cmd,"openssl enc -aes-256-cbc -md sha512 -pbkdf2 -iter 10000 -salt -in archivorecibido.base64 -out sesiontgs.cifrado -pass  pass:%s",clavesesion_tgs); 
        system("openssl enc -base64 -in sesiontgs.cifrado -out archivooriginal.base64");
        system(cmd);
        strcpy(buf3,file_buffer("archivooriginal.base64"));

        // proceso para unir los 3 bufr qu s nviaran al TGS
         strcat(buf1, "\n"); // buf1= msj encriptado, buf2 y buf3 msj creados 
         strcat(buf1,buf2); 
         strcat(buf1, "\n\n");
         strcat(buf1, buf3); 

        //system("clear");


        pausa();

    

    }
   
    printf("\n\n\n[CLIENT AS]: socket cerrado \n\n");

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

        strcpy(idcliente_tgs,buscarD("ID",message,NULL,0));
        strcpy(servicename_tgs,buscarD("Servicename",message,NULL,0));
  
    encriptado_keypublic(message,"as_publica"); 
    system("clear");

    printf("Comunicacion CLIENTE --->AS \n\nMensaje original:\n");
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

//---------------COMUNICAION CLIENTE-TGS----------------------------------// 
void *client_tcp_t_TGS(void *args){
                   
    buf_as[0]='\0';               
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
        system("clear"); 
        printf("COMUNICACION CLINTE-TGS\n\n");
    	printf("[CLIENT]: connected to SERVER %s\n",conexion->IP_SERVER); 
        printf(" Este es el archivo que se envia al servidor: \n"); 
        printf("%s",conexion->message);
        write(sockfd, conexion->message, sizeof(conexion->message));  //envia
       // auxiliar = 1;
        pausa();
    }

           // system("rm mensajes.txt"); 
            n = read(sockfd, buf_as, BUF_SIZE); // recibe
             

    
    buf_as[n]='\0'; 
    if (n==0 || auxiliar==0){
        printf("\nNo se hallaron resultados en el servidor de IP: %s",conexion->IP_SERVER);
    }
    else {
        system("clear");

        FL_UPDATE_FILE_NAME[0]='\0';
        printf("\n[SERVER TGS%s] resultado: \n%s\n",conexion->IP_SERVER,buf_as); 

        // empezar a operar con buf_as 
        
        buffer_file_tgs(buf_as,"mensajes.txt");
        separar_mensaje();
        timestamp_tgs[0]='\0';
        buf_as[0]='\0';
 
        cmd[0]='\0';
        FL_UPDATE_FILE_NAME[0]='\0';
        system("openssl enc -base64 -d -in Mensaje1.txt -out archivorecibido.cifrado");
        printf("\nCavesesion tgs: %s",clavesesion_tgs);
        sprintf(cmd,"openssl enc -aes-256-cbc -md sha512 -pbkdf2 -in archivorecibido.cifrado -out testclear.txt -d -pass pass:%s",clavesesion_tgs);
        system(cmd);
        FL_UPDATE_FILE_NAME[0]='\0';
        buf1[0]='\0';

        FILE *pFilexx;
        pFilexx=fopen("testclear.txt","r");
        if(pFilexx!=NULL){      
            while(!feof(pFilexx)){
                fgets(cadena10,3,pFilexx);
                if(feof(pFilexx)) break;          
                strcat(cadena11,cadena10);
			
            }
        }
        fclose(pFilexx);
        strcpy(buf1,cadena11);
       // system("rm testclear.txt && archivorecibido.cifrado");
        printf("\n\nBuffer 1 desencriptado con clave de sesion %s",clavesesion_tgs);
        printf("\n%s",buf1);
        FL_UPDATE_FILE_NAME[0]='\0'; 
        buf2[0]='\0';
        timestamp_tgs[0]='\0';
        strcpy(clavesesion_ss,buscarDatos("Clavesesion",buf1,NULL,0)); 
        strcpy(timestamp_tgs,buscarDatos("Timestamp",buf1,NULL,0)); 

        //comparar que el timestampactual - el timestampbuf < lifetime
        
        buf1[0]='\0';
        FL_UPDATE_FILE_NAME[0]='\0'; 
        strcpy(buf1,file_buffer("Mensaje2.txt"));
        FL_UPDATE_FILE_NAME[0]='\0'; 

        //CONSTRUCCION DE MENSAJES CLIENTE -SS
        time_t timestampSS; 
        time(&timestampSS);
        timestamp_ss[0]='\0';
        strcpy(timestamp_ss,time_to_string(timestampSS));
        strcpy(buf2,formatoCadenaSS(idcliente_tgs,timestamp_ss));

       // system("rm archivorecibido.base64");
        buffer_file(buf2);
        cmd[0]='\0'; 
        FL_UPDATE_FILE_NAME[0]='\0';
        sprintf(cmd,"openssl enc -aes-256-cbc -md sha512 -pbkdf2 -iter 10000 -salt -in archivorecibido.base64 -out sesionSS.cifrado -pass  pass:%s",clavesesion_ss); 
        printf("\n\nClavesesion ss: %s",clavesesion_ss);
        system(cmd);
        buf2[0]='\0';
        system("openssl enc -base64 -in sesionSS.cifrado -out archivooriginal.base64");        
        strcpy(buf2,file_buffer("archivooriginal.base64"));

        strcat(buf1,"\n"); 
        strcat(buf1,buf2); 

        pausa();

    }
   
    printf("\n\n\n[CLIENT - TGS]: socket cerrado \n\n");
    

    close(sockfd); 
    
}


void runClient_tcp_t_TGS(int port, char *address, char *message){

    system("clear"); 
 
	Conexion conexion; 
	pthread_t hilo;
    conexion.PORT = port; 
    strcpy(conexion.IP_SERVER,address);
    strcpy(conexion.message, buf1); 
	pthread_create(&hilo, NULL, client_tcp_t_TGS, (void *)&conexion);
	pthread_join(hilo, NULL);

    FL_UPDATE_FILE_NAME[0] = '\0'; // para eliminar contenido de variable 

}

//---------------COMUNICACION CLIENTE-SS----------------------------// 
void *client_tcp_t_SS(void *args){
                   
    buf_as[0]='\0';               
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
        system("clear"); 
        printf("COMUNICACION CLINTE-SS\n\n");
    	printf("[CLIENT]: connected to SERVER %s\n",conexion->IP_SERVER); 
        printf(" Este es el archivo que se envia al servidor: \n"); 
        printf("%s",conexion->message);
        write(sockfd, conexion->message, sizeof(conexion->message));  //envia
       // auxiliar = 1;
        pausa();
    }

           // system("rm mensajes.txt"); 
            buf_as[0]='\0';
            n = read(sockfd, buf_as, BUF_SIZE); // recibe
             

    
    buf_as[n]='\0'; 
    if (n==0 || auxiliar==0){
        printf("\nNo se hallaron resultados en el servidor de IP: %s",conexion->IP_SERVER);
    }
    else {
        system("clear");

        FL_UPDATE_FILE_NAME[0]='\0';
        printf("\n[SERVER SS%s] resultado: \n\n",conexion->IP_SERVER); 
        system(buf_as);


        pausa();

    }
   
    printf("\n\n\n[CLIENT - SS]: socket cerrado \n\n");
    

    close(sockfd); 
    
}


void runClient_tcp_t_SS(int port, char *address, char *message){

    system("clear"); 
 
	Conexion conexion; 
	pthread_t hilo;
    conexion.PORT = port; 
    strcpy(conexion.IP_SERVER,address);
    strcpy(conexion.message, buf1); 
	pthread_create(&hilo, NULL, client_tcp_t_SS, (void *)&conexion);
	pthread_join(hilo, NULL);

    FL_UPDATE_FILE_NAME[0] = '\0'; // para eliminar contenido de variable 

}



// printf("\nCOMUNICACION CLIENTE-SS\nMENSAJE ENVIADO: \n%s",buf1);
//-------------------------------------------------------------------------//
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

char *buscarD(char clave[], char registro[], char valor[], int id_flag){

 char *strAux = NULL, *valorAux = NULL, *r = NULL;
//char strAux[20], valorAux[20], r[20];  

int j = 0;
//printf("Entra en validacion222");
for(int i = 0; i < strlen(registro)-1; i++){

if(registro[i] == ':'){

if(strcmp(strAux,clave) == 0){
if(valorAux == NULL)
valorAux = (char*) malloc(30 *sizeof(char));
int l = 0;
int k = i+2;
do{
valorAux[l] = registro[k];
l++;
k++;
}
while(registro[k] != ',' && registro[k] != '\0');

valorAux[k] = '\0';

if(id_flag){
if(strcmp(valorAux, valor) == 0){
//printf("Entra en validacion");
r = (char * ) malloc(sizeof(char) * 350);
r = registro;
return r;
}
}else
return valorAux;

}
free(strAux);
strAux = NULL;
int g = 0;
for(g = i; g < strlen(registro)-1; g++){
if(registro[g] == '\0' || registro[g] == ','  )
break;

}
i = g + 1;
}
if(strAux == NULL){
j = 0;
strAux = (char*)malloc(sizeof(char) * 30);
}
strAux[j] = registro[i];
j++;
}
 
 return NULL;
}

void buffer_file_tgs2(char *buff_rx_as, char nombre_archivo[100]){
    FILE *fichero2; 
    fichero2 = fopen(nombre_archivo,"wt");
    fputs(buff_rx_as,fichero2); 
    fclose(fichero2);
}

#endif
