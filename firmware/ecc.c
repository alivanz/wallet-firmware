#include <stdio.h>
#include <stdint.h>
#include <uECC.h>
#include "command.h"
#include "sha2.h"

uint8_t pubkey[64];
uint8_t privkey[64];
uECC_Curve curve;

unsigned char hex2c(unsigned char hex){
  if('0'<=hex && hex<='9') return hex-'0';
  if('a'<=hex && hex<='f') return hex-'a'+10;
  if('A'<=hex && hex<='F') return hex-'F'+10;
  return -1;
}

unsigned char * hex2bin(unsigned char *hex, unsigned char *out){
  size_t len = strlen(hex)/2;
  // unsigned char *out = malloc(len);
  for(size_t i=0; i<len; i++){
    out[i] = (hex2c(hex[i*2])<<4) | (hex2c(hex[i*2+1]));
  }
  return out;
  // return 1;
}

char * bin2hex(unsigned char *bin, unsigned int len){
  static char out[1024];
  for(unsigned int i=0; i<len; i++){
    sprintf_s(&out[i*2], sizeof(out), "%02x", bin[i]);
  }
  out[len*2] = 0;
  return out;
}

void set_pubkey(){
  int keyid;
  char type[64];
  char hex[192];
  uint8_t bin[sizeof(hex)/2];
  scanf_s("%i %s %s", &keyid, type, 64, hex, (unsigned int)sizeof(hex));
  // Check memory slot
  if(keyid==-1){

  }else{
    ERROR("invalid memory slot");
    return;
  }
  // Check curve
  #define IF_CURVE(name) if(strcmp(type,#name)==0){ curve = uECC_##name(); } else
  IF_CURVE(secp160r1)
  IF_CURVE(secp192r1)
  IF_CURVE(secp224r1)
  IF_CURVE(secp256r1)
  IF_CURVE(secp256k1)
  {
    ERROR("unknown curve");
    return;
  }
  hex2bin(hex, bin);
  if(!uECC_valid_public_key(bin, curve)){
    ERROR("invalid key");
    return;
  }
  memcpy(pubkey,bin,64);
  OK();
}

void set_privkey(){
  int keyid;
  char type[64];
  char priv_hex[65];
  uint8_t priv[sizeof(priv_hex)/2];
  uint8_t pub[64];
  scanf_s("%i %s %s", &keyid, type, 64, priv_hex, (unsigned int)sizeof(priv_hex));
  hex2bin(priv_hex, priv);
  // Check memory slot
  if(keyid==-1){

  }else{
    ERROR("invalid memory slot");
    return;
  }
  // Check curve
  uECC_Curve _curve;
  #define IF_CURVE(name) if(strcmp(type,#name)==0){ _curve = uECC_##name(); } else
  IF_CURVE(secp160r1)
  IF_CURVE(secp192r1)
  IF_CURVE(secp224r1)
  IF_CURVE(secp256r1)
  IF_CURVE(secp256k1)
  {
    ERROR("unknown curve");
    return;
  }
  // Compute pubkey
  if(!uECC_compute_public_key(priv,pub,_curve)){
    ERROR("failed to compute pubkey");
    return;
  }
  // Store
  curve = _curve;
  memcpy(privkey,priv,uECC_curve_public_key_size(curve));
  memcpy(pubkey,pub,uECC_curve_public_key_size(curve));
  OK();
}

void get_pubkey(){
  int keyid;
  scanf_s("%i", &keyid);
  if(keyid==-1){
    DATA("%s", bin2hex(pubkey,uECC_curve_public_key_size(curve)));
    OK();
  }
}

void verify(){
  int keyid;
  char msg_hex[65];
  char sig_hex[129];
  char msg[sizeof(msg_hex)/2];
  char sig[sizeof(sig_hex)/2];
  scanf_s("%i %s %s", &keyid, msg_hex, (unsigned int)sizeof(msg_hex), sig_hex, (unsigned int)sizeof(sig_hex));
  hex2bin(msg_hex, msg);
  hex2bin(sig_hex, sig);
  // Check key id
  if(keyid==-1){

  }else{
    ERROR("invalid memory slot");
  }
  // Verify
  if(!uECC_verify(pubkey, msg, strlen(msg_hex)/2, sig, curve)){
    ERROR("Invalid signature");
    return;
  }
  OK();
}

typedef struct SHA256_HashContext {
    uECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

static inline void init_SHA256(uECC_HashContext *base)
{
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    sha256_Init(&context->ctx);
}

static inline void update_SHA256(uECC_HashContext *base,
                                 const uint8_t *message,
                                 unsigned message_size)
{
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    sha256_Update(&context->ctx, message, message_size);
}

static inline void finish_SHA256(uECC_HashContext *base, uint8_t *hash_result)
{
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    sha256_Final(hash_result, &context->ctx);
}

void sign_deterministic(){
  static uint8_t tmp[32 + 32 + 64];
  static SHA256_HashContext ctx = {{&init_SHA256, &update_SHA256, &finish_SHA256, 64, 32, tmp}};
  int keyid;
  char msg_hex[65];
  char msg[sizeof(msg_hex)/2];
  char sig[64];
  scanf_s("%i %s", &keyid, msg_hex, (unsigned int)sizeof(msg_hex));
  hex2bin(msg_hex, msg);
  // Check key id
  if(keyid==-1){

  }else{
    ERROR("invalid memory slot");
  }
  // Signature
  if(!uECC_sign_deterministic(privkey, msg, SHA256_DIGEST_LENGTH, &ctx.uECC, sig, curve)){
    ERROR("Signature failed.");
    return;
  }
  // Print signature
  DATA("%s", bin2hex(sig, uECC_curve_public_key_size(curve)));
  OK();
}
