/*

impronta digitale:
  vcc --> 3.3V
  gnd --> gnd
  rx --> 1 (bianco)
  tx --> 0 (giallo)

servo motore:
  pin 8

lcd:
  sda --> A4/sda
  scl --> A5/scl

buzzer:
  vcc --> 7

tastiera a matrice:
  rows --> 14-15-16-17
  cols --> 2-3-4-5

*/

//dichiarazione librerie
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "WiFiS3.h"

//definizione principali costanti
//led per lo scehrmo lcd
#define LED_PIN 3
//pin buzzer
#define BUZZER 7

//pin led
#define LEDR 11
#define LEDF 12

char *SSID_WIFI = "ISILINE-29366";
char *PASS_WIFI = "pij7ioMoas";

//dichiarazione dei vari componenti
//dichiarazione del servo
Servo myservo;
//dichiarazione dell'lcd i2c
LiquidCrystal_I2C lcd(0x3f, 16, 2);
//dichiarazione variabili globali
//variabile posizione servo motore
int pos = 0;
//inizializzazione id per la lettura del tipo di accesso
uint8_t id;

int keyIndex = 0; 

#define PORT 22222  

#define LED 13
int status = WL_IDLE_STATUS;
WiFiServer server(PORT);

//funzione lettura numero seriale
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

//funzione che scrive sull'lcd
void typewriting(String messaggio){
  int lunghezza = messaggio.length();
  for(int i = 0; i < lunghezza; i++){
    lcd.print(messaggio[i]);
    delay(150);
  }
}

//funzione per scrivere lo status del wifi
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void riuscita(){
  //scrivo sulla seriale in caso di riuscita
      Serial.println("riuscita");

      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDF, LOW);
          
      //scrivo sull'lcd che il tutto è avvenuto con successo
      String riga1 = "autentificazione";
      String riga2 = "riuscita";
      lcd.clear();
      lcd.setCursor(0, 0);
      typewriting(riga1);
      lcd.setCursor(0, 1);
      typewriting(riga2);
      delay(1000);

      //inizializzo il buzzer
      tone(BUZZER, 200);
      delay(500);

      //aziono il servo motore
      for (pos = 0; pos < 150; pos += 1){
        myservo.write(pos);
        //delay(15);              
      }

      delay(3000);

      //lo faccio tornare alla posizione di partenza
      for (pos = 60; pos >= 45; pos -= 1){
        myservo.write(pos);
        delay(15);
      }
}

void fallita(){
  //scrivo sulla seriale in caso di fallimento

      Serial.println("fallita");

      digitalWrite(LEDR, LOW);
      digitalWrite(LEDF, HIGH);
          
      //scrivo sull'lcd che c'è stato un errore
      String riga1 = "autentificazione";
      String riga2 = "fallita";
      lcd.clear();
      lcd.setCursor(0, 0);
      typewriting(riga1);
      lcd.setCursor(0, 1);
      typewriting(riga2);
      delay(1000);
}