/*
 * Ethernet morse code device. (Internet of things weekend project.)
 * Author: Koen van Vliet <8by8mail@gmail.com>
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This example was based upon uIP hello-world by Adam Dunkels <adam@sics.se>
 * Ported to the Arduino IDE by Adam Nielsen <malvineous@shikadi.net>
 * Adaption to Enc28J60 by Norbert Truchsess <norbert.truchsess@t-online.de>
 */

#define _PORT 1000
#define _LEDPIN 7
#define _BUTTON 4
#define _BUZZER 3

#include <UIPEthernet.h>
// The connection_data struct needs to be defined in an external file.
#include <UIPServer.h>
#include <UIPClient.h>

EthernetServer server = EthernetServer(_PORT);

void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = {0x74,0x69,0x69,0x2d,0x30,0x31};
  IPAddress myIP(192,168,1,200);

  Ethernet.begin(mac,myIP);
  
  pinMode(_LEDPIN,OUTPUT);
  digitalWrite(_BUTTON, HIGH);

  server.begin();
}

void bleep(boolean onoff){
  if (onoff)
  {
    tone(_BUZZER,1000);
  } else {
    noTone(_BUZZER);
  }
  digitalWrite(_LEDPIN,onoff);
}

void loop()
{
  boolean state = LOW;
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    if (client.connected()){
      Serial.println("Connected\r");
      client.println("Welkom bij de arduino server!");
    }
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        switch(c){
          case 'a' : Serial.println("LED is AAN");
                     bleep(HIGH);
                     break;
          case 'b' : Serial.println("LED is UIT");
                     bleep(LOW);
                     break;
          default :  break;
        }
      }
      if (state == LOW){
        if(digitalRead(_BUTTON) == HIGH){
          Serial.println("KNOP IS UIT");
          client.println("KNOP IS UIT");
          state = HIGH;
        }
      }
      else
      {
        if(digitalRead(_BUTTON) == LOW){
          Serial.println("KNOP IS AAN");
          client.println("KNOP IS AAN");
          state = LOW;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    bleep(LOW);
  }
}
