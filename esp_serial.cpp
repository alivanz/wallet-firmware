#include <Arduino.h>
// #include <stdio.h>
//
#ifdef __cplusplus
extern "C" {
#endif
//
void serial_init(){
  Serial.begin(115200);
}

//const char *readString(){
//  return Serial.readString().c_str();
//}
//
// int printf(const char *format, ...){
//   va_list vargs;
//   va_start(vargs, format);
//   int ret = Serial.printf(format, vargs);
//   va_end(vargs);
//   return ret;
// }
//
// int scanf(const char *format, ...){
//   static char c[1024];
//   Serial.readBytesUntil('\n', c, sizeof(c));
//   va_list args;
//   va_start(args, format);
//   int ret = sscanf(c, format, args);
//   va_end(args);
//   return ret;
// }
//
#ifdef __cplusplus
}
#endif
