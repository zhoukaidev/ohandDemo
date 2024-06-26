 #include <Arduino.h>
#include "config.h"
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <arduino-timer.h>
#include "jsonmethod.h"
#include "ohand.h"


//global variable
WiFiClient g_wifiClient;
MqttClient g_mqttClient(g_wifiClient);
auto g_WiFiMonitorTimer = timer_create_default();
auto g_DevicePingTimer = timer_create_default();

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

bool checkWifiConnections(void *) {
  static bool g_wifiConnected = false;
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("-->Try connect to wifi");
    if(WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED) {
      Serial.println("-->connect to wifi failed");
      return true;
    }
    Serial.println("-->connect to wifi OK");
  }

  if(WiFi.status() == WL_CONNECTED) {
    if(!g_mqttClient.connected()) {
       Serial.println("-->start connect to mqttServer");
       if(g_mqttClient.connect(g_broker, g_brokerPort)) {
          Serial.println("-->start connect to mqttServer OK");
          g_mqttClient.subscribe(g_serviceTopic);
       } else {
          Serial.println(g_mqttClient.connectError()); 
       }
    }
  }
  return true;
}

bool devicePings(void *) {
  Serial.println("-->start device ping");
  if(!g_mqttClient.connected()) {
    Serial.println("-->ping fail, since no connection");
    return true;
  }
  auto pingMsg = generate_device_ping_msg(1);
  String pingMsgStr = "";
  serializeJson(pingMsg, pingMsgStr);
  g_mqttClient.beginMessage(g_pingTopic);
  g_mqttClient.print(pingMsgStr);
  g_mqttClient.endMessage();
  return true;  
}

void setup() {
  uint8_t err;
  InitLED();
  OpenLED();

  //mqtt init
  g_mqttClient.setUsernamePassword(g_brokerUser, g_brokerPwd);
  g_mqttClient.onMessage(onMqttMessage);
  
  Serial.begin(115200);  // for log ouput
  SerialToDevice.begin(115200); // ohand control port


// while(1)
// {
//       uint8_t message[] = {0x55, 0xaa, 0x02, 0x01, 0x01, 0x00, 0x02};
//      SerialToDevice.write(message, sizeof(message));
//      int delaycount = 0;
//      digitalWrite(LED, LED_ON_STATE);
//      while(1)
//      {
//        while(SerialToDevice.available() > 0){Serial.println("here");int  bs = SerialToDevice.read(); Serial.println(bs, DEC);}
//        delay(1);
//        delaycount += 1;
//        if(delaycount >=5000){break;}
//      } 
//      digitalWrite(LED, !LED_ON_STATE);
//      delay(1000);
// }
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

  g_WiFiMonitorTimer.every(12000, checkWifiConnections);
  g_DevicePingTimer.every(17000, devicePings);
  CloseLED();
}

void loop() {
  // put your main code here, to run repeatedly:
  g_WiFiMonitorTimer.tick();
  g_DevicePingTimer.tick();
  g_mqttClient.poll();
}

void onMqttMessage(int messageSize) {
  Serial.print("topic");
  Serial.println(g_mqttClient.messageTopic());
  StaticJsonDocument<1024> request;
  DeserializationError error = deserializeJson(request, g_mqttClient);
  if(error) {
    Serial.print("-->deser failed:");
    Serial.println(error.f_str());
    return;
  }
  if(!request.containsKey("Request")) {return;}
  StaticJsonDocument<1024> actionObj;
  const char * requestStr = request["Request"];
  error = deserializeJson(actionObj, requestStr);
  if(error) {
    Serial.print("-->deser action failed:");
    Serial.println(error.f_str());
    return;
  }
  const char* method = actionObj["method"];
  if(strcmp(method, "thing.service.finger.set")) {return;}
  const char* paras = actionObj["inputData"]["fingers"];
  String parasStr = paras;
  int findStart = 0;
  for(int i=0; i < 5; i++) {
    auto endIndex = parasStr.indexOf(',',findStart);
    if(endIndex == -1){ Serial.println("-->invlaid paras"); return; }
    auto subStr = parasStr.substring(findStart, endIndex);
    findStart = endIndex + 1;
    if(subStr.length() == 0) { continue; }
    int vint = subStr.toInt();
    setFingerPosition(i, vint);
    
    if(i == 4){
      auto lastSubStr = parasStr.substring(findStart);
      if(lastSubStr.length() == 0) { continue; }
      int vint = lastSubStr.toInt();
      setFingerPosition(5, vint);
    }
  }
  
  
//  String pingMsgStr = "";
//  serializeJson(doc, pingMsgStr);
//  Serial.println(pingMsgStr);
//  while(g_mqttClient.available()) {
//     Serial.print((char)g_mqttClient.read());
//  }
  Serial.println();
  Serial.println();
}
