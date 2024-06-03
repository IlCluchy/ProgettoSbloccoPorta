//dichiarazione librerie
#include "librerie.h"

//funzione setup
void setup() {
  init_setup();
}

//funzione loop
void loop() {
  //azzero il led
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDF, LOW);

  //azzero l'lcd
  lcd.clear();

  //imposto il buzzer a LOW
  noTone(BUZZER); 

  //inizializzo il server
  char echo_message[50];
  WiFiClient client = server.available();

  if(client.connected()){

    client = server.available();

    if(client.available()){
      //ricevo un segnale
      Serial.println("Client available");
      Serial.print("Received: ");
      //leggo il segnale ricevuto
      char received = client.read();

      Serial.println(received);

      //avviso
      Serial.println("Enter your card, fingerprint or code immediately after the next beep");
      delay(2000);

      //beep
      tone(BUZZER, 200);
      delay(100);
      noTone(BUZZER);

      //delay per permettere all'utente di reagire
      delay(300); 

      switch(received){
        //utilizzo il metodo scelto
        case '1': {
          digitalWrite(LED, HIGH);
          strcpy(echo_message, "metodo scelto improta digitale");
          client.write(echo_message);
          fingerPrint();
          break;
        }
        
        case '2': {
          digitalWrite(LED, HIGH);
          strcpy(echo_message, "metodo scelto tastiera a matrice");
          client.write(echo_message);
          keypad(client);
          break;
        }

        default: {
          digitalWrite(LED,  LOW);
          Serial.println("Error! char not properly received");
          strcpy(echo_message, "Error! char not properly received");
          client.write(echo_message);
          break;
        }

      }

    }
    //fermo il client
    client.stop();
    Serial.println("client disconnected");
  }
}