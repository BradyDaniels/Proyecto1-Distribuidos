#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>



int main(){
    static const char key[] = "-----BEGIN PUBLIC KEY-----\n" 
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC7vbqajDw4o6gJy8UtmIbkcpnk\n" 
    "O3Kwc4qsEnSZp/TR+fQi62F79RHWmwKOtFmwteURgLbj7D/WGuNLGOfa/2vse3G2\n" 
    "eHnHl5CB8ruRX9fBl/KgwCVr2JaEuUm66bBQeP5XeBotdR4cvX38uPYivCDdPjJ1\n" 
    "QWPdspTBKcxeFbccDwIDAQAB\n" 
    "-----END PUBLIC KEY-----\n"; 
 
    BIO* bio = BIO_new_mem_buf(key, (int)sizeof(key));
    BIO* f_bio=BIO_new_file("public_convert.pem", "w+"); 
    
    RSA *pkey=RSA_new();
    PEM_read_bio_RSA_PUBKEY(bio,pkey,NULL,NULL);
    PEM_write_bio_RSAPublicKey(f_bio,pkey);


    
    
    EVP_PKEY_free(pkey); 
    BIO_free(bio);
}

