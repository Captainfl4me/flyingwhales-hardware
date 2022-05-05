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
  _____    _
  |  __ \  (_)
  | |__) |  _   _ __     ___    ___
  |  ___/  | | | '_ \   / __|  / _ \
  | |      | | | | | | | (__  |  __/
  |_|      |_| |_| |_|  \___|  \___|

  -----------------------------------

*/

#include <Servo.h>
#include <SPI.h>
#include <RF24.h>

//----------------------------------Pins
//SERVO
#define PIN_FRONT 3
#define PIN_MIDDLE 5
#define PIN_BACK 6

//NRF24
#define PIN_CE   7
#define PIN_CSN  8
//----------------------------------Servo
// de 700 a 2300
#define MINSERVO 700 //Valeur pour laquelle la pince est ouverte
#define MAXSERVO 2300 //Valeur pour laquelle la pince est fermee
Servo front;
Servo middle;
Servo back;
//----------------------------------NRF24
#define tunnel  "PINCE"
RF24 radio(PIN_CE, PIN_CSN);

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

struct { //STRUCTURE DE DONNE RECUES
  int value[4];
} dataReceived;

//----------------------------------Variables
int mode = -1;
bool modeUpdated;
bool externalControlMode = false;
void setup() {

  //SERVO
  front.attach(PIN_FRONT);
  middle.attach(PIN_MIDDLE);
  back.attach(PIN_BACK);

  //DEBUG
  //Serial.begin(9600);

  //RADIO
  radio.begin();                      // Initialisation du module NRF24
  radio.setPALevel(RF24_PA_MAX);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();             // Démarrage de l'écoute du NRF24 (signifiant qu'on va recevoir, et non émettre quoi que ce soit, ici)
}

void loop() {

  if (radio.available() > 0) {
    radio.read( &dataReceived, sizeof(dataReceived) );
    if (mode != dataReceived.value[0])
    {
      mode = dataReceived.value[0];
      modeUpdated = true;
    }
    if (false) {
      if (modeUpdated)
      {
        modeUpdated = false;
        switch (mode) {
          case 0: {
              OpenPince(1000);
              externalControlMode = false;
            } break;
          case 1: {
              ClosePince(1000);
              externalControlMode = false;
            } break;
          case 2: {
              externalControlMode = true;
            } break;
        }
      }

      if (externalControlMode)
      {
        if (dataReceived.value[1] >= MINSERVO && dataReceived.value[1] <= MAXSERVO)
          front.writeMicroseconds(dataReceived.value[1]);
        if (dataReceived.value[2] >= MINSERVO && dataReceived.value[2] <= MAXSERVO)
          back.writeMicroseconds(dataReceived.value[2]);
        if (dataReceived.value[3] >= MINSERVO && dataReceived.value[3] <= MAXSERVO)
          middle.writeMicroseconds(dataReceived.value[3]);
      }
    }
  }
}

void OpenPince(int tempsOuverture)//tempsOuverture est le temps que met la pince a s'ouvrir (en microsecondes)
{
  int delaytime = (int)(tempsOuverture / (MAXSERVO - MINSERVO));
  for (int i = MINSERVO; i < MAXSERVO; i++)
  {
    front.writeMicroseconds(i);
    back.writeMicroseconds(i);
    middle.writeMicroseconds(i);
    delay(delaytime);
  }
}

void ClosePince(int tempsOuverture)
{
  int delaytime = (int)(tempsOuverture / (MAXSERVO - MINSERVO));
  for (int i = MAXSERVO; i > MINSERVO; i--)
  {
    front.writeMicroseconds(i);
    back.writeMicroseconds(i);
    middle.writeMicroseconds(i);
    delay(delaytime);
  }
}
