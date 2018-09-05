extern "C"{
  #include "serial.h"
  #include "command.h"
}

void setup(){
  // uECC_set_rng(RNG);
  serial_init();
}

int main(){
  command();
  return 0;
}
