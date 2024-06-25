#include <Arduino.h>
#include "OHandSerialAPI.h"
#include "config.h"


void recvDataUART()
{
  while (SerialToDevice.available() > 0)
  {
    uint8_t data = (uint8_t)SerialToDevice.read();
    HAND_OnData(data);
  }
}

void sendDataUART(uint8_t addr, uint8_t *data, uint8_t size)
{
  while (size-- > 0)
    SerialToDevice.write(*data++);
}

void errorHandler(uint8_t node_addr, uint8_t err)
{
}

void InitLED() {
  pinMode(LED, OUTPUT);
}

void OpenLED()
{
  digitalWrite(LED, LED_ON_STATE);
}

void CloseLED() {
  digitalWrite(LED, !LED_ON_STATE);
}

void setup() {
  uint8_t err;
  InitLED();
  OpenLED();
  Serial.begin(115200);  // for log ouput
  SerialToDevice.begin(115200); // ohand control port

  Serial.println("--> start to initialize ohand");
  HAND_SetDataInterface(HAND_PROTOCOL_UART, ADDRESS_MASTER, sendDataUART, recvDataUART);  // For non-interrupt receive mode, specify receive function.
  HAND_SetCommandTimeOut(255);
  HAND_SetTimerFunction(millis, delay);
  do
  {
    uint8_t major_ver, minor_ver;
    uint16_t revision;
    err = HAND_GetFirmwareVersion(ADDRESS_HAND, &major_ver, &minor_ver, &revision, errorHandler);

    if (err == HAND_RESP_SUCCESS)
    {
      Serial.println("read fireware success");

      if (major_ver < FW_VER_MAJOR_MIN || major_ver > FW_VER_MAJOR_MAX)
      {
        while (1)
        {

          Serial.println("fireware not support");
          digitalWrite(LED, LED_ON_STATE);
          delay(500);
          digitalWrite(LED, !LED_ON_STATE);
          delay(500);
        }
      }
    }
    else if (HAND_RESP_TIMEOUT == err)
    {
      Serial.println("timeout");
      digitalWrite(LED, LED_ON_STATE);
      delay(500);
      digitalWrite(LED, !LED_ON_STATE);
      delay(500);
    }
    else
    {
      //Serial.println(err);
      digitalWrite(LED, LED_ON_STATE);
      delay(2000);
      digitalWrite(LED, !LED_ON_STATE);
      delay(2000);
      delay(100);
    }
  } while (err != HAND_RESP_SUCCESS);

  //setup the PID
#ifdef SET_PID
  // Set PID
  for (int i = 0; i < NUM_MOTORS; i++)
  {
    do
    {
      err = HAND_SetFingerPID(ADDRESS_HAND, i, _pidGains[i][0], _pidGains[i][1], _pidGains[i][2], _pidGains[i][3], errorHandler);

      if (err != HAND_RESP_SUCCESS)
      {
        delay(1000);
      }
    } while (err != HAND_RESP_SUCCESS);
  }
#endif

  //ohand finger initialization
  // Open thumb
  err = HAND_SetFingerPos(ADDRESS_HAND, 0, 0, 255, errorHandler);
  if (err != HAND_RESP_SUCCESS)
  {
    Serial.println("HAND_SetFingerPos() thumb error");
  }

  // Open others
  for (int i = 1; i < NUM_FINGERS; i++)
  {
    err = HAND_SetFingerPos(ADDRESS_HAND, i, 0, 255, errorHandler);
    if (err != HAND_RESP_SUCCESS)
    {
      Serial.println("HAND_SetFingerPos() error");
    }
  }

  //ohand thumb root motor initialization
#ifdef HAS_THUMB_ROOT_MOTOR
  // Open thumb root
  err = HAND_SetFingerPos(ADDRESS_HAND, THUMB_ROOT_ID, 0, 255, errorHandler);
  if (err != HAND_RESP_SUCCESS)
  {
    Serial.println("-->init thumb root motor error");
  }
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
