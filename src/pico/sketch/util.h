#pragma once

uint16_t tos(uint8_t* bytes) {
  return ((uint32_t)bytes[1] << 8) | (uint32_t)bytes[0];
}

uint32_t tol(uint8_t* bytes) {
  return ((uint32_t)bytes[3] << 24) | ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[1] << 8) | (uint32_t)bytes[0];
}

uint16_t tos(char* bytes) {
  return tos((uint8_t*) bytes);
}

uint32_t tol(char* bytes) {
  return tol((uint8_t*) bytes);
}