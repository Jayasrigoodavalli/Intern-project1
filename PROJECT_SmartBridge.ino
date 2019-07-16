#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include<LiquidCrystal.h>
#include <ThingSpeak.h> 
WiFiClient client;
WiFiServer server(80);
const char* ssid     = "chandu"; 
const char* password = "chandu26"; 
float temp_celsius = 0;
float temp_fahrenheit = 0;
const int rs=13, en=12, d4=14, d5=27, d6=26, d7=25;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long myChannelNumber = 787435;  
const char * myWriteAPIKey = "N8M3DUOZFX1N2UQW";  


void setup() 
{
   lcd.begin(16, 2);
   Serial.begin(115200);
   pinMode(34, INPUT);
   pinMode(4,OUTPUT);   
   Serial.println();
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
    delay(500);
    Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi is connected");
   server.begin();
   Serial.println("Server started");
   Serial.println(WiFi.localIP());
   ThingSpeak.begin(client); 
}
void loop()
{
 lm35();
 motor();
 lcd16();
 static boolean data_state = false; 
 float temperature=temp_celsius; 
 ThingSpeak.writeField(myChannelNumber, 1, temperature, myWriteAPIKey);  
 delay(30000); // ThingSpeak will only accept updates every 15 seconds.  
 WiFiClient client = server.available();
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println("Connection: close");  // the connection will be closed after completion of the response
 client.println("Refresh: 10");  // update the page after 10 sec
 client.println();
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>Digital Temperature meter</strong></span></p>");
 client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>Temperature (*C)= ");
 client.println(temp_celsius);
 client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>Temperature (F) = ");
 client.println(temp_fahrenheit);
 client.print("</p>");
 client.println("</html>");
 delay(5000);
}
void lm35()
{
  float a = analogRead(34);
 temp_celsius =  (a/9.31) ;  // To convert analog values to Celsius We have 3.3 V on our board and we know that output voltage of LM35 varies by 10 mV to every degree Celsius rise/fall. So , (A0*3300/10)/1023 = celsius
 temp_fahrenheit = temp_celsius * 1.8 + 32.0;
 Serial.print("  Temperature = ");
 Serial.print(temp_celsius);
 Serial.print(" Celsius, ");
 Serial.print(temp_fahrenheit);
 Serial.println(" Fahrenheit");
}
void motor()
{
  lm35();
  if(temp_celsius>40)
  {
    digitalWrite(4,HIGH);
  }
  else
  {
    digitalWrite(4,LOW);
  }
}
void lcd16()
{
  lm35();
  lcd.clear();
  lcd.println(temp_celsius);
  lcd.print("celsius");
  lcd.setCursor(0,1);
  lcd.println(temp_fahrenheit);
  lcd.print("Fahrenheit");
}
