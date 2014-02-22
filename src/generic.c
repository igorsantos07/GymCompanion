#include <pebble.h>

char itoc(uint16_t number) { return number + 64; }
uint16_t ctoi(char letter) { return letter - 64; }
