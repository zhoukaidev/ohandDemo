#pragma once

#include "OHandSerialAPI.h"
#include "config.h"
#include "Arduino.h"

volatile uint16_t g_fingerPos[] = {0, 0, 0, 0, 0, 0};
void errorHandler(uint8_t node_addr, uint8_t err) {
  
}

bool setFingerPosition(uint8_t finger_id, uint16_t pos) {
  Serial.print("-->ohand finger:");
  Serial.print(finger_id);
  Serial.print(" pos:");
  Serial.println(pos);
  uint8_t err = HAND_SetFingerPos(ADDRESS_HAND, finger_id, pos, 255, errorHandler);
  if(err == HAND_RESP_SUCCESS) {
    g_fingerPos[finger_id] = pos;
    return true;
  }
  return false;
}
