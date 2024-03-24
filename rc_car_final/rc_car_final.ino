#include <ESP32Servo.h>
#include <SimpleBLE.h>
#include <BluetoothSerial.h>

char message = 'S';
const char *pin = "1234";  // Change this to more secure PIN.
String device_name = "HYPERNOVA";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig` to and enable it
#endif
#define byte dutyCycle 255;
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
int motor1Pin1 = 13;
int motor1Pin2 = 12;
int motor2Pin1 = 27;
int motor2Pin2 = 26;
int flag = 0;

const int freq = 50000;
const int pwmChannel = 0;  //setting up two pwm channels
const int pwmChannel1 = 1;
const int resolution = 8;
int dutyCycle = 200;  //initiating
int dutyCycle1 = 200;

Servo myservo;
const int ServoPin = 18;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
//Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChannel1, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(motor1Pin1, pwmChannel);   //pin 1 of motors to channel 0 for forward motiom
  ledcAttachPin(motor1Pin2, pwmChannel1);  //pin 2 of both motors to channel for backward motion
  ledcAttachPin(motor2Pin1, pwmChannel);
  ledcAttachPin(motor2Pin2, pwmChannel1);

  myservo.attach(ServoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available()) {
    message = SerialBT.read();
    Serial.println(message);
  }
  if (message == 'F') {
    if (flag != 1) {
      myservo.write(90);
    }
    ledcWrite(pwmChannel, 700);
    ledcWrite(pwmChannel1, 0);
    flag = 0;
    //forward();
  } else if (message == 'B') {
    if (flag != 1) {
      myservo.write(90);
    }
    ledcWrite(pwmChannel1, 700);
    ledcWrite(pwmChannel, 0);
    flag = 0;
    //reverse();
  } else if (message == 'L') {
    left();
    flag = 1;
  } else if (message == 'R') {
    right();
    flag = 1;

  } else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }
}

// void forward() {
//   myservo.write(90);  //servo in neutral position
//   dutyCycle1 = 0;
//   dutyCycle = 0;
//   ledcWrite(pwmChannel1, dutyCycle1);  //pin2 of both motors to low
//   while (SerialBT.read() == 'F') {
//     ledcWrite(pwmChannel, dutyCycle);
//     Serial.print("Forward with duty cycle: ");
//     Serial.println(dutyCycle);
//     dutyCycle = dutyCycle + 5;  //increasing duty cycle of pin1 of both motors for forward motion
//     if (dutyCycle >= 255)       //checking max value
//     {
//       dutyCycle = 255;
//     }
//     //delay(50);
//   }
//   //dutyCycle = 200;
// }

// void reverse() {
//   myservo.write(90);
//   dutyCycle = 0;  //pin 1 of both motors to low
//   dutyCycle1 = 0;
//   ledcWrite(pwmChannel, dutyCycle);
//   while (SerialBT.read() == 'B') {
//     ledcWrite(pwmChannel1, dutyCycle1);
//     Serial.print("Backward with duty cycle: ");  //increasing duty cycle of pin2 of both motors for reverse motion
//     Serial.println(dutyCycle1);
//     dutyCycle1 = dutyCycle1 + 5;
//     if (dutyCycle >= 255) {
//       dutyCycle = 255;
//     }
//     //delay(50);
//   }
//   //dutyCycle = 200;
// }

void left() {
  myservo.write(120);  //left turn
}
void right() {
  myservo.write(60);  //right turn
}