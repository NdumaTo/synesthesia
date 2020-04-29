#include <FastLED.h> //Lights Library

//Wifi Libraries
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
//#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
//#include <WiFiServer.h>
//#include <WiFiUdp.h>

//Variables for the LEDs
#define DATA_PIN D1
#define numLEDs 300

CRGB leds[numLEDs];
#define red CRGB(255,0,0)
#define green CRGB(0,255,0)
#define blue CRGB(0,0,255)
#define white CRGB(255,255,255)
#define black CRGB(0,0,0)

uint8_t currentR =0;
uint8_t currentG=0;
uint8_t currentB=0;

bool redMax = false;
bool blueMax = false;
bool greenMax = false;

CRGB colour = white;

//variables for the WIFI
ESP8266WebServer server(80);
WiFiManager wifiManager;

void setup() 
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, numLEDs);
  fill_solid(leds,numLEDs,black);
  FastLED.show();
  WiFiSetup();
}

void loop()
{ 
  server.handleClient();
  fade();
  FastLED.show();
}

void WiFiSetup()
{
  WiFi.hostname("Ntunu_Lights"); //Connect to saved WiFi using the name Ntunu_Light
  wifiManager.setAPCallback(callbackDisplay); 
  wifiManager.autoConnect("Lights"); //If this fails create a new WiFi network

  connectedDisplay();
   
  MDNS.begin("ntunu"); //Allows connection to the lights without typing in an address
  server.begin(); //Start the server
  server.on("/", handleRoot);
  server.on("/red",changeRed);
  server.on("/green",changeGreen);
  server.on("/blue", changeBlue);
  server.on("/white", changeWhite);
}

void callbackDisplay(WiFiManager *myWiFiManager) //For when connecting fails and you have to configure manually
{
  for(int j =0; j<3; j++)
  {
    fill_solid(leds, numLEDs, CRGB(255,0,0));
    FastLED.show();
    for(int i=1; i<100; i++)
    {
    fadeToBlackBy( leds, numLEDs, 10);
    FastLED.show();
    delay(10);
    }
  }
  fill_solid(leds, numLEDs, CRGB(0,0,0));
  FastLED.show();
}

void runner()
{
  //a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, numLEDs, 20);
  int pos = beatsin16(13,0,numLEDs);
  leds[pos] += CRGB(colour);
}

void connectedDisplay()
{
  fill_solid(leds, numLEDs, green);
    FastLED.show();
    for(int i=1; i<100; i++)
    {
    fadeToBlackBy( leds, numLEDs, 10);
    FastLED.show();
    delay(10);
    }
}

//Handles requests from clients
void handleRoot()
{
  server.send(200,"text/html", webpage());
}

//Colour changing functions
void changeRed()
{
  colour = red;
  server.send(200,"text/html", webpage());
}

void changeGreen()
{
  colour = green;
  server.send(200,"text/html", webpage());
}

void changeBlue()
{
  colour = blue;
  server.send(200,"text/html", webpage());
}

void changeWhite()
{
  colour = white;
  server.send(200,"text/html", webpage());
}
//The webpage a client will be shown
String webpage()
{
String webpage=
"<html> <head>"
"<title>"
"Lights Control"
"</title> </head>"
"<body>"
"<p>"
"Red: "
"<a href=/red><button> On </button> </a>"

"</p> <p>"
"Green: "
"<a href=/green> <button> On </button> </a>"

"</p>"
"<p>"
"Blue: "
"<a href=/blue> <button> On </button> </a>"

"</p> <p>"
"White: "
"<a href=/white> <button> On </button> </a>"
"</p>"
"</body>  </html>";
return webpage;
}

void epilepticFit()
{
  int r = random8();
  int b = random8();
  int g = random8();

  fill_solid(leds,numLEDs,black);
  FastLED.show();
  delay(10);
  
  fill_solid(leds,numLEDs,CRGB(r,0,b));
  FastLED.show();
  delay(10);
  
  fill_solid(leds,numLEDs,black);
  FastLED.show();
  delay(10);

  fill_solid(leds,numLEDs,red);
  FastLED.show();
  delay(10);

  fill_solid(leds,numLEDs,black);
  FastLED.show();
  delay(10);

  fill_solid(leds,numLEDs,blue);
  FastLED.show();
  delay(10);
}

void fade()
{
  EVERY_N_MILLISECONDS(260)
  {
    if(blueMax==false)
    {
      for(int i =0; i<numLEDs; i++)
      {
        leds[i]=CRGB(255-currentB,50,currentB);
      }

      if(currentB<255)
      {
        currentB++;
      }

      else
      {
        blueMax=true;
      }
    }

    else
    {
      for(int i =0; i<numLEDs; i++)
      {
        leds[i]=CRGB(255-currentB,50,currentB);
      }

      if(currentB>0)
      {
        currentB--;
      }

      else
      {
        blueMax=false;
      }
    }
  }
}

