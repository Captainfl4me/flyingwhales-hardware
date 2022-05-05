/*
  Code by :
  Nicolas BLIN
  Arthur GERST
  Augustin LALAU
  Tifenn LE COLLEN
  Laurine LOHEZ
  Vianney SAADETIAN
  Nicolas THIERRY

  2021-2022
  _____  _____  _____ ______   ___
  /  ___||_   _||  ___|| ___ \ / _ \
  \ `--.   | |  | |__  | |_/ // /_\ \
  `--. \  | |  |  __| |    / |  _  |
  /\__/ / _| |_ | |___ | |\ \ | | | |
  \____/  \___/ \____/ \_| \_|\_| |_/

  -----------------------------------
  _____
  |  __ \
  | |__) |  ___
  |  ___/  / __|
  | |     | (__
  |_|      \___|

  -----------------------------------

*/

#include <SPI.h>
#include <RF24.h>
#include <ArduinoJson.h>

//DEF PIN
#define PIN_CE   7
#define PIN_CSN  8
//---------------------------NRF24
RF24 radio(PIN_CE, PIN_CSN);
const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

//[0] send mode [1 : 4] send microseconds
struct {
  int value[4] = {0, 0, 0, 0};
} dataToSend;

int input = 0;
int readIndex = 0;
int mode = 0;

int valeurpot1 = 0;
int valeurpot2 = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Init nano...");
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3, 5);
  radio.openWritingPipe(slaveAddress);
  Serial.println("Ready !");
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 1) {
    StaticJsonDocument<200> doc;

    DeserializationError error = deserializeJson(doc, Serial);
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    checkMode(doc);
  }
}

void checkMode(StaticJsonDocument<200> doc) {
  if(doc["m"])
    mode = doc["m"];
  uint16_t pwm_1 = doc["pwm_1"] ? doc["pwm_1"]: 0;
  uint16_t pwm_2 = doc["pwm_2"] ? doc["pwm_2"]: 0;
  switch (mode) {
    case 1: {
        Serial.println("Open all");
        dataToSend.value[0] = 0;
        sendData();
      } break;
    case 2: {
        Serial.println("Close all");
        dataToSend.value[0] = 1;
        sendData();
      } break;
    case 3: {
        Serial.println("Control ");
        dataToSend.value[0] = 2;
        valeurpot1 += (pwm_1);
        valeurpot1 -= (pwm_2);
        if(valeurpot1 <= 0)
          valeurpot1 = 0;
        if(valeurpot1 >= 25500)
          valeurpot1 = 25500;
        const int mapValeur1 = map(valeurpot1, 0, 25500, 700, 2300);
        dataToSend.value[1] = mapValeur1;
        dataToSend.value[2] = mapValeur1;
        dataToSend.value[3] = map(valeurpot2, 0, 660, 700, 2300);
        sendData();
      } break;
  }
}

void sendData() {
  bool rslt;
  rslt = radio.write( &dataToSend, sizeof(dataToSend) );
  // Always use sizeof() as it gives the size as the number of bytes.
  // For example if dataToSend was an int sizeof() would correctly return 2
  /*
  Serial.print("Data Sent ");
  if (rslt) {
    Serial.println("  Acknowledge received");
  }
  else {
    Serial.println("  Tx failed");
  }*/
}
