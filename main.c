
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
int count;


/**
 * Get a single line from the counsole and place it into the buffer
 * @param buffer
 * A pointer to the buffer that the line will be stored
 * @return 
 * returns the number of elements in the buffer
 */
int GetCLine(char * buffer, int MaxL){
    int c = 0;//holds a character form the counsole stream
    int count = 0;//number of chars in buffer
    
    memset(buffer, 0x00, MaxL);//clean the buffer
    
    while(count <= 0){
    
        while((c=fgetc(stdin)) != '\n' && count < MaxL){//get the char from console, see if the line is over, make sure were not overruning buffer
            buffer[count++] = (char)c;//throw the char into the buffer and incrament the char counter/buffer position
        }
        if(count >= MaxL){
            while ( getchar() != '\n' );
            printf("too long, please keep input less than %i\n",MaxL);
            memset(buffer, 0x00, MaxL);//clean the buffer
            count = 0;
        }else if(memchr(buffer,'\"',MaxL)){
            printf("Cannot contain quotation marks\n");
            memset(buffer, 0x00, MaxL);//clean the buffer
            count = 0;
        } else if(count == 0){
            printf("input cannot be empty\n");        
        }   
    }
        
    //buffer[count++] = '\n';//the while loop discards the new line char, so lets pop it back in
    return(count);//tell the caller the number of chars in buffer
}

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
    char p[MAX_VM_PASSWORD_LEGNTH];
    memset(&p, 0x00, MAX_VM_PASSWORD_LEGNTH);
    char v[Max_VM_Legnth];
    memset(&v, 0x00, Max_VM_Legnth);
    
    KEY key;
    
    printf("Enter encryption password\n");
    GetCLine(password,MAX_PASSWORD_LEGNTH);
    printf("Enter User Name\n");//ask the user
    GetCLine(s,Max_User_Legnth);
    printf("Enter Password\n");//repeate
    GetCLine(p,MAX_VM_PASSWORD_LEGNTH);
    printf("Enter VM\n");
    GetCLine(v,Max_VM_Legnth);
    
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