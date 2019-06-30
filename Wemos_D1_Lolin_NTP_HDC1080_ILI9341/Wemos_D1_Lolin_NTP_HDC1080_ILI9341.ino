/********************************************************************************************
    Incluir as últimas versões das lib Adafruit_GFX, Adafruit_ILI9341 and XPT2046_Touchscreen Library:
    https://github.com/adafruit/Adafruit-GFX-Library
    https://github.com/adafruit/Adafruit_ILI9341
    https://github.com/PaulStoffregen/XPT2046_Touchscreen
    https://github.com/closedcube/ClosedCube_HDC1080_Arduino
    Rotinas de tempo utiliza a NTPclient.
    https://github.com/arduino-libraries/NTPClient
    Este prograna necessita de acesso a internet através de rede wifi
    todas as instruções estão na porta serial com velocidadade 115.200 bps
    Desenvolvido para Wemos D1 Mini Lolin conectado ao TFT-2.4 V 3.0.0
    Necessita do sensor de temperatura HDC1080
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
#include <WiFiUdp.h>
#include <NTPClient.h>

// Configurações do Servidor NTP
const char* servidorNTP = "pool.ntp.br"; // Servidor NTP para pesquisar a hora

const int fusoHorario = -10800; // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos

WiFiUDP ntpUDP; // Declaração do Protocolo UDP
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);

//Inicializa Sensor de temperatura e umidade HDC 1080
ClosedCube_HDC1080 hdc1080;

// Configuração para acesso à rede wifi coloca aqui ssid e senha
const char* ssid = "123644987";
const char* password = "planeta514";

// Definições para o TFT no Wemos D1 Min Lolin
#define TFT_CS D0  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_DC D8  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_RST -1 //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TS_CS D3   //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// declara variáveis globais para dias da semana e meses do ano
char daysOfTheWeek[7][14] = {"Domingo", "Segunda-Feira", "Terça-Feira", "Quarta-Feira", "Quinta-Feira", "Sexta-Feira", "Sabado"};
char mesesdoano [12][10] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio" , "Junho", "Julho" , "Agosto", "Setembro", "Outubro" , "Novembro" , "Dezembro"};

// Declara variáveis de armazenamento minimo e maximo
float tmaxima;
float tminima;
String datamin, timemin, datamax, timemax ;
String diadasemanamin, diadasemanamax;

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

String getFormattedDateCPP(long rawTime) {
  struct tm * dt;
  char buffer [30];
  dt = localtime(&rawTime);
  strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", dt);
  return (buffer);
}


// Busca hora que estão na Internet via NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;


void setup(void) {

  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("   ");
  Serial.println("ILI9341 Teste!");

  // Iniciar cliente de aquisição do tempo
  timeClient.begin();

  // inicializa TFT
  tft.begin();

#ifndef ESP8266
 while (!Serial);
#endif

  //Inicializa temperatura tmaxima e tminima
  tminima, tminima = hdc1080.readTemperature();

  //Inicializa o sensor de temperatura HDC1080 no endereço 0x40
  hdc1080.begin(0x40);

  // Inicializa conexão wifi
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // mostra na serial o diagnóstico do TFT
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


  // Caso necessário configura WEMOS D1 Mini com IP fixo
  //IPAddress subnet(255, 255, 255, 0);
 // WiFi.config(IPAddress(10, 0, 0, 25), IPAddress(10, 0, 0, 1), subnet);


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
  //  Variavel data formatada
  timeClient.update();
  unsigned long timeTratar = timeClient.getEpochTime ();
  String dataFormatada = getFormattedDateCPP(timeTratar);

  Serial.print("Data cpp: ");
  Serial.println(dataFormatada);

  //rotina para rotacionar o display
  // rotation = 0,1,2,3
  // tft.setRotation(rotation);
  tft.setRotation(1);
  testText(dataFormatada);
  delay(5000);


  // Hora
  Serial.print("Hora NTP: ");
  Serial.println(timeClient.getFormattedTime());
  // Data
  Serial.print("Data : ");
  Serial.println(dataFormatada);
  // dia da semana
  Serial.print("Dia: ");
  Serial.println(daysOfTheWeek[timeClient.getDay()]);
  // mes do ano
  Serial.print("Mês: ");
  Serial.println(mesesdoano [dataFormatada.substring(4, 5).toInt() - 1]);


  // Mostra os valores na página web
  server.handleClient();

  //Mostra os valores no Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(hdc1080.readTemperature());
  Serial.print(" oC, Umidade: ");
  Serial.print(hdc1080.readHumidity());
  Serial.print(" %");
  Serial.println();
  Serial.println();
  digitalWrite(BUILTIN_LED, 1);
  delay(2000);

  // Calcula a temp max e min
  if (hdc1080.readTemperature() > tmaxima) {
    tmaxima = hdc1080.readTemperature();
    timeClient.update();
    datamax        = dataFormatada;
    timemax        = timeClient.getFormattedTime ();
    diadasemanamax = daysOfTheWeek[timeClient.getDay ()];
  }
  if (hdc1080.readTemperature() < tminima) {
    tminima        = hdc1080.readTemperature();
    timeClient.update();
    datamin        = dataFormatada;
    timemin        = timeClient.getFormattedTime();
    diadasemanamin = daysOfTheWeek[timeClient.getDay ()];
  }
}

unsigned long testFillScreen()

{
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText(String dataFormatada)

{
  String tempmax     = String(tmaxima);
  String tempmin     = String(tminima);
  String temp        = String(hdc1080.readTemperature());
  String umid        = String(hdc1080.readHumidity());
  String tempstr     = String(hdc1080.readTemperature());
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.println();
  tft.println("Sitio Pe de Serra");
  tft.setTextSize(1);
  tft.println();
  tft.setTextColor(ILI9341_YELLOW);
  tft.println("           Temperatura Interna da Garagem");
  tft.println();
  tft.print("               ");
  tft.println(dataFormatada);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.println();
  tft.println();
  tft.println("      " + temp + " oC  " + umid + "%");
  tft.println();
  tft.println();
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED);
  tft.println("Temperatura Max:");
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(tempmax + " oC " + diadasemanamax + ", " + datamax + " [" + timemax +  "] ");
  tft.println();
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(2);
  tft.println("Temperatura Min:");
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(tempmin + " oC " + diadasemanamin + ", " + datamin + " [" + timemin +  "]");
  return micros() - start;
}


String homePage() {
  return (
           "<!DOCTYPE html>"
           "<html>"
           "<meta http-equiv='refresh' content='10'/>"
           "<meta charset=\"utf-8\" />"
           "<title>Sitio Pe de Serra - Temperatura da Granja</title>"
           "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
           "<span style='display: none;'>" + String(0) + "</span>"
           "<div style='text-align: center;'><img src =  'http://iot-day.com/images/projects/Sitiopedeserra.png' alt='' width='290' height=63' /></div>"
           "<div style='text-align: center;'>" + daysOfTheWeek[timeClient.getDay()]  + "  [" + timeClient.getFormattedTime() + "] </div>"
           "<h1>Temperatura: " + String(hdc1080.readTemperature()) + "<sup>o</sup>C</h1>"
           "<h1>Umidade: " + String(hdc1080.readHumidity()) + "%</h1>"
           "<table><tbody><tr><td><img src='http://iot-day.com/images/projects/Tmin.png' alt=' ' width='20' height='35' /></td><td><h4><span style='color: #87CEFA;'>Temperatura Mínima: " + String(tminima)  + " <sup>o</sup>C -  " + diadasemanamin + ", " + datamin + " [ " + timemin +  " ] </td></tr></tbody></table>"
           "<table><tbody><tr><td><img src='http://iot-day.com/images/projects/Tmax.png' alt=' ' width='20' height='35' /></td><td><h4><span style='color: #ff0000;'>Temperatura Máxima: " + String(tmaxima)  + " <sup>o</sup>C -  " + diadasemanamax + ", " + datamax + " [ " + timemax +  " ] </td></tr></tbody></table>"
           "<p>&nbsp;</p>"
           "<div style='text-align: center; '>&copy 2019 - Epaminondas Lage </div>"
         );
}
