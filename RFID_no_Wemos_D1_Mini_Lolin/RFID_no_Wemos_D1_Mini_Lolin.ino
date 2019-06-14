/*
   ----------------------------------------------------------------------------
   This sketch uses the MFRC522 library ; see https://github.com/miguelbalboa/rfid
   for further details and other examples.
   Tem que acertar as bibliotecas
   NOTE: The library file MFRC522.h has a lot of useful info. Please read it.

   This sketch show a simple locking mechanism using the RC522 RFID module.
   ----------------------------------------------------------------------------
   Typical pin layout used:
   ---------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    WEMOS
               Reader/PCD   Uno           Mega      Nano v3    Mini D1 Lolin
   Signal      Pin          Pin           Pin       Pin        Pin
   ----------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9          D3
   SPI SS      SDA(SS)      10            53        D10         D15
   SPI MOSI    MOSI         11 / ICSP-4   51        D11         D13
   SPI MISO    MISO         12 / ICSP-1   50        D12         D12
   SPI SCK     SCK          13 / ICSP-3   52        D13         D14

*/


// Informações da Rede WiFi

char ssid[] = "123644987";
char wifiPassword[] = "planeta514";


#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         D3        // Configurable, see typical pin layout above
#define SS_PIN          D8        // Configurable, see typical pin layout above

int ledPin = D4; // GPIO2
int relePin = D1; // GPIO5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String read_rfid;
String ok_rfid[] = {"b60711b", "b71962a3", "14a7b17a", "71603e2e"};

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 2000;    // the debounce time; increase if the output flickers
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
}

/*
   Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}

void check1() {

  digitalWrite(D4, LOW);
  delay(2000);
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);


  if ((millis() - lastDebounceTime) < debounceDelay) {
    return;
  }


  lastDebounceTime = millis();


  Serial.println(read_rfid);

  for (int x = 0; x < 4; x++) {
    if (read_rfid == ok_rfid[x]) {
      digitalWrite(ledPin, LOW);
      digitalWrite(relePin, HIGH);
      delay(3000);
      digitalWrite(ledPin, HIGH);
      digitalWrite(relePin, LOW);
    }
  }
}
