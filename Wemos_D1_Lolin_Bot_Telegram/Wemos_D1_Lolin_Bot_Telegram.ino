/**************************************************************************
                   Placa de desenvolvimento Wemos D1 Mini Lolin
                   Acionamento de um módulo relé com bot Telegram
                   Necessita de shild relé para Wemos d1 mini
                   bot_telegram Popo_esp286_bot
                   Bibliotecas: 
                   CTBot.h
                   ArduinoJson.h  Versão  5.13.5 
 **************************************************************************/
//Incluir as libraries  

#include "CTBot.h"
CTBot myBot;

//Definicoes da rede wifi e token de conexao bot Telegram
String ssid  = "SEU SSID"; //Nome da sua rede wifi
String pass  = "SUA SENHA"; //Senha da sua rede wifi
String token = "SEU TOKEN"; //Token bot Telegram

//Pinos dos leds
uint8_t led1 = D4;// GPIO2 Led build in para monitoramento
uint8_t led2 = D1;// GPIO5 acesso ao módulo relé

void setup()
{
  Serial.begin(115200);
  Serial.println("  ");
  Serial.println("Inicializando bot Telegram...");

  //Conexao na rede wifi
  myBot.wifiConnect(ssid, pass);

  //Define o token
  myBot.setTelegramToken(token);

  //Verifica a conexao
  if (myBot.testConnection())
    Serial.println("\nConexao Ok!");
  else
    Serial.println("\nFalha na conexao!");

  //Define os pinos dos leds como saida e apaga os leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
}

void loop()
{
  //Variavel que armazena a mensagem recebida
  TBMessage msg;

  //Verifica se chegou alguma mensagem
  if (myBot.getNewMessage(msg))
  {
    //Verifica se foi recebia a mensagem "Liga LED"
    if (msg.text.equalsIgnoreCase("LIGA LED"))
    {
      //Liga o led 1 
      digitalWrite(led1, LOW);
      //Retorna uma pensagem informando que o led foi aceso
      myBot.sendMessage(msg.sender.id, "LED Aceso!");
    }
    else if (msg.text.equalsIgnoreCase("DESLIGA LED"))
    {
      digitalWrite(led1, HIGH);
      myBot.sendMessage(msg.sender.id, "LED Apagado!");
    }
    else if (msg.text.equalsIgnoreCase("LIGA RELE"))
    {
      digitalWrite(led2, HIGH);
      myBot.sendMessage(msg.sender.id, "Rele  Ligado!");
    }
    else if (msg.text.equalsIgnoreCase("DESLIGA RELE"))
    {
      digitalWrite(led2, LOW);
      myBot.sendMessage(msg.sender.id, "Rele Desligado!");
    }
    else
    {
      //Caso receba qualquer outro comando, envia uma
      //mensagem generica/informativa
      String reply;
      reply = (String)"Ola " + msg.sender.username + (String)"! Sitio Pe de Serra ! Contole de Relés";
      myBot.sendMessage(msg.sender.id, reply);
    }
  }
  delay(500);
}

