#include "lib/tcp_thread_server.h"
//#include "lib/tcp_thread_client.h"
#include "lib/herramientas.h"
#include "lib/finder.h"

//variable global
int port=2020;

void getPort(int static_port){
    char decition = ' ';
    int resp = 0;
    port = static_port;
    port = 2020;
    printf("Puerto actual: %d\nDeseas cambiar el puerto? s/n ", port);
    scanf(" %c", &decition);
    if (decition == 's' || decition == 'S'){
        printf("Ingresa el puerto que quiere utilizar: ");
        scanf("%d", &resp);
        port=resp; 
    }  
}

int main(){

    fflush(stdin); // limpiar buffer 
    char serv_addr[20];         //direccion IP de un servidor
    Direccion *address = NULL;  //Lista de direcciones IP de los servidores a buscar
    char instruction[1024];  	//Consulta a preguntar a los servidores 
    char* inst=instruction;
    char* rlocal;
    char decition = ' ';
    char resp=' ';
    char buscarlocal=' ';

    getPort(2002);
    runServer_tcp_t(port);

    return 0;
}
