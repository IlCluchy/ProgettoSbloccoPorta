#include "fingerprint.h"

/*
##########################################

          TASTIERINO NUMERICO

##########################################
*/

//righe e colonne per il tastierino
#define ROWS 4
#define COLS 4

//dichiarazione elementi per lettura tastierino
const char tastierino[ROWS][COLS]={
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//pin tastierino
const int rowPins[ROWS]={14, 15, 16, 17};
const int colPins[COLS]={2, 3, 4, 5};

//inizializzazione lunghezza e il tipo di password
const int password_lenght = 4;
char PASSWORD[password_lenght] = {'1', '2', '3', '\0'};

//funzione per inizializzare il tastierino
void init_keypad(){
  //metto le righe ad output
  for(int i = 0; i < ROWS; i++){
    pinMode(rowPins[i], OUTPUT);
  }
  //metto le colonne a input e inizializzo il pull up interno
  for(int i = 0; i < COLS ; i++){
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

//funzione per la conversione in carattere
char return_tasto(int i, int j){
  return tastierino[i][j];
}

//funzione per la lettura del tastierino
char read_keypad(){
  char taste;
  //faccio un for per controllare le colonne
  for(int i = 0; i < ROWS; i++){
    //inizializzo ogni colonna a 1
    for(int k = 0; k < ROWS; k++){
      digitalWrite(rowPins[k], HIGH);
    }
    //setto la colonna a 0
    digitalWrite(rowPins[i], LOW);
    //faccio un for per controllare se qualcosa è stato premuto
    for(int j = 0; j < COLS; j++){

      delay(2);
      //controllo se qualcosa è stato premuto        
      if(!digitalRead(colPins[j])){
        //leggo il tasto
        taste = return_tasto(i, j);
        //scrivo il tasto
        Serial.print("premuto il tasto: ");   
        Serial.println(taste); 
      }
    }
  }
  //ritorno il tasto letto
  return taste;
}

//funzione per comparare le stringhe e verificare le password
int compara_stringhe(char *s1, char *s2){
	int l1 = strlen(s1);
	int l2 = strlen(s2);
  //se hanno una lunghezza diversa ritorno -1
	if(l1 != l2 && l1 > 0 && l2 > 0){
		return -1;
	}else if(l1 == 0 || l2 == 0){ //ritorno -2 se non esistono
	  return -2;
	}else{
    //controllo ogni lettera
		for(int i = 0; i < l1; i++){
			if(s1[i] != s2[i])//ritorno -1 se sono diverse
				return -1;
		}

	}
  //ritorno 0 se sono uguali
	return 0;
}

//funzione che controlla che il carattere sia presente all'interno del tastierino
bool inKeypad(char carattere) {
  //giro tutte le lettere del tastierino
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      //controllo se il carattere è presente, se lo è ritorno true
      if (tastierino[i][j] == carattere) {
        return true;
      }
    }
  }
  //in caso non è presente ritorno false
  return false;
}

//funzione per la lettura del carattere
void keypad(WiFiClient client){
  char taste[password_lenght];

  Serial.println("write the code after press the charapter '#'.");

  delay(1500);

  char c = ' ';
  int i = 0;
  //leggo i caratteri finchè non viene letto il carattere di terminazione
  while(c != '#'){
    //leggo il carattere
    c = read_keypad();
    //controllo se il carattere è nel tastierino e aggiungo il carattere alla password scritta
    if((c != '#') && (inKeypad(c))){
      taste[i] = c;
      i += 1;
    }

    delay(150);
  }

  //metto il terminatore alla fine della stringa
  taste[i] = '\0';

  int verify = 1;

  //controllo le stringhe per verificare che siano uguali
  if((verify = compara_stringhe(taste, PASSWORD)) == 0)
    riuscita();
  else
    fallita();

  delay(2000);
}