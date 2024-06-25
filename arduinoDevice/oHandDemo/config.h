#pragma once


#define SerialToDevice Serial1

const int LED = LED_BUILTIN;  // On when high
#define LED_ON_STATE 1

#define SET_PID   // Uncomment this if you want to set PID

#define HAS_THUMB_ROOT_MOTOR
#define NUM_FINGERS (5)

#ifdef HAS_THUMB_ROOT_MOTOR
# define NUM_MOTORS     (6)
# define THUMB_ROOT_ID  (5)
#else
# define NUM_MOTORS     (NUM_FINGERS)
#endif


//PID configuration
#ifdef SET_PID
static const float _pidGains[][4] =
{
  {250.00, 1.00, 250.00, 1.00},
  {250.00, 1.00, 250.00, 0.25},
  {250.00, 1.00, 250.00, 0.25},
  {250.00, 1.00, 250.00, 0.25},
  {250.00, 1.00, 250.00, 0.25},
#ifdef HAS_THUMB_ROOT_MOTOR
  {250.00, 1.00, 250.00, 1.00}
#endif
};
#endif


//OHAND address
const uint8_t ADDRESS_MASTER     = 0x01;
const uint8_t ADDRESS_HAND       = 0x02;
const unsigned int TICK_INTERVAL = 1;

//Wifi configuration
#define SECRET_SSID "vmilabs"
#define SECRET_PASS "vmilabs1234!"
