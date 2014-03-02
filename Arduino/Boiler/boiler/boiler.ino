
#include <Adafruit_NeoPixel.h>
#include <Process.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define BUTTON_PIN 3
#define BUTTON_LED_PIN 4
#define REALY_PIN 5
#define GUI_PIN 6

Adafruit_NeoPixel ring = Adafruit_NeoPixel(24, GUI_PIN, NEO_GRB + NEO_KHZ800);
YunServer server;

uint32_t BLUE = ring.Color(0, 0, 255);
uint32_t RED = ring.Color(255, 0, 0);
uint32_t GREEN = ring.Color(0, 255, 0);

char GUI[25] = {0};
bool g_blink = false;

void setup() 
{
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  
  pinMode(BUTTON_PIN,INPUT);
  pinMode(BUTTON_LED_PIN,OUTPUT);
  pinMode(REALY_PIN,OUTPUT);
  digitalWrite(REALY_PIN,LOW);
  
//  Serial.begin(9600);
  
  ring.begin();
  ring.setBrightness(40);
  ring.show();
  
}

bool GetGUIConfigString(char* configString)
{ 
  Process p;
  p.begin("cat");
  p.addParameter("/boiler/gui_config.txt");
  p.run();
  
  char firstChar = p.peek();
  if( (firstChar != '-') && (firstChar != '*') )
     return false;
    
  int i = 0;
  while( (p.available()) && (i < ring.numPixels()) )
  {
    char c = p.read();
    configString[i] = c;
    ++i;
  }
  configString[ring.numPixels()] = 0;
  return true;
}


void loadingConfig()
{
  for(int j = 0;j<ring.numPixels();++j)
  {
    ring.setPixelColor(j,RED);
    ring.show();
    delay(100);
  }
  
  for(int k = 0;k<ring.numPixels();++k)
  {
    ring.setPixelColor(k,GREEN);
    ring.show();
    delay(100);
  }
}

void setGUI()
{  
  for(int i = 0;i<ring.numPixels();++i)
  {
    if(GUI[i] == '-')
      ring.setPixelColor(i,RED);
    else if(GUI[i] == '*')
      ring.setPixelColor(i,GREEN);
    else
      ring.setPixelColor(i,0,0,0);
  }
}

bool startBoiler()
{  
  digitalWrite(REALY_PIN,HIGH);
  digitalWrite(BUTTON_LED_PIN,HIGH);
}

bool stopBoiler()
{
  digitalWrite(REALY_PIN,LOW);
  digitalWrite(BUTTON_LED_PIN,LOW);
}

void process(YunClient client) 
{  
  String command = client.readStringUntil('/');
  command.trim();
   
  if (command == "StartBoiler")
    startBoiler();
    
  if (command == "StopBoiler") 
    stopBoiler();
    
  if (command == "CheckButton")
  {
    if(digitalRead(BUTTON_PIN))
      client.print("On");
    else
      client.print("Off");
  }
} 

int getTimePixel()
{
  Process p;
  p.begin("date");
  p.addParameter("+%H");
  p.run();
  
  char hourString[3] = {0};
  int i = 0;
  while( (p.available()) && (i < 2) )
  {
    char c = p.read();
    hourString[i] = c;
    ++i;
  }
    
  return atoi(hourString);
}

void blinkCurrentHour()
{  
  if(g_blink)
  {
    g_blink = false;
    ring.setPixelColor(getTimePixel(),BLUE);
  }
  else
  {
    g_blink = true; 
  }
}

void handleGUI()
{  
  char configString[ring.numPixels()+1];
  
  if(GetGUIConfigString(configString))
  { 
    if( strcmp(configString,GUI) != 0)
    {
      loadingConfig();
      strcpy(GUI,configString);
    }
  }
  
  setGUI();
  blinkCurrentHour();
  ring.show();
}

void handleWebRequests()
{
  YunClient client = server.accept();
  if (client) 
  {
    process(client);
    client.stop();
  }
}

void loop() 
{  
  handleWebRequests();
  handleGUI();
  delay(1000);
}
