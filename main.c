#include "lib/tcp_thread_client.h"
#include "lib/herramientas.h"
#include "lib/finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AZUL_T "\x1b[34m"
#define VERDE_T "\x1b[32m"
#define BLANCO_T "\x1b[37m"


char id[20], ipcliente[20], servicename[20], opc[2], instruction[1024], lifetime[20], 
serv_addr[20], public[20], buffer[4096]=""; 
int port=2020, input = 0, inicio =0; 
Direccion *address = NULL;

void loginMenu();
void getPort(int static_port);
int Menu();
char *formatoCadena(char str1[], char str2[], char str3[], char str4[], char str5[]);
void ipServidor();
void searchip(); 
char *formatollave(char str1[], char str2[], char str3[]);

int main(){ 
    
    
    loginMenu();  
    char* inst=instruction, rlocal; 
    char decition = ' ', resp=' ', buscarlocal=' ';
    int  input = Menu();    
    char dig = (char)(((int)'0')+input); 
    opc[0] = dig; 
    servicename[0]='A'; // siempre definido como A, como se puede cambiar?  
    lifetime[0]='3'; // ingresar tiempo que va a durar 
    ipcliente[0]='C'; 

    while (input != 5){ 
        switch (input){
                   
        case 1: // por nombre de archivo
        ipServidor(); 
        searchip();
        system("clear"); 

        printf("\nSeguido de 'search $/' ingresa el nombre del archivo con su extension: \n");
        fgets(instruction, sizeof(instruction), stdin);           
        while (address != NULL){
            runClient_tcp_t(port, address->address,formatoCadena(id,ipcliente,servicename,opc,instruction));
            address = address->next; 
        }
        printf("\n\nPresiona enter para regresar a inicio..\n");
        system("rm ip.txt"); 
        pausa();      
                 
        break; 
        
        case 2: // por archivo vacio
        ipServidor();
        system("clear"); 
        printf("\nIngrese el comando 'search -/empty' : ");
        fgets(instruction, sizeof(instruction), stdin);
        while (address != NULL){
            runClient_tcp_t(port, address->address,instruction);
            address = address->next; 
        }
        printf("\n\nPresiona enter para volver al menu...\n");
        pausa();
        break; 
        
        case 3: // por carpetas vacias
        ipServidor();
        system("clear");  
        printf("\nIngrese el comando 'search #/empty': ");
        fgets(instruction, sizeof(instruction), stdin);
        while (address != NULL){
            runClient_tcp_t(port, address->address,instruction);
            address = address->next; 
        }
        printf("\n\nPresiona enter para regresar al menu principal..\n");
        pausa();
        break; 

        default:
            printf("Opcion incorrecta\n");
            pausa();
            break;
        }
        input = Menu();
    }
    //Salir
    return EXIT_SUCCESS;
}


void loginMenu(){

   while (inicio != 1 ){ 
   system("clear");
   printf("\n******BINVENIDO******\n\nIngrese la opcion que desee: \n1.Iniciar sesion\n2.Registrarse\n3.Salir\n");
   scanf("%d", &inicio);

   if(inicio ==1)
   {
    printf("\nIngrese usuario \n\n");
    printf("Usuario: ");
    scanf("%s", &id);
   }
   else 
   {
    if(inicio == 2)
    {
        instruction[0] = '\0';
        printf("\nIngrese Username: "); 
        scanf("%s",&id); 
        strcpy(public,generar_llaves()); //-- save name public key 
        ipServidor();          
        system("clear"); 
        strcpy(instruction,file_buffer(public)); 
        while (address != NULL)
        {            
            runClient_tcp_t(port, address->address,formatollave(id,instruction,"1"));
            address = address->next; 
            printf("\n\nPresiona enter para regresar al menu principal..\n");
            pausa();
        break;

        }

        instruction[0] = '\0'; 

    }   else 
        {
            printf("\nIngrese una opcion correcta");
        }
   }
   }

}

void getPort(int static_port){
    char decition = ' ';
    int resp = 0;
    port = static_port;
    printf("\nPuerto actual: %d\nDeseas cambiar el puerto? s/n ", static_port);
    scanf(" %c", &decition);
    if (decition == 's' || decition == 'S'){
        printf("Ingresa el puerto que quiere utilizar: ");
        scanf("%d", &resp);
        port=resp; 
    }  
}

int Menu(){   
    system("clear");
    printf(AZUL_T"\n*******************************************\n");
    printf(VERDE_T"************** Proyecto Sist.D *************\n");
    printf(AZUL_T "*******************************************\n");
    printf(BLANCO_T "\nBusquedas\n\n");
    printf("1. Por nombre de archivo  \n2. Archivos vacios \n3. Carpetas vacias\n");
    printf("5. SALIR\n\n");
    printf("Opcion: ");
    scanf("%d", &input);          
    return input;
}
// Funcion para concatenar id, opc, mensaje
char *formatoCadena(char str1[], char str2[], char str3[], char str4[], char str5[]){
	char *buffer = (char*) malloc(sizeof(char) * 300);
	strcat(buffer,"ID: ");
	strcat(buffer,str1);
    strcat(buffer,",IPcliente: ");
	strcat(buffer,str2);
	strcat(buffer,",Servicename: ");
	strcat(buffer,str3);
    strcat(buffer,",Opc: ");
	strcat(buffer,str4);
    strcat(buffer,",Ins: ");
	strcat(buffer,str5);
return buffer;	
}

char *formatollave(char str1[], char str2[], char str3[]){
    cleanBuffer();
    buffer[0] = '\0';
//	char *buffer = (char*) malloc(sizeof(char) * 300);
	strcat(buffer,"ID: ");
	strcat(buffer,str1);
    strcat(buffer,",publica: ");
	strcat(buffer,str2);
    strcat(buffer,",nivel: ");
	strcat(buffer,str3);

return buffer;	
}

void ipServidor(){
   /* do{
    printf("Ingresa la direccion IP del servidor: ");
    scanf("%s", serv_addr);
    address = addDireccion(address, newDireccion(serv_addr));
    printf("Desea agregar otra direccion? s/n ");
    scanf(" %c", &decition);
    } while (decition == 's' || decition == 'S'); */
    getPort(port);
    printf("\nIngresa la direccion IP del servidor: ");
    scanf("%s", serv_addr);
    address = addDireccion(address, newDireccion(serv_addr));
    cleanBuffer();
} 

void searchip(){

        system("hostname -I > ip.txt");
        FILE *pFile10;
        pFile10=fopen("ip.txt","r");
        fgets(ipcliente,20,pFile10); 
       
       // printf("Esta es mi ip: %s \n",ipcliente); 
}