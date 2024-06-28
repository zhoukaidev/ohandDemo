#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARDUINO_ARCH_STM32F1

#define SerialToDevice Serial3 // PB10: TX, PB11: RX

const int LED = PB1;  // On when high

#else

#define SerialToDevice Serial1

const int LED = 13;  // On when high
#endif

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


const uint8_t ADDRESS_MASTER     = 0x01;
const uint8_t ADDRESS_HAND       = 0x02;
const unsigned int TICK_INTERVAL = 1;




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

static const int g_ohandId [] = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
static const int g_initGestures [][7] {
  {0x02, 0, 0, 0, 0, 0, 0},
  {0x03, 0, 0, 0, 0, 0, 0},
  {0x04, 0, 0, 0, 0, 0, 0},
  {0x05, 0, 0, 0, 0, 0, 0},
  {0x06, 0, 0, 0, 0, 0, 0},
  {0x07, 0, 0, 0, 0, 0, 0},
  {0x08, 0, 0, 0, 0, 0, 0},
  {0x09, 0, 0, 0, 0, 0, 0}
};

#define SLEEP_ID 100
//OHand address at positon 0, sleep if Ohand address equal SLEEP_ID
static const int g_gestures [][7] = {
  {0x02, -1, -1, -1, -1, -1, 65535},
  {0x03, -1, -1, -1, -1, -1, 65535},
  {0x04, -1, -1, -1, -1, -1, 65535},
  {0x05, -1, -1, -1, -1, -1, 65535},
  {0x06, -1, -1, -1, -1, -1, 65535},
  {0x07, -1, -1, -1, -1, -1, 65535},
  {0x08, -1, -1, -1, -1, -1, 65535},
  {0x09, -1, -1, -1, -1, -1, 65535},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 37000, 29000, 32000, 30000, 30000, -1},
  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x05, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {0x07, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {0x09, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x02, 0, 0, 0, 0, 0, -1},
  {0x03, 0, 0, 0, 0, 0, -1},
  {0x04, 0, 0, 0, 0, 0, -1},
  {0x05, 0, 0, 0, 0, 0, -1},
  {0x06, 0, 0, 0, 0, 0, -1},
  {0x07, 0, 0, 0, 0, 0, -1},
  {0x08, 0, 0, 0, 0, 0, -1},
  {0x09, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 37000, 29000, 32000, 30000, 30000, -1},
  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x05, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {0x07, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {0x09, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 3000, 0, 0, 0, 0, 0},

  {0x02, 0, 0, 0, 0, 0, -1},
  {0x06, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x03, 0, 0, 0, 0, 0, -1},
  {0x07, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x04, 0, 0, 0, 0, 0, -1},
  {0x08, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x05, 0, 0, 0, 0, 0, -1},
  {0x09, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x05, 37000, 29000, 32000, 30000, 30000, -1},
  {0x09, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x03, 37000, 29000, 32000, 30000, 30000, -1},
  {0x07, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x02, 0, 0, 0, 0, 0, -1},
  {0x06, 0, 0, 0, 0, 0, -1},
  {0x04, 0, 0, 0, 0, 0, -1},
  {0x08, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 750, 0, 0, 0, 0, 0},

  {0x03, 0, 0, 0, 0, 0, -1},
  {0x05, 0, 0, 0, 0, 0, -1},
  {0x07, 0, 0, 0, 0, 0, -1},
  {0x09, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x02, 0, 0, 0, 0, 0, -1},
  {0x06, 0, 0, 0, 0, 0, -1},
  {0x04, 0, 0, 0, 0, 0, -1},
  {0x08, 0, 0, 0, 0, 0, -1},
  {0x05, 37000, 29000, 32000, 30000, 30000, -1},
  {0x09, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 37000, 29000, 32000, 30000, 30000, -1},
  {0x07, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 0, 0, 0, 0, 0, -1},
  {0x05, 0, 0, 0, 0, 0, -1},
  {0x07, 0, 0, 0, 0, 0, -1},
  {0x09, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x02, 0, 0, 0, 0, 0, -1},
  {0x06, 0, 0, 0, 0, 0, -1},
  {0x04, 0, 0, 0, 0, 0, -1},
  {0x08, 0, 0, 0, 0, 0, -1},
  {0x05, 37000, 29000, 32000, 30000, 30000, -1},
  {0x09, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 37000, 29000, 32000, 30000, 30000, -1},
  {0x07, 37000, 29000, 32000, 30000, 30000, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},

  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 0, 0, 0, 0, 0, -1},
  {0x05, 0, 0, 0, 0, 0, -1},
  {0x07, 0, 0, 0, 0, 0, -1},
  {0x09, 0, 0, 0, 0, 0, -1},
  {SLEEP_ID, 1000, 0, 0, 0, 0, 0},
  
  {0x02, 37000, 29000, 32000, 30000, 30000, -1},
  {0x03, 37000, 29000, 32000, 30000, 30000, -1},
  {0x04, 37000, 29000, 32000, 30000, 30000, -1},
  {0x05, 37000, 29000, 32000, 30000, 30000, -1},
  {0x06, 37000, 29000, 32000, 30000, 30000, -1},
  {0x07, 37000, 29000, 32000, 30000, 30000, -1},
  {0x08, 37000, 29000, 32000, 30000, 30000, -1},
  {0x09, 37000, 29000, 32000, 30000, 30000, -1},




};


#ifdef __cplusplus
}
#endif

#endif /*_CONFIG_H_*/
