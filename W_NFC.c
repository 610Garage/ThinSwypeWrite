#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>

#include <nfc/nfc.h>

#include "utils/nfc-utils.h"
#include "utils/mifare.h"

#include "W_NFC.h"

#include "UserInfo.h"


char mdefMSG[MAX_MDEF_MSG_LENGTH];
static mifare_param mp;

static void print_success_or_failure(bool bFailure, uint32_t *uiCounter){
    
  printf("%c", (bFailure) ? 'x' : '.');
  if (uiCounter){
    *uiCounter += (bFailure) ? 0 : 1;
  }
}

void NDDEF_Generate(char * Payload, int l, mifareul_tag * tag){
    
  memset(mdefMSG,0,MAX_MDEF_MSG_LENGTH);
  memcpy(mdefMSG, DefualtHeader,DEFUALT_HEADER_LEGNTH);
  
  if(l >= (MAX_MDEF_MSG_LENGTH -DEFUALT_HEADER_LEGNTH)){
      printf("Mdef Payload too large. Payload size %i, max size %i",l,MAX_MDEF_MSG_LENGTH-DEFUALT_HEADER_LEGNTH);
      exit(-1);
  }
  mdefMSG[TLV_L] = l+DEFUALT_HEADER_LEGNTH-2;
  mdefMSG[PAYLOAD_LEGNTH] = l;
  memcpy(&mdefMSG[DEFUALT_HEADER_LEGNTH],Payload,l);
  
  int count =1;
  int count2 = 0;
  while(1){
      if(DEFUALT_HEADER_LEGNTH+l-count2 > 16){
        memcpy(&(tag->amb[count].mbd.abtData[0]), &mdefMSG[count2],16);
      }else if(DEFUALT_HEADER_LEGNTH+l-count2 <= 0){
          break;
      }else{
        memcpy(&tag->amb[count].mbd.abtData[0], &mdefMSG[count2],DEFUALT_HEADER_LEGNTH+l-count2);
      }
      count++;
      count2 += 16;
  }
}

bool write_card(ReaderTag rt,mifareul_tag * tag){
    
  uint32_t uiBlock = 0;
  bool    bFailure = false;
  uint32_t uiWritenPages = 0;
  uint32_t uiSkippedPages;

  char    buffer[BUFSIZ];
  bool    write_otp=0;
  bool    write_lock=0;

 
 
  uiSkippedPages = 2;

  for (int page = 0x2; page <= 30; page++) {
    if ((page == 0x2) && (!write_lock)) {
      printf("s");
      uiSkippedPages++;
      continue;
    }
    if ((page == 0x3) && (!write_otp)) {
      printf("s");
      uiSkippedPages++;
      continue;
    }
    // Show if the readout went well
    if (bFailure) {
      // When a failure occured we need to redo the anti-collision
      if (nfc_initiator_select_passive_target(rt.device, nmMifare, NULL, 0, &rt.nt) <= 0) {
        ERR("tag was removed");
        return false;
      }
      bFailure = false;
    }
    // For the Mifare Ultralight, this write command can be used
    // in compatibility mode, which only actually writes the first
    // page (4 bytes). The Ultralight-specific Write command only
    // writes one page at a time.
    uiBlock = page / 4;
    memcpy(mp.mpd.abtData, tag->amb[uiBlock].mbd.abtData + ((page % 4) * 4), 16);
    if (!nfc_initiator_mifare_cmd(rt.device, MC_WRITE, page, &mp))
      bFailure = true;

    print_success_or_failure(bFailure, &uiWritenPages);
  }
  printf("|\n");
  printf("Done, %d of 16 pages written (%d pages skipped).\n", uiWritenPages, uiSkippedPages);

  return true;
}

void TagInit(ReaderTag *rt){
    
  
  nfc_init(&rt->context);
  if (rt->context == NULL) {
    ERR("Unable to init libnfc (malloc)");
    exit(EXIT_FAILURE);
  }

  // Try to open the NFC device
  rt->device = nfc_open(rt->context, NULL);
  if (rt->device == NULL) {
    ERR("Error opening NFC device");
    nfc_exit(rt->context);
    exit(EXIT_FAILURE);
  }

  if (nfc_initiator_init(rt->device) < 0) {
    nfc_perror(rt->device, "nfc_initiator_init");
    nfc_close(rt->device);
    nfc_exit(rt->context);
    exit(EXIT_FAILURE);
  }

  // Scan until the cows come home, or the tag, which ever comes first.
  if (nfc_device_set_property_bool(rt->device, NP_INFINITE_SELECT, true) < 0) {
    nfc_perror(rt->device, "nfc_device_set_property_bool");
    nfc_close(rt->device);
    nfc_exit(rt->context);
    exit(EXIT_FAILURE);
  }

  printf("NFC device: %s opened\n", nfc_device_get_name(rt->device));
  printf("\nWaiting for tag\n");

  // Try to find a MIFARE Ultralight tag
  if (nfc_initiator_select_passive_target(rt->device, nmMifare, NULL, 0, &rt->nt) <= 0) {
    ERR("no tag was found\n");
    nfc_close(rt->device);
    nfc_exit(rt->context);
    exit(EXIT_FAILURE);
  }
  // Test if we are dealing with a MIFARE compatible tag

  if (rt->nt.nti.nai.abtAtqa[1] != 0x44) {
    ERR("tag is not a MIFARE Ultralight card\n");
    nfc_close(rt->device);
    nfc_exit(rt->context);
    exit(EXIT_FAILURE);
  }
  // Get the info from the current tag
  printf("Found MIFARE Ultralight card with UID: ");
  size_t  szPos;
  for (szPos = 0; szPos < rt->nt.nti.nai.szUidLen; szPos++) {
    printf("%02x", rt->nt.nti.nai.abtUid[szPos]);
  }
  printf("\n");
}
