
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "UserInfo.h"

void SetUserInfo(char * u, char * p, char * vm, Credentials *crd){
    
    memset(crd, 0x00, sizeof(*crd));//Clean the credentials struct
    
    int UL = strlen(u);//get the legnth of the user name
    if(UL > Max_User_Legnth){//why this should have been checked sooner
        printf("\nUser is too long\n");
        return;
    }
    int PL = strlen(p);//get the legnth of the user name
    if(PL > Max_Password_Legnth){
        printf("\nPassword is too long\n");
        return;
    }
    int VL = strlen(vm);//get the legnth of the user name
    if(VL > Max_VM_Legnth){
        printf("\nVM is too long\n");
        return;
    }
    //copy the data to the strut
    memcpy(&(crd->user[0]),u,UL);
    crd->ULegnth = UL;
    memcpy(&crd->password,p,PL);
    crd->PLegnth = PL;
    memcpy(&crd->VM,vm,VL);
    crd->VLegnth = VL;
}
