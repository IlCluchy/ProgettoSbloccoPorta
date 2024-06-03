#include "keypad.h"

//dichiarazione server
char *ssid = SSID_WIFI;
char *pass = PASS_WIFI;

void init_setup(){
  //inizializzazione della seriale
  Serial.begin(9600);     
  pinMode(LEDR, OUTPUT);   
  pinMode(LEDF, OUTPUT);      


  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }


  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   


    status = WiFi.begin(ssid, pass);
 
    delay(10000);
  }
  server.begin();                          
  printWifiStatus();

  //inizializzo la seriale
  Serial.begin(9600);

  //inizializzo lcd i2c
  lcd.init();
  lcd.backlight();
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 50);

  //inizializzo il servo motore
  myservo.attach(8);

  //inizializzo il buzzer
  pinMode(BUZZER, OUTPUT);

  //controllo la seriale
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  //inizializzo la seriale per la lettura dell'impronta digitale
  finger.begin(57600);

  delay(5);

  //controllo se il sensore è rilevato
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  //controllo che il sensore abbia delle impronte memorizzate
  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }

  //inizializzazione del tastierino
  init_keypad();

  //inizializzazione led
  pinMode(LED, OUTPUT);
}