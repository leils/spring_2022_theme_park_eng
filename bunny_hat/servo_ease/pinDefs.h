
/*
 * Pin mapping table for different platforms
 *
 * Platform         Servo1      Servo2      Servo3      Analog
 * -----------------------------------------------------------
 * ESP32              5           18          19          A0
 */
#if !defined(LED_BUILTIN) && !defined(ESP32)
#define LED_BUILTIN PB1

#elif defined(ESP32)
#define SERVO1_PIN 5
#define SERVO2_PIN 18
#define SERVO3_PIN 19
#define SPEED_IN_PIN A0 // 36/VP
#define MODE_ANALOG_INPUT_PIN A3 // 39

#define SERVO_UNDER_TEST_PIN SERVO1_PIN

#define SPEED_OR_POSITION_ANALOG_INPUT_PIN SPEED_IN_PIN
#define POSITION_ANALOG_INPUT_PIN SPEED_IN_PIN

// for ESP32 LED_BUILTIN is defined as: static const uint8_t LED_BUILTIN 2
#endif
