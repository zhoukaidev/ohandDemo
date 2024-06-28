// Suported board: Arduino Mega 2560, Generic STM32F1 Series BluePill F103C8
//

#include <Arduino.h>

#include "retarget.h"
#include "log.h"
#include "OHandSerialAPI.h"
#include "config.h"

// -------------------------------------------------------
// user functions for OHand

void recvDataUART()
{
  while (SerialToDevice.available() > 0)
  {
    uint8_t data = (uint8_t)SerialToDevice.read();
    HAND_OnData(data);
    //    Serial.print(data, HEX);
    //    Serial.print(" ");
  }
}

void sendDataUART(uint8_t addr, uint8_t *data, uint8_t size)
{
  while (size-- > 0)
    SerialToDevice.write(*data++);
}


void errorHandler(uint8_t node_addr, uint8_t err)
{
  LOG_E("NODE: 0x%02x, ERROR: 0x%02x\n", node_addr, err);
}

bool setFingerPosition(uint8_t hand_id, uint8_t finger_id, uint16_t pos) {
  uint8_t err = HAND_SetFingerPos(hand_id, finger_id, pos, 255, errorHandler);
  if (err == HAND_RESP_SUCCESS) {
    return true;
  }
  return false;
}

bool initOhand() {
  int gestureRows = sizeof(g_initGestures) / sizeof(g_initGestures[0]);

  for (int i = 0; i < gestureRows; ++i) {
    int hand_id = g_initGestures[i][0];
    for (int fingerIndex = 1; fingerIndex < 7; ++fingerIndex) {
      if (g_initGestures[i][fingerIndex] == -1) {
        continue;
      }
      setFingerPosition(hand_id, fingerIndex - 1, g_initGestures[i][fingerIndex]);
    }
  }
}

void setup()
{
  delay(1000);
  uint8_t err;

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LED_ON_STATE);
  Serial.begin(115200);           // start serial for output
  //while (!Serial);

  SerialToDevice.begin(115200);

  retarget();
  log_set_level(LOG_LVL_DEBUG);

  while (millis() < 4000)
    ;    // Let oHand boot

  LOG_D("Begin.\n");

  HAND_SetDataInterface(HAND_PROTOCOL_UART, ADDRESS_MASTER, sendDataUART, recvDataUART);  // For non-interrupt receive mode, specify receive function.
  HAND_SetCommandTimeOut(255);
  HAND_SetTimerFunction(millis, delay);

  int ohandCount = sizeof(g_ohandId) / sizeof(int);
  for (int i = 0; i < ohandCount; ++i) {
    do
    {
      uint8_t major_ver, minor_ver;
      uint16_t revision;

      err = HAND_GetFirmwareVersion(g_ohandId[i], &major_ver, &minor_ver, &revision, errorHandler);
      
      LOG_I("HAND_GetFirmwareVersion() returned 0x%02x, handid:%02x\n", err, g_ohandId[i]);

      if (err == HAND_RESP_SUCCESS)
      {
        LOG_I("major_ver: 0x%02d\n", major_ver);

        if (major_ver < FW_VER_MAJOR_MIN || major_ver > FW_VER_MAJOR_MAX)
        {
          while (1)
          {
            LOG_F("OHand firmware version '0x%02x' not matches SDK's '0x%02x'-'0x%02x'\n", major_ver, FW_VER_MAJOR_MIN, FW_VER_MAJOR_MAX);

            digitalWrite(LED, LED_ON_STATE);
            delay(500);
            digitalWrite(LED, !LED_ON_STATE);
            delay(500);
          }
        }
      }
      else
      {
        delay(100);
      }
    } while (err != HAND_RESP_SUCCESS);
  }


#ifdef SET_PID
  // Set PID
  for (int ohandIndex = 0; ohandIndex < ohandCount; ++ohandIndex) {
    for (int i = 0; i < NUM_MOTORS; i++)
    {
      do
      {
        err = HAND_SetFingerPID(g_ohandId[ohandIndex], i, _pidGains[i][0], _pidGains[i][1], _pidGains[i][2], _pidGains[i][3], errorHandler);

        if (err != HAND_RESP_SUCCESS)
        {
          delay(1000);
          LOG_E("HAND_SetFingerPID() returned 0x%02x\n", err);
        }
      } while (err != HAND_RESP_SUCCESS);
    }
  }
#endif

  initOhand();
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED, LED_ON_STATE);
    delay(500);
    digitalWrite(LED, !LED_ON_STATE);
    delay(500);
  }
}

void loop()
{
  int gestureRows = sizeof(g_gestures) / sizeof(g_gestures[0]);

  for (int i = 0; i < gestureRows; ++i) {
    if (g_gestures[i][0] == SLEEP_ID) {
      delay(g_gestures[i][1]);
      continue;
    }
    int hand_id = g_gestures[i][0];
    for (int fingerIndex = 1; fingerIndex < 7; ++fingerIndex) {
      if (g_gestures[i][fingerIndex] == -1) {
        continue;
      }
      setFingerPosition(hand_id, fingerIndex - 1, g_gestures[i][fingerIndex]);
    }
  }
}
