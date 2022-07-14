//#ifndef _test_pem_h
//#define _test_pem_h
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>
static int passwd_callback(char *pcszbuff,int size,int rwflag,void *pPass);
#ifndef MAX
#define MAX 100
#endif

// funcion para generar llave privada y publica 
// tiene como parametros los nombre que quiero ponerles 
int generate_key(char *priv_name,char *pub_name)
{
	int				ret = 0;
	RSA				*r = NULL,*rpub=NULL;
	BIGNUM			*bne = NULL;
	BIO				*bp_public = NULL, *bp_private = NULL;
	char buffer[MAX];
	char buffer2[MAX];

	int				bits = 2048;
	unsigned long	e = RSA_F4;

	// 1. generate rsa key
	bne = BN_new();
	ret = BN_set_word(bne,e);
	if(ret != 1){
		goto free_all;
	}

	r = RSA_new();
	ret = RSA_generate_key_ex(r, bits, bne, NULL);
	if(ret != 1){
		goto free_all;
	}

	// 2. save public key
	strcat(strcpy(buffer,pub_name),".pem");
	bp_public = BIO_new_file(buffer, "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	
	if(ret != 1){
		goto free_all;
	}
	


	// 3. save private key
	strcat(strcpy(buffer2,priv_name),".pem");
	bp_private = BIO_new_file(buffer2, "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    //4.Encryption and Decryption


	// 5. free
free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(r);
	BN_free(bne);

	return (ret == 1);
};

// funcion para enriptar 
char * public_encript(char *msg,char *pub_name){
   
   
   FILE *fp=NULL;
   char buffer[MAX];
   char buffer2[MAX];
   char *encrypt,*err;
   int encrypt_len;
   

   //Leer la clave publica
   strcat(strcpy(buffer,pub_name),".pem");
   fp=fopen(buffer,"r");
   RSA *rsa_pub=PEM_read_RSAPublicKey(fp,NULL,NULL,NULL);

   //Preparar el mensaje a encriptar
   //msg[strlen(msg)-1]='\0';

   //Encriptacion con clave publica
   err=malloc(130);
   encrypt=malloc(RSA_size(rsa_pub));
   if((encrypt_len=RSA_public_encrypt(strlen(msg)+1,(unsigned char*)msg,(unsigned char*)encrypt,rsa_pub,RSA_PKCS1_OAEP_PADDING))==-1)
	{
		ERR_error_string(ERR_get_error(),err);
        fprintf(stderr,"Error encrypting message %s\n",err);

        goto free_stuff;
    }
   
    RSA_free(rsa_pub);
	fclose(fp);

	return encrypt;
    free_stuff:
     RSA_free(rsa_pub);
	 free(encrypt);
	 fclose(fp);

	 //return " ";

}

// 10.173.108.203
// funcion para desencriptar 
char * private_decript(char *priv_name,char *encrypt){
   
  // printf("Este es l mensaje quee recibo: %s",encrypt);
  // printf("\n\n Este es el nombre d la llave: %s",priv_name);
   FILE *fp=NULL;
   char buffer[MAX];
   char buffer2[MAX];
   char *decrypt,*err;
   int encrypt_len=256;
  // int encrypt_len = sizeof(encrypt); 
	//printf("El tamano del encriptado es de: %d",encrypt_len);

   //Leer la clave privaada
   strcat(strcpy(buffer,priv_name),".pem");
   fp=fopen(buffer,"r");
   RSA *rsa_priv=PEM_read_RSAPrivateKey(fp,NULL,NULL,NULL);

   //Desencriptar mensaje
   decrypt=malloc(encrypt_len);
   err=malloc(130);
   	if(RSA_private_decrypt(encrypt_len,(unsigned char *)encrypt,(unsigned char*)decrypt,rsa_priv,RSA_PKCS1_OAEP_PADDING)==-1){
        ERR_load_CRYPTO_strings();
        ERR_error_string(ERR_get_error(),err);
        fprintf(stderr,"Error decrypting message: %s\n",err);
    };


	fclose(fp);

	return decrypt;


}

//-------------------------------------------------------------------
// para que sirve esta ??

int read_public_key(){
    FILE *fp=NULL,*fp2;
	fp=fopen("public.pem","r");
	fp2=fopen("private.pem","r");
	passwd_callback;
	const char *p="hola mundo";
    RSA *rsa_pub=PEM_read_RSAPublicKey(fp,NULL,NULL,NULL);
	RSA *rsa_priv=PEM_read_RSAPrivateKey(fp2,NULL,NULL,NULL);
	BIO *bio_pub=BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPublicKey(bio_pub,rsa_pub);
	size_t pub_len;
	pub_len=BIO_pending(bio_pub);
	char *pub_key;
	pub_key=malloc(pub_len+1);
	BIO_read(bio_pub,pub_key,pub_len);
	printf("PUBLIC KEY: \n %s\n",pub_key);
	char *encrypt,*err,*decrypt;
	int encrypt_len;
	char msg[2048/8];
	printf("Message to encrypt: ");
    fgets(msg,2048-1,stdin);
    msg[strlen(msg)-1]='\0';
	err=malloc(130);
	encrypt=malloc(RSA_size(rsa_pub));
	if((encrypt_len=RSA_public_encrypt(strlen(msg)+1,(unsigned char*)msg,(unsigned char*)encrypt,rsa_pub,RSA_PKCS1_OAEP_PADDING))==-1)
	{
		ERR_error_string(ERR_get_error(),err);
        fprintf(stderr,"Error encrypting message %s\n",err);

        //goto free_stuff;
    }
	printf("\n mensaje encriptado: %s\n",encrypt);
    decrypt=malloc(encrypt_len);
	if(RSA_private_decrypt(encrypt_len,(unsigned char *)encrypt,(unsigned char*)decrypt,rsa_priv,RSA_PKCS1_OAEP_PADDING)==-1){
        ERR_load_CRYPTO_strings();
        ERR_error_string(ERR_get_error(),err);
        fprintf(stderr,"Error decrypting message: %s\n",err);
    };
    printf("Decrypted message %s\n",decrypt);
	
	return 0;
};




/*
int main(int argc, char* argv[]) 
{
    
	generate_key("test_priv","test_pub");
	char *msg=public_encript("hola mundo","test_pub");
	char *dec=private_decript("test_priv",msg);
	printf("%s",dec);
	//read_public_key();
        return 0;
} 

*/
// para que sirve esta funcion ?? 

int passwd_callback(char *pcszbuff,int size,int rwflag,void *pPass){
	size_t unPass=strlen((char *)pPass);
	if(unPass>(size_t)size){
		unPass=(size_t)size;
	};
	memcpy(pcszbuff,pPass,unPass);
	return (int) unPass;
};


