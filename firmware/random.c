#include <stdint.h>
#include <stdlib.h>

int RNG(uint8_t *dest, unsigned size){
  // printf("RAND ");
  for(unsigned i=0; i<size; i++){
    dest[i] = rand();
    // printf("%02X", dest[i] % 255);
  }
  // printf("\n");
  return 1;
}
