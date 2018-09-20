extern "C"{
  #include "serial.h"
  #include "command.h"
}

void setup(){
  // uECC_set_rng(RNG);
  serial_init();
}

int main(){
  setup();
  command();
  return 0;
}
