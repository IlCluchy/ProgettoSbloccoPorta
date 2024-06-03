#include <Adafruit_Fingerprint.h>
#include "Configurazione.h"

/*
##########################################

              fingerPrint

##########################################
*/

//controllo della seriale per il sensore di impronta digitale
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);

#else
#define mySerial Serial1

#endif

//inizializzazione del sensore di impronta digitale
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// returns 0 if failed, otherwise returns ID #
int getFingerprintIDez(uint8_t p) {
  //uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return 0;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return 0;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return 0;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

//funzione che prende l'impronta digitale
uint8_t getFingerprintID( uint8_t p) {

  //uint8_t p = finger.getImage();
  //controllo che l'immagine sia stata presa o no
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }

  // OK success!
  //cerco di convertire l'immagine
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }
  //cerco se c'è una corrispondenza tra l'impronta trovata e quelle memorizzate
  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return 0;
  } else {
    Serial.println("Unknown error");
    return 0;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

void fingerPrint(){
  //variabile per la lettura dell'impronta digitale e per il tastierino
  int verify = 1;
  int finger_verify = 0;

  //dichiaro la variabile per la lettura dell'impronta digitale
  uint8_t p = finger.getImage();

  while((verify = getFingerprintID(p)) > 0){
    Serial.println(verify);
    if(verify != 0 && verify != 255){
      finger_verify = verify;
    }
  }

  if(finger_verify)
    riuscita();
  else{
    fallita();
        
  }
}