/* 
 * File:   W_NFC.h
 * Author: 640bob
 *
 * Created on November 8, 2015, 1:02 AM
 */

#ifndef W_NFC_H
#define	W_NFC_H

#include <nfc/nfc.h>
#include "utils/nfc-utils.h"
#include "utils/mifare.h"

#include "UserInfo.h"

#define MAX_MDEF_MSG_LENGTH 1024
#define PAYLOAD_LEGNTH 4
#define DEFUALT_HEADER_LEGNTH 6
#define TLV_L 1

static const char DefualtHeader[] = { 0x03, 0, 0xD1,0x01, 0, 'T'};

static const nfc_modulation nmMifare = {
  .nmt = NMT_ISO14443A,
  .nbr = NBR_106,
};

typedef struct {
  nfc_device *device;
  nfc_target nt;
  nfc_context *context;
} ReaderTag;



void NDDEF_Generate(char * Payload,int l, mifareul_tag * tag);
void TagInit(ReaderTag *rt);

bool write_card(ReaderTag rt,mifareul_tag * tag);

#endif	/* W_NFC_H */
