/*
   ----------------------------------------------------------------------------
   Placa utilizada Wemos D1 Mini Lolin
   Este Skech utiliza o shild MFRC522 library
   https://github.com/miguelbalboa/rfid

   ----------------------------------------------------------------------------
                   Correspondencia de pinos entre módulo e placas
   -----------------------------------------------------------------
               MFRC522      Arduino        Arduino    WEMOS
               Reader/PCD   Uno            Nano v3    Mini D1 Lolin
   Signal      Pin          Pin            Pin        Pin
   -----------------------------------------------------------------
   RST/Reset   RST          9              D9          D3
   SPI SS      SDA(SS)      10             D10         D15
   SPI MOSI    MOSI         11 / ICSP-4    D11         D13
   SPI MISO    MISO         12 / ICSP-1    D12         D12
   SPI SCK     SCK          13 / ICSP-3    D13         D14

*/

#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         D3        // Configurable, see typical pin layout above
#define SS_PIN          D8        // Configurable, see typical pin layout above

int ledPin = D4; // GPIO2
int relePin = D1; // GPIO5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Lista de keys/Cartões autorizados
String read_rfid;
String ok_rfid[] = {"d37f9479", "b71962a3", "14a7b17a", "71603e2e"};

unsigned long lastDebounceTime = 0;     // the last time the output pin was toggled
unsigned long debounceDelay = 2000;    // debounce time; increase if the output flickers
String lastId;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);
  Serial.begin(9600);         // Initialize serial communications with the PC
  while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();         // Init MFRC522 card
  Serial.println(" ");
  Serial.println("--------------------");
  Serial.println("Wemos Lolin Di mini ");
  Serial.println("Shild MFRC522");
  Serial.println("--------------------");
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}

void loop() {
  // Verifica novos tags/cartões
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Seleciona um tag
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

//Evita as multiplas leitura do mesmo TAG
  if ((millis() - lastDebounceTime) < debounceDelay) {
    return;
  }


  lastDebounceTime = millis();

  Serial.print("Tag: ");
  Serial.print(read_rfid);
  // Verifica se o tag/cartão está cadastrado
  // Deve-se colocar um teste para cada tag cadastrado
    if ((read_rfid == ok_rfid[0]) || (read_rfid == ok_rfid[1]) || (read_rfid == ok_rfid[2]) || (read_rfid == ok_rfid[3])){
      Serial.println( " Cadastrado");
      digitalWrite(ledPin, LOW);
      digitalWrite(relePin, HIGH);
      delay(3000); // tempo de ativação do relé
      digitalWrite(ledPin, HIGH);
      digitalWrite(relePin, LOW);
    }
    else {
      Serial.println( " Não Cadastrado");
    }
  
}
