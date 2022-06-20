#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>
static int passwd_callback(char *pcszbuff,int size,int rwflag,void *pPass);

int generate_key()
{
	int				ret = 0;
	RSA				*r = NULL,*rpub=NULL;
	BIGNUM			*bne = NULL;
	BIO				*bp_public = NULL, *bp_private = NULL;

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
	bp_public = BIO_new_file("public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	
	if(ret != 1){
		goto free_all;
	}
	


	// 3. save private key
	bp_private = BIO_new_file("private.pem", "w+");
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

int main(int argc, char* argv[]) 
{
    
	generate_key();
	read_public_key();
        return 0;
}

int passwd_callback(char *pcszbuff,int size,int rwflag,void *pPass){
	size_t unPass=strlen((char *)pPass);
	if(unPass>(size_t)size){
		unPass=(size_t)size;
	};
	memcpy(pcszbuff,pPass,unPass);
	return (int) unPass;
};
