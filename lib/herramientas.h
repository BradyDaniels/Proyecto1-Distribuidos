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

char FL_UPDATE_FILE_NAME[BUF_SIZE], buf_as[BUF_SIZE], buf_as2[BUF_SIZE],
buff_as[BUF_SIZE], buff_as2[BUF_SIZE],cadenap[200], cmd[512], publickey[20], publickey2[20],
nombrearchivo[30], cmd2[512], prueba[2048], cadenaz[100];
int w =0; 


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
    
	if(w==0){
		strcat(publickey2,publickey); 
		strcat(publickey2,".pem");
	}
	
	
    FILE* fichero;
    fichero = fopen("archivooriginal.txt", "wt"); 
    fputs(message, fichero);
    fclose(fichero); 
    sprintf(cmd,"openssl rsautl -encrypt -in archivooriginal.txt -inkey %s -pubin -out archivooriginal.cifrado",publickey2);
    system(cmd); // encriptando el archivo original con la llave publica 
    system("openssl enc -base64 -in archivooriginal.cifrado -out archivooriginal.base64"); // codificacion del archivo encripado con base 64
	w++;
    

}

// cambiar el nombre del archivo a recibir *file_cliente
char *file_buffer(char *file){
	
	
    FILE *pFile;
    pFile=fopen(file,"r");
    if(pFile!=NULL){      
        while(!feof(pFile)){
            fgets(cadenap,50,pFile);
            if(feof(pFile)) break;          
            strcat(FL_UPDATE_FILE_NAME,cadenap);
			
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



char *file_buffer2(char *filez){
	
	
    FILE *pFilez;
    pFilez=fopen(filez,"r");
    if(pFilez!=NULL){      
        while(!feof(pFilez)){
            fgets(cadenaz,100,pFilez);
			strcat(prueba,cadenaz);
            if(feof(pFilez)) break;          
            
			
        }
    }
    fclose(pFilez);

return prueba; 
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

void desencriptado_clinte(char *file2, char *idcliente){

cmd[0]='\0';
cmd2[0]='\0'; 
  sprintf(cmd,"openssl enc -base64 -d -in %s -out archivorecibido.cifrado",file2);
  system(cmd);
  sprintf(cmd2,"openssl rsautl -decrypt -inkey %s -in archivorecibido.cifrado -out archivorecibido.txt",idcliente);
  system(cmd2);
	// quizas se modifique el argumento de llave privada y la clave para acceder.   
}



void registro(char *message){
    FILE* fichero;
    fichero = fopen("archivoregistro.txt", "wt");
    fputs(message, fichero);
    fclose(fichero); 
    system("openssl enc -base64 -in archivoregistro.txt -out archivoregistro.codificado"); // codificacion del archivo encripado con base 64
    file_buffer("archivoregistro.codificado"); 
    //printf("\nEste es el mensaje codificado:  \n");
    //system("cat archivoregistro.codificado \n");
    //system("rm archivoregistro.txt");
   // system("rm archivoregistro.codificado");
   
    // printf("Esto es lo que tiene el buffer: %s",FL_UPDATE_FILE_NAME); 

}

char *generate_key(int max){
    // diccionario de caracteres
    char diccionario[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z',
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    srand(getpid()); // genero una semilla para poder tener un numero random
    char *keysesion = malloc(sizeof(char)+max); // le doy la cantidad de memoria a la variable que guardara la clave
    int len = (int)strlen(diccionario); // obtengo la longitud de caracteres del diccionario

    for(int i = max; i >= 0; i--){
        keysesion[i] += diccionario[rand() % len]; // se genera la clave aleatoria
    }
    
    return keysesion; // retorno la clave
}

// -------- 


/*--Estructura para guardar los datos de un registro de la bdd--*/
typedef struct Registro{
	char *ID ;
	char *nivel ;
} Registro;
/*--------------------------------------------------------------*/

/*--Estructura para guardar los datos de un mensaje--*/
typedef struct Mensaje_cliente_as{
	char *ID ;
	char *nivel ;
	char *pcliente ;
	char *servicename ;
	char *opcion ;
	char *instruccion ;
} Mensaje_cliente_as;

Mensaje_cliente_as *mcas; 

/*-------------------------------------------------------------*/

/*--Funcion que escribe los registros en la bdd--*/
int insertarRegistro(char id[], char nivel[]){
	FILE *usuarios = fopen("usuarios.txt", "a");
	
	if (usuarios==NULL){
		printf("Error en la apertura del archivo \n");
		return -1;	
	}	
		
		fprintf(usuarios, "ID: %s,Nivel: %s\n" , id,nivel);
		
	fclose(usuarios);
return 0;}
/*------------------------------------------------*/

						  
char *buscarDatos(char clave[], char registro[], char valor[], int id_flag){

  
	char *strAux = NULL, *valorAux = NULL, *r = NULL;
	int j = 0;
//	printf("Entra en validacion222");
	for(int i = 0; i < strlen(registro)-1; i++){
	
		if(registro[i] == ':'){

			if(strcmp(strAux,clave) == 0){
				if(valorAux == NULL)
					valorAux = (char*) malloc(2050 *sizeof(char));
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
						
						r = (char * ) malloc(sizeof(char) * 2056); // 350
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
			strAux = (char*)malloc(sizeof(char) * 2056); //30
		}	
		strAux[j] = registro[i];
		j++;
	}
 
 return NULL;
}
/*----------------------------------------------------------------*/

/*-----Funcion que copia una linea de registro en una estructura y lo devuelve----*/
Registro *copiarRegistro(char registro[], char valor[]){
	Registro *r = (Registro *) malloc(sizeof(Registro));
	r->ID = (char *) malloc(sizeof(char) * 100);
	r->nivel = (char *) malloc(sizeof(char) * 1);
	
	r->ID = valor;
	r->nivel = buscarDatos("Nivel",registro,NULL,0);
	
return r;		
}
/*------------------------------------------------------------------------------*/

/*--Funcion que copia una linea de mensaje del cliente en una estructura y lo devuelve--*/
Mensaje_cliente_as *copiarMensajeClienteAS(char mensaje[]){
	mcas = (Mensaje_cliente_as *) malloc(sizeof(Mensaje_cliente_as));
	mcas->ID = (char *) malloc(sizeof(char) * 100);
	mcas->nivel = (char *) malloc(sizeof(char) * 1);
	mcas->pcliente = (char *) malloc(sizeof(char) * 2050);
	mcas->servicename = (char *) malloc(sizeof(char) * 100);
	mcas->opcion = (char *) malloc(sizeof(char) * 1);
	mcas->instruccion = (char *) malloc(sizeof(char) * 200);
	
	mcas->ID = buscarDatos("ID",mensaje,NULL,0);
	mcas->nivel = buscarDatos("nivel",mensaje,NULL,0);
	mcas->pcliente = buscarDatos("IPcliente",mensaje,NULL,0);
	mcas->servicename = buscarDatos("Servicename",mensaje,NULL,0);
	mcas->opcion = buscarDatos("Opc",mensaje,NULL,0);
	mcas->instruccion = buscarDatos("Ins",mensaje,NULL,0);

	return mcas;	
}
/*-----------------------------------------------------------------------------------*/


/*-------Funcion que lee un archivo con el fin de encontrar un ID en él--------------*/
Registro *buscarRegistro(char ID[]){
	FILE *archivo;
	char registro[200];
	Registro *r = NULL;
	archivo = fopen("usuarios.txt","r");
	char *rencontrado = NULL;

	if (archivo == NULL)
    	{
        printf("\nError en la apertura del archivo. \n\n");
        return NULL;
    	}
    	else
    	{
		while (feof(archivo) == 0)
 	    	{
 	        	fgets(registro,200,archivo);
 	        	rencontrado = buscarDatos("ID", registro, ID,1);
 	        	if(rencontrado != NULL){
 	        		r = copiarRegistro(rencontrado, ID);
 	        		return r;
 	        }
 	        			        
 	    }
    }

   fclose(archivo);
return NULL;
}

/*-----------Metodo nro. 1---------------------------------*/
char *formatoCadenaRegistro(char str1[], char str2[]){
	char *buffer = (char*) malloc(sizeof(char) * 400);
	/*Limpia el buffer*/
	for(int i = 0; i < strlen(buffer); i++)
		buffer[i] = '\0';
	strcat(buffer,"ID: ");
	strcat(buffer,str1);
	strcat(buffer,",Nivel: ");
	strcat(buffer,str2);

return buffer;	}
/*---------------------------------------------------------*/

/*--------------------------------------------------------*/
int validarExistenciaUsuario(char ID[100]){

	printf("\nDENTRO llega: %s",ID);

	if (buscarRegistro(ID) == NULL)
		return 0;
return 1;
}
/*------------------------------------------------------*/
void buffer_file2(char *buff_rx_as, char ID[100]){
    FILE *fichero2; 
    fichero2 =fopen(ID,"wt");
    fputs(buff_rx_as,fichero2); 
    fclose(fichero2);
}

/*------------------------------------------------------*/
int registrarUsuario(char *buff){
	char *r = "", *ID = "", *publica = "", *nivel = "";
	
	if ( buscarDatos("publica", buff, NULL, 0) == NULL)
		return 0;
		
	r = buscarDatos("publica", buff, NULL, 0);
	ID = buscarDatos("ID", buff, NULL, 0);
	nivel = buscarDatos("nivel", buff, NULL, 0);
	publica = buscarDatos("publica", buff, NULL, 0);
	
	if (validarExistenciaUsuario(ID) == 0){
		insertarRegistro(ID, nivel);
		strcat(ID,".pem");
		buffer_file2(publica,ID);
		printf("Registro exitoso\n");
	}else{
		printf("El ID ingresado ya se encuentra registrado en nuestro sistema\n");
		return 0;
	}
	
	printf("ID: %s,nivel: %s,publica: %s", ID,nivel,publica);
	
return 1;}

//funcion que dada dos marcas de tiempo retorna la -1 si la diferencia entre ambas supera 
// el limite(limit)
int diff_time(time_t start,time_t end,int limit){

     double diff_t=difftime(end,start);
     if(diff_t>=limit){
         return -1;
     }
     else 
        return 0;
}


char *asCliente1(char tgsname[], char timestamp[], char lifetime[], char clavesesion[]){
	//char *buff_as = (char*) malloc(sizeof(char) * 300);

	strcat(buff_as,"TGS: ");
	strcat(buff_as,tgsname);
    strcat(buff_as,",Timestamp: ");
	strcat(buff_as,timestamp);
	strcat(buff_as,",Lifetime: ");
	strcat(buff_as,lifetime);
    strcat(buff_as,",Clavesesion: ");
	strcat(buff_as,clavesesion);

return buff_as;	
}

char *asCliente2(char id[], char tgsname[], char timestamp[], char ipcliente[], char lifetime[], char clavesesion[]){
	//char *buff_as = (char*) malloc(sizeof(char) * 300);
	strcat(buff_as2,"ID: ");
	strcat(buff_as2,id);	
	strcat(buff_as2,",TGS: ");
	strcat(buff_as2,tgsname);
    strcat(buff_as2,",Timestamp: ");
	strcat(buff_as2,timestamp);
	strcat(buff_as2,",Ipcliente: ");
	strcat(buff_as2,ipcliente);
	strcat(buff_as2,",Lifetime: ");
	strcat(buff_as2,lifetime);
    strcat(buff_as2,",Clavesesion: ");
	strcat(buff_as2,clavesesion);

return buff_as2;	
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

    s[2]=s[5]='\0';

    ts.tm_hour=atoi(&s[0]);
    ts.tm_min=atoi(&s[3]);
    ts.tm_sec=atoi(&s[6]);

    time_t t=mktime(&ts);

    return t;

}

char *formatoCadenaClienteTgs1(char str1[], char str2[]){
	char *buffer = (char*) malloc(sizeof(char) * 400);
	for(int i = 0; i < strlen(buffer); i++)
		buffer[i] = '\0';
	strcat(buffer,"Servicename: ");
	strcat(buffer,str1);
	strcat(buffer,",Lifetime: ");
	strcat(buffer,str2);

return buffer;	}

char *formatoCadenaClienteTgs2(char str1[], char str2[]){
	char *buffer = (char*) malloc(sizeof(char) * 400);
	for(int i = 0; i < strlen(buffer); i++)
		buffer[i] = '\0';
	strcat(buffer,"ID: ");
	strcat(buffer,str1);
	strcat(buffer,",Timestamp: ");
	strcat(buffer,str2);

return buffer;	}

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

void buffer_file_tgs(char *buff_rx_as, char nombre_archivo[100]){
    FILE *fichero2; 
    fichero2 = fopen(nombre_archivo,"wt");
    fputs(buff_rx_as,fichero2); 
    fclose(fichero2);
}

int contar_saltos_linea(){
	FILE *archivo;
	char linea[500], buffer[2048] , msj[15] = "";
	int j = 0, num_linea = 0;
	archivo = fopen("mensajes.txt","r");

	if (archivo == NULL)
    	{
        	printf("\nError en la apertura del archivo. \n\n");
        	return 0;
    	}
    	else
    	{
		while (feof(archivo) == 0)
 	    	{	
 	        	fgets(linea,500,archivo);
 
  	        	if (strcmp(linea,"\n") == 0  ){	        
  	        		num_linea++;	        		     
 	    		}	    		    			
   		}
   	fclose(archivo);
   }


return num_linea;}

char *separar_mensaje(){
	FILE *archivo;
	char linea[500], linea_anterior[500], buffer[2048] , msj[15] = "", num_text[2] = "\0";
	int j = 0, num = 0, num_lineas = 0;
	archivo = fopen("mensajes.txt","r");
	char *rencontrado = NULL;

	if (archivo == NULL)
    	{
        	printf("\nError en la apertura del archivo. \n\n");
        	return NULL;
    	}
    	else
    	{
    	num_lineas = contar_saltos_linea();
    	
		while (feof(archivo) == 0)
 	    	{	
 	        	fgets(linea,500,archivo);
 	   	        	
  	        	if (strcmp(linea,"\n") == 0  || (num == num_lineas)){
  	        		num++;
  	        		sprintf(num_text,"%d",num);
 
 	        		for (int k = 0; k < strlen(msj); k++)
 	        			msj[k] = '\0';
 	        			
 	        		if (num > num_lineas){
 	        			/*for (int k = 0; k < strlen(buffer); k++)
 	        				buffer[k] = '\0';*/
 	        			for (int i = 0; i < strlen(linea); i++){
 	    					buffer[j] = linea[i];
 	    					j++;
 	    				}
 	    				buffer[j] = '\0';
 	        		}else{
 	        			buffer[j-1] = '\0';	
 	        		}
 	        		
 	        		
 	        		strcat(msj,"Mensaje");	
 	        		strcat(msj,num_text);
 	        		strcat(msj,".txt");
 	        		printf("Buffercito:-%s-\n", buffer);
 	        		buffer_file_tgs(buffer,msj);
	   
 	        		j = 0;
 	        		
 	        		for (int k = 0; k < strlen(buffer); k++)
 	        			buffer[k] = '\0';
 	        		     
 	    		}
 	    		else
 	    			{
 	    			for (int i = 0; i < strlen(linea); i++){
 	    				buffer[j] = linea[i];
 	    				j++;
 	    			}
 	    			
	}
   }
   fclose(archivo);
   }
return NULL;
}

#endif
