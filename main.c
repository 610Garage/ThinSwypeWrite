
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <string.h>
#include <ctype.h>

#include <nfc/nfc.h>

#include "utils/nfc-utils.h"
#include "utils/mifare.h"

#include "W_NFC.h"
#include "UserInfo.h"
#include "EDCrypt.h"

ReaderTag RT;
static mifareul_tag mtDump; //holds tag payload


int main(int argc, const char *argv[]){
    
    memset(&mtDump, 0x00, sizeof(mtDump));//clean the payload varible. You know what they say, clean varibles, messy desk.
    Credentials crd;//struct that holds the credentials that will be written to the card.
   //char key[32];
    char IV[16];
        
    //set and clean the user, password and vm adress varibles
    char password[MAX_PASSWORD_LEGNTH];
    memset(&password, 0x00, MAX_PASSWORD_LEGNTH);
    char s[Max_User_Legnth];
    memset(&s, 0x00, Max_User_Legnth);
    char p[Max_Password_Legnth];
    memset(&p, 0x00, Max_Password_Legnth);
    char v[Max_VM_Legnth];
    memset(&v, 0x00, Max_VM_Legnth);
    
    KEY key;
    
    
    /*
     *To Do
     * sanitize thease before sticking them into the struct
     */
    printf("Enter password\n");
    scanf("%128s",password);
    while ( getchar() != '\n' );//clear out the consule input
    printf("Enter User Name\n");//ask the user
    scanf("%12s", s);//grap the input, and place it into the opropreite varible
    while ( getchar() != '\n' );//clear out the consule input
    printf("Enter Password\n");//repeate
    scanf("%30s", p);
    while ( getchar() != '\n' );
    printf("Enter VM\n");
    scanf("%24s", v);
    while ( getchar() != '\n' );
    /*printf("Enter Key\n");
    while ( getchar() != '\n' );
    scanf("%32s",key);
    printf("Enter IV\n");
    while ( getchar() != '\n' );
    scanf("%16s",IV);*/
    
    keyGen(&key, password);
    
   SetUserInfo(s, p, v, &crd);//set the input into the strut

    TagInit(&RT);// start the tag reader

  
   /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);
  
  unsigned char NDEFPayload[300];
  int NDEFPayloadSize;
  
  /* Encrypt the plaintext */
  NDEFPayloadSize = encrypt ((unsigned char*)&crd, sizeof(crd), key.key, key.iv, NDEFPayload);
  
  NDDEF_Generate(NDEFPayload, NDEFPayloadSize, &mtDump);//gereate the ndef data from the credentials from the struct
  
  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)NDEFPayload, NDEFPayloadSize);

    
  write_card(RT,&mtDump);//Place all of that goodness on the card
  
  printf("Done.\n");//tell the user

  //clean up
  nfc_close(RT.device);
  nfc_exit(RT.context);
  exit(EXIT_SUCCESS);
}