#include <stdio.h>
#include "command.h"
#include <uECC.h>

#define KEYDEBUG(curve) LOG("%s : %i %i", #curve, uECC_curve_private_key_size(curve), uECC_curve_public_key_size(curve))

int main(int argc, char const *argv[]) {
  LOG("Wallet Simulator");
  #ifdef _DEBUG
  KEYDEBUG(uECC_secp160r1());
  KEYDEBUG(uECC_secp192r1());
  KEYDEBUG(uECC_secp224r1());
  KEYDEBUG(uECC_secp256r1());
  KEYDEBUG(uECC_secp256k1());
  #endif
  command();
  return 0;
}
