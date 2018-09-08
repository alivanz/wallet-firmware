#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "ecc.h"
#include "serial.h"

// #define BUFLEN 0x800
// char buf[BUFLEN];

static void echo(){
  printf("DATA ");
  char c = uart_getchar();
  while(c!='\n'){
    uart_putchar(c);
    c = uart_getchar();
  }
  uart_putchar('\n');
  // DATA("%s", buf);
  OK();
}
static void echo2(){
  char buf[256];
  scanf("%s", buf);
  DATA("%s", buf);
  OK();
}
static void version(){
  DATA("1.0");
  OK();
}

func_t cmdlist[] = {
  {"echo", echo, "echo."},
  {"echo2", echo2, "echo."},
  {"version", version, "get version."},
  // {"list_wallet", list_wallet, "list all wallet."},
  {"sign_deterministic", sign_deterministic, "Deterministic signature. (key_id message_hash_hex)"},
  {"verify", verify, "Verify. (key_id message_hash_hex signature)"},
  {"set_pubkey", set_pubkey, "Set public key (key_id curve bin_hex)."},
  {"get_pubkey", get_pubkey, "Get public key (key_id)."},
  {"set_privkey", set_privkey, "Set private key (key_id curve bin_hex)."},
  // {"gen_privkey", set_pubkey, "Generate private key from TRNG (key_id curve)."},
  {0,0,0} // SENTINEL
};

#define CMD_LOOP(i) for(int i=0; cmdlist[i].name != 0; i++)

void help(){
  CMD_LOOP(i){
    DATA("%s\t%s", cmdlist[i].name, cmdlist[i].desc);
  }
  OK();
}

void command(){
  int found;
  char buf[64];
  #ifdef __AVR_ARCH__
  for(;;){
  #else
  for(;!feof(stdin);){
  #endif
    scanf("%64s", buf);
    // help
    if(strcmp(buf,"help")==0){
      help();
      continue;
    }
    // search
    found = 0;
    CMD_LOOP(i){
      if(strcmp(buf,cmdlist[i].name)==0){
        found = 1;
        func_call fx = cmdlist[i].fx;
        if(fx){
          fx();
        }else{
          ERRORF("function undefined");
        }
        break;
      }
    }
    if(!found){
      ERRORF("unknown command");
    }
  }
}
