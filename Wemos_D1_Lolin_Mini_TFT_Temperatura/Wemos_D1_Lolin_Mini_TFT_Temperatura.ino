/********************************************************************************************
    Sensor de Temperatura e umidade do ar
    Sitio pé de Serra
    Processador Wemos D1 mini lolin
    Setup latest Adafruit_GFX, Adafruit_ILI9341 and XPT2046_Touchscreen Library first:
    https://github.com/adafruit/Adafruit-GFX-Library
    https://github.com/adafruit/Adafruit_ILI9341
    https://github.com/PaulStoffregen/XPT2046_Touchscreen
    atualizado em 25/06/21
**********************************************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


//Inicializa Sensor de temperatura e umidade HDC 1080
ClosedCube_HDC1080 hdc1080;

// Configuração para acesso à rede wifi
// Colocar ssi e senha da rede 

//const char* ssid = "pedeserra";
//const char* password = "planetfone";
const char* ssid = "PLT-DIR";
const char* password = "epaminondas";

// configura TFT para i2c

#define TFT_CS D0  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_DC D8  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_RST -1 //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TS_CS D3   //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


// Declara variáveis de armazenamento minimo e maximo  e fator de correção
float tmaxima;
float tminima;
float ajustetemp = 1.25;
float ajusteumidade = 6.1;


int a = 0;

// inicializa servidor web
ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(BUILTIN_LED, 1);
  server.send(200, "text/html", homePage());
  digitalWrite(BUILTIN_LED, 0);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("   ");
  Serial.println("Velocidade 115200-  Teste!");
  
  // inicializa TFT
  tft.begin();

#ifndef ESP8266
  while (!Serial);
#endif

  delay(3000); // Aguarda abertura da console


  //Inicializa temperatura tmaxima e tminima
  tminima, tminima = (hdc1080.readTemperature()-ajustetemp);

  //Inicializa o sensor de temperatura HDC1080 no endereço 0x40
  hdc1080.begin(0x40);

  // Inicializa conexão wifi
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //Diagnostico do TFT
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Formato do Pixel: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Formato da Imagem: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Auto diagnóstico (Hexa): 0x");
  Serial.println(x, HEX);
  Serial.print("Auto diagnóstico (Decimal): 0x");
  Serial.println(x, DEC);
  delay(500);
  Serial.println(F("Feito!"));


  //Configura WEMOS LOLIN com IP fixo
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(IPAddress(10, 0, 2, 25), IPAddress(10,0,2, 1), subnet);
  //Serial.println("");

  // Espera por conexão wifi pisca led na placa
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, 1);
    delay(500);
    Serial.print(".");
    digitalWrite(BUILTIN_LED, 0);
  }

  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS Inicializado");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "Esta funcionando bem");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor HTTP inicializado");
  Serial.println("");
}

void loop(void)
{
  
  tft.setRotation(1);
  testText();
  delay(5000);

  // Mostra os valores na página web
  server.handleClient();
  //Mostra os valores no Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(hdc1080.readTemperature()-ajustetemp);
  Serial.print(" oC, Umidade: ");
  Serial.print(hdc1080.readHumidity()-ajusteumidade);
  Serial.print(" %");
  Serial.println();
  Serial.println();
  digitalWrite(BUILTIN_LED, 0);
  delay(2000);

  // Calcula a temp max e min
  if ((hdc1080.readTemperature()-ajustetemp) > tmaxima) {
    tmaxima = (hdc1080.readTemperature()-ajustetemp);
  }
  if ((hdc1080.readTemperature()-ajustetemp) < tminima) {
    tminima = (hdc1080.readTemperature()-ajustetemp);
  }


}

unsigned long testFillScreen()

{
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText()

{
  String tempmax     = String(tmaxima);
  String tempmin     = String(tminima);
  String temp        = String(hdc1080.readTemperature()-ajustetemp);
  String umid        = String(hdc1080.readHumidity()-ajusteumidade);
  String tempstr = String(hdc1080.readTemperature()-ajustetemp);
  
  // Configuração do TFT
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.println("Sitio Pe de Serra");
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(100,35);
  tft.println("Igarape-MG");
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10,100);
  tft.println("Temperatura:" );
  tft.setTextSize(3);
  tft.setCursor(155,100);
  tft.println(temp);
  tft.setTextSize(2);
  tft.setCursor(250,95);
  tft.println("o");
  tft.setTextSize(3);
  tft.setCursor(265,100);
  tft.println("C");
  tft.setTextSize(2);
  tft.setCursor(10,140);
  tft.println("Umidade....:" );
  tft.setTextSize(3);
  tft.setCursor(155,140);
  tft.println( umid + " %");
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(10,220);
  tft.println("Max:"+tempmax + " C");
  tft.setTextSize(1);
  tft.setCursor(120,220);
  tft.println("o");
  tft.setTextSize(2);
  tft.setCursor(160,220);
  tft.println("Min:"+tempmin + " C");
  tft.setTextSize(1);
  tft.setCursor(270,220);
  tft.println("o");
  return micros() - start;
}
//***********************************************************************
String homePage() {
  String tempmax     = String(tmaxima);
  String tempmin     = String(tminima);
  String temp        = String(hdc1080.readTemperature()-ajustetemp);
  String umid        = String(hdc1080.readHumidity()-ajusteumidade);
  return (
           "<!DOCTYPE html>"
           "<html>"
           "<meta http-equiv='refresh' content='10'/>"
           "<meta charset=\"utf-8\" />"
           "<title>Sitio Pe de Serra - Temperatura do Sítio Pé de Serra</title>"
           "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
           "<span style='display: none;'>" + String(0) + "</span>"
           "<div style='text-align: center;'><img src =  'http://caixagg.planetfone.com.br/img/Sitiopedeserra.png' alt='' width='290' height=63' /></div>"
           "<h1>Temperatura: " + temp + "<sup>o</sup>C</h1>"
           "<h1>Umidade: " + umid + "%</h1>"
           "<table><tbody><tr><td><img src='http://caixagg.planetfone.com.br/img/Tmin.png' alt=' ' width='20' height='35' /></td><td><h4><span style='color: #87CEFA;'>Temperatura Mínima: " + tempmin + " <sup>o</sup>C</span></h4>" " </td></tr></tbody></table>"
           "<table><tbody><tr><td><img src='http://caixagg.planetfone.com.br/img/Tmax.png' alt=' ' width='20' height='35' /></td><td><h4><span style='color: #ff0000;'>Temperatura Máxima: " + tempmax + " <sup>o</sup>C</span></h4>" "</td></tr></tbody></table>"
           "<p>&nbsp;</p>"
           "<p>&nbsp;</p>"
           "<div style='text-align: center;'>&copy 2021 - Epaminondas Lage </div>"
         );
  //***********************************************************************       
}
