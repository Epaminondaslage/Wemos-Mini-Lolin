/********************************************************************************************
    Setup latest Adafruit_GFX, Adafruit_ILI9341 and XPT2046_Touchscreen Library first:
    https://github.com/adafruit/Adafruit-GFX-Library
    https://github.com/adafruit/Adafruit_ILI9341
    https://github.com/PaulStoffregen/XPT2046_Touchscreen
**********************************************************************************************/
// atualizado em 25/06/21
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "time.h"


//Inicializa Sensor de temperatura e umidade HDC 1080
ClosedCube_HDC1080 hdc1080;

// Configuração para acesso à rede wifi
const char* ssid = "Popo";
const char* password = "Planetfone";

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
int diamin, mesmin, anomin, horamin, minutomin, segundomax, diamax, mesmax, anomax, horamax, minutomax, segundomin;
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

// Busca time estam na Internet via NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

/******************************
 void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
*********************************/


void setup(void) {
  
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("   ");
  Serial.println("ILI9341 Teste!");

  //Inicializa e obtem a hora
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //*****************************
  //  printLocalTime();
  //*****************************
  
  // inicializa TFT
  tft.begin();

#ifndef ESP8266
  while (!Serial);
#endif

  delay(3000); // Aguarda abertura da console


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


  //Configura WEMOS com IP fixo
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.config(IPAddress(10, 0, 0, 23), IPAddress(10, 0, 0, 1), subnet);
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
  //rotina para rotacionar o display
  // rotation = 0,1,2,3
  // tft.setRotation(rotation);

  tft.setRotation(1);
  testText();
  delay(5000);

//******************
//  printLocalTime();
//******************


  // Mostra os valores na página web
  server.handleClient();

  /*Mostra dados do RTC na porta serial
    DateTime now = rtc.now();
    Serial.print("Data :");
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(mesesdoano[now.month() - 1]);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") Hora: ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    digitalWrite(BUILTIN_LED, 1);
    delay(2000);
  */

  //Mostra os valores no Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(hdc1080.readTemperature());
  Serial.print(" oC, Umidade: ");
  Serial.print(hdc1080.readHumidity());
  Serial.print(" %");
  Serial.println();
  Serial.println();
  digitalWrite(BUILTIN_LED, 0);
  delay(2000);

  // Calcula a temp max e min
  if (hdc1080.readTemperature() > tmaxima) {
    tmaxima = hdc1080.readTemperature();
    /*  DateTime now   = rtc.now();
      diamax         = now.day(), DEC;
      mesmax         = now.month(), DEC;
      anomax         = now.year(), DEC;
      horamax        = now.hour(), DEC;
      minutomax      = now.minute(), DEC;
      segundomax     = now.second(), DEC;
      diadasemanamax = daysOfTheWeek[now.dayOfTheWeek()];*/
  }
  if (hdc1080.readTemperature() < tminima) {
    tminima        = hdc1080.readTemperature();
    /*  DateTime now   = rtc.now();
      diamin         = now.day(), DEC;
      mesmin         = now.month(), DEC;
      anomin         = now.year(), DEC;
      horamin        = now.hour(), DEC;
      minutomin      = now.minute(), DEC;
      segundomin     = now.second(), DEC;
      diadasemanamin = daysOfTheWeek[now.dayOfTheWeek()]; */
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
  String temp        = String(hdc1080.readTemperature());
  String umid        = String(hdc1080.readHumidity());
  /*String dmin        = diamin < 10 ? "0" + String(diamin) : String(diamin);
    String mmin        = mesmin < 10 ? "0" + String(mesmin) : String(mesmin);
    String amin        = String(anomin);
    String hmin        = horamin < 10 ? "0" + String(horamin) : String(horamin);
    String mimin       = minutomin < 10 ? "0" + String(minutomin) : String(minutomin);
    String smin        = segundomin < 10 ? "0" + String(segundomin) : String(segundomin);
    String dmax        = diamax < 10 ? "0" + String(diamax) : String(diamax);
    String mmax        = mesmax < 10 ? "0" + String(mesmax) : String(mesmax);
    String amax        = String(anomax);
    String hmax        = horamax < 10 ? "0" + String(horamax) : String(horamax);
    String mimax       = minutomax < 10 ? "0" + String(minutomax) : String(minutomax);
    String smax        = segundomax < 10 ? "0" + String(segundomax) : String(segundomax);
    DateTime now       = rtc.now();
    String diadasemana = String(daysOfTheWeek[now.dayOfTheWeek()]);
    //String dia         = (dia.toInt() < 10 ? "0" + dia : dia);
    String dia         = String(now.day(), DEC);
    String mes         = String(now.month(), DEC);
    String ano         = String(now.year(), DEC);
    String hora        = String(now.hour(), DEC);
    String minuto      = String(now.minute(), DEC);
    String segundo     = String(now.second(), DEC);*/
  String tempstr = String(hdc1080.readTemperature());
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
  tft.println("           Temperatura Interna do Galpao");
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
  //tft.println(tempmax + " oC " + diadasemanamax + ", " + dmax + "/ " + mmax + "/ " + amax + " [" + hmax + ":" + mimax + ":" + smax +  "] ");
  tft.println();
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(2);
  tft.println("Temperatura Min:");
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  //tft.println(tempmin + " oC " + diadasemanamin + ", " + dmin + "/ " + mmin + "/ " + amin + " [" + hmin + ":" + mimin + ":" + smin +  "]");
  return micros() - start;
}
//********************************
String homePage() {
  String tempmax     = String(tmaxima);
  String tempmin     = String(tminima);
  String temp        = String(hdc1080.readTemperature());
  String umid        = String(hdc1080.readHumidity());
  String dmin        = diamin < 10 ? "0" + String(diamin) : String(diamin);
  String mmin        = mesmin < 10 ? "0" + String(mesmin) : String(mesmin);
  String amin        = String(anomin);
  String hmin        = horamin < 10 ? "0" + String(horamin) : String(horamin);
  String mimin       = minutomin < 10 ? "0" + String(minutomin) : String(minutomin);
  String smin        = segundomin < 10 ? "0" + String(segundomin) : String(segundomin);
  String dmax        = diamax < 10 ? "0" + String(diamax) : String(diamax);
  String mmax        = mesmax < 10 ? "0" + String(mesmax) : String(mesmax);
  String amax        = String(anomax);
  String hmax        = horamax < 10 ? "0" + String(horamax) : String(horamax);
  String mimax       = minutomax < 10 ? "0" + String(minutomax) : String(minutomax);
  String smax        = segundomax < 10 ? "0" + String(segundomax) : String(segundomax);
  String diadasemana = "dia indefinido";
  String dia         = "xx";
  String mes         = "yy";
  String ano         = "zz";
  String hora        = "99";
  String minuto      = "88";
  String segundo     = "77";
  String mesesdoano  = "mes indef.";
  return (
           "<!DOCTYPE html>"
           "<html>"
           "<meta http-equiv='refresh' content='10'/>"
           "<meta charset=\"utf-8\" />"
           "<title>Sitio Pe de Serra - Temperatura da Granja</title>"
           "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
           "<span style='display: none;'>" + String(0) + "</span>"
           "<div style='text-align: center;'><img src =  'http://caixagg.planetfone.com.br/img/Sitiopedeserra.png' alt='' width='290' height=63' /></div>"
           "<div style='text-align: center;'>" + diadasemana + ", " + dia + " de " + mesesdoano + " de " + ano + "</div>"
           "<h1>Temperatura: " + temp + "<sup>o</sup>C</h1>"
           "<h1>Umidade: " + umid + "%</h1>"
           "<table><tbody><tr><td><img src='http://caixagg.planetfone.com.br/img/Tmin.png' alt=' ' width='20' height='35' /></td><td><h4><span style='color: #87CEFA;'>Temperatura Mínima: " + tempmin + " <sup>o</sup>C</span></h4>" + diadasemanamin + ", " + dmin + "/ " + mmin + "/ " + amin + " [" + hmin + ":" + mimin + ":" + smin +  "] </td></tr></tbody></table>"
           "<table><tbody><tr><td><img src='http://caixagg.planetfone.com.br/img/Tmax.png' alt=' ' width='20' height='35' /></td><td><h4><span style='color: #ff0000;'>Temperatura Máxima: " + tempmax + " <sup>o</sup>C</span></h4>" + diadasemanamax + ", " + dmax + "/ " + mmax + "/ " + amax + " [" + hmax + ":" + mimax + ":" + smax +  "] </td></tr></tbody></table>"
           "<p>&nbsp;</p>"
           "<p>&nbsp;</p>"
           "<div style='text-align: center;'>&copy 2019 - Epaminondas Lage </div>"
         );


  //*********************************
  /*unsigned long testLines(uint16_t color)
    {
    unsigned long start, t;
    int x1, y1, x2, y2,
        w = tft.width(),
        h = tft.height();

    tft.fillScreen(ILI9341_BLACK);
    yield();

    x1 = y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    t = micros() - start; // fillScreen doesn't count against timing

    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();

    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
      tft.drawLine(x1, y1, x2, y2, color);

    yield();
    return micros() - start;
    }

    unsigned long testFastLines(uint16_t color1, uint16_t color2)
    {
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (y = 0; y < h; y += 5)
      tft.drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5)
      tft.drawFastVLine(x, 0, h, color2);

    return micros() - start;
    }

    unsigned long testRects(uint16_t color)
    {
    unsigned long start;
    int n, i, i2,
        cx = tft.width() / 2,
        cy = tft.height() / 2;

    tft.fillScreen(ILI9341_BLACK);
    n = min(tft.width(), tft.height());
    start = micros();
    for (i = 2; i < n; i += 6)
    {
      i2 = i / 2;
      tft.drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
    }

    unsigned long testFilledRects(uint16_t color1, uint16_t color2)
    {
    unsigned long start, t = 0;
    int n, i, i2,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    n = min(tft.width(), tft.height());
    for (i = n; i > 0; i -= 6)
    {
      i2 = i / 2;
      start = micros();
      tft.fillRect(cx - i2, cy - i2, i, i, color1);
      t += micros() - start;
      // Outlines are not included in timing results
      tft.drawRect(cx - i2, cy - i2, i, i, color2);
      yield();
    }

    return t;
    }

    unsigned long testFilledCircles(uint8_t radius, uint16_t color)
    {
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (x = radius; x < w; x += r2)
    {
      for (y = radius; y < h; y += r2)
      {
        tft.fillCircle(x, y, radius, color);
      }
    }

    return micros() - start;
    }

    unsigned long testCircles(uint8_t radius, uint16_t color)
    {
    unsigned long start;
    int x, y, r2 = radius * 2,
              w = tft.width() + radius,
              h = tft.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for (x = 0; x < w; x += r2)
    {
      for (y = 0; y < h; y += r2)
      {
        tft.drawCircle(x, y, radius, color);
      }
    }

    return micros() - start;
    }

    unsigned long testTriangles()
    {
    unsigned long start;
    int n, i, cx = tft.width() / 2 - 1,
              cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    n = min(cx, cy);
    start = micros();
    for (i = 0; i < n; i += 5)
    {
      tft.drawTriangle(
        cx, cy - i,     // peak
        cx - i, cy + i, // bottom left
        cx + i, cy + i, // bottom right
        tft.color565(i, i, i));
    }

    return micros() - start;
    }

    unsigned long testFilledTriangles()
    {
    unsigned long start, t = 0;
    int i, cx = tft.width() / 2 - 1,
           cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (i = min(cx, cy); i > 10; i -= 5)
    {
      start = micros();
      tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                       tft.color565(0, i * 10, i * 10));
      t += micros() - start;
      tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                       tft.color565(i * 10, i * 10, 0));
      yield();
    }

    return t;
    }

    unsigned long testRoundRects()
    {
    unsigned long start;
    int w, i, i2,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    w = min(tft.width(), tft.height());
    start = micros();
    for (i = 0; i < w; i += 6)
    {
      i2 = i / 2;
      tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(i, 0, 0));
    }

    return micros() - start;
    }

    unsigned long testFilledRoundRects()
    {
    unsigned long start;
    int i, i2,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
    {
      i2 = i / 2;
      tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, i, 0));
      yield();
    }

    return micros() - start;
  */

}
