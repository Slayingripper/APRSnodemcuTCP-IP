#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>


#include "Wire.h"
#include "LiquidCrystal.h"
const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);



#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdlib_noniso.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

//const int LED = D0;
ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial
#define SLEEP_DELAY 3000

#include <ArduinoJson.h>
 
//#define ONE_WIRE_BUS 2  // DS18B20 pin 2 по nodemcu 0.9 D4
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature DS18B20(&oneWire);
//////////////////////////////////////////
// set location and API key for weather
String CityID = "146268";
String APIKEY = "3e4c703c58358f810723b382adf2195c";
//Set Callsign and APIkey APRS/////
String Callsign = "5B4ANU-13";
String APIaprs = "117511.y5T2lut5UFcsj0PY";
char aprsserver[] = "api.aprs.fi";

String Comment ;


///////////////////////////////////
WiFiClient client;
char servername[]="api.openweathermap.org";              // remote server we will connect to
String result;
String weatherDescription ="";
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;
float WindDirection;
float Windspeed;
String Description;
//////////////////////////////////////////

/*
#define I2C_SCL 12      // Barometric Pressure Sensor (BMP085)
#define I2C_SDA 13      // Barometric Pressure Sensor (BMP085)
Adafruit_BMP085 bmp;
 
#define DHTPIN 4 
#define DHTTYPE DHT22  
//#define DHTTYPE DHT11
 


DHT dht(DHTPIN, DHTTYPE);

float dst,bt,bp,ba;
bool bmp085_present=true;
 
char * skipControlChars(char * sLine) {
    char * startFrom = sLine;
  
    while(true) {
        if(nullptr == sLine) {
            break;
        }
    
        if('#' == (*sLine)) {
            if((sLine - startFrom) > 0) {
                --sLine;
            } else {
                sLine = nullptr;
            }
            
            break;
        } else if('\0' == (*sLine)) {
            sLine = nullptr;
        
            break;
        }
        
        ++sLine;
    }
    
    return sLine;
}
 */
void setup() {
  lcd.begin(16, 2);
  lcd.print("Starting up");
  delay(5000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("5B4ANU");
  lcd.setCursor(2, 1);
  lcd.print("APRS WXSMS");
//  pinMode(LED, OUTPUT);
  USE_SERIAL.begin(115200); // SET BAUDRATE
  USE_SERIAL.flush();    
  delay(1000);
  WiFiMulti.addAP("Airbus Home Private", "costas46"); // SSID PASSWORD
 // digitalWrite(LED, HIGH);
 // Wire.begin(I2C_SDA, I2C_SCL);
  delay(10);
  //digitalWrite(LED, LOW);
  /*
  if (!bmp.begin()) {
    USE_SERIAL.println("No BMP085 sensor detected!");
    bmp085_present=false;
  }
 
    dht.begin(); 
}
 */
}
void closeConnection(HTTPClient * pClientToClose) {
  pClientToClose -> end();
  delay(SLEEP_DELAY);
}


 
void loop() {
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    
 
        const uint16_t port = 14580; // APRS PORT
        const char * host = "asia.aprs2.net"; // APRS SERVER
        WiFiClient client; //
        delay(5000); //  5 second delay 
       
        getWeatherData();
        getAPRSdata();
        if (!client.connect(host, port)) {
          
            
           return;
        }
 
        client.println("user 5B4ANU-13 pass 15540 vers ESP8266_SM 0.1 filter m/1");  // Insert Callsign and APRS PASSWORD
        delay (250);
        //THIS SECTION IS IF YOU WANT TO USE SENSORS INSTEAD OF THE WEB API////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////
        /*
        DS18B20.requestTemperatures();
        dst = DS18B20.getTempCByIndex(0);
        USE_SERIAL.print("Temperature: ");
        USE_SERIAL.println(dst);
 
        dst=(dst*1.8)+32;
 
        if(bmp085_present) {
          bt = bmp.readTemperature();//(bmp.readTemperature() * 9/5) + 32;
          USE_SERIAL.print("Temperature bmp: ");
          USE_SERIAL.println(bt);
        }
  
        
        if(bmp085_present) {
          bp = bmp.readPressure()/10;//133.3224;// / 3386;
          USE_SERIAL.print("Pressure: ");
          USE_SERIAL.println(bp);
        }
   
        if(bmp085_present) {
          USE_SERIAL.print("Real altitude = ");
          USE_SERIAL.print(bmp.readAltitude(101500));
          USE_SERIAL.println(" meters");
          ba=bmp.readAltitude(101500);
        }
 
         float h22 = dht.readHumidity(); 
         float t22 = dht.readTemperature();
 
        if (isnan(t22) || isnan(h22)) 
        { 
          Serial.println("Failed to read from DHT"); 
        } 
        else
        {
          USE_SERIAL.print("Temperature dht: ");
          USE_SERIAL.println(t22);
          USE_SERIAL.print("Humidity dht: ");
          USE_SERIAL.println(h22);
        }
     
        const int & f = dst; // Temprature
        const int & h = h22; // Humidity
        const int & p = bp; // Pressure
         
        USE_SERIAL.println(f); // 
        USE_SERIAL.println(h);
        USE_SERIAL.println(p);
*/
        ///////////////////////////////////////////////////////////////////////
        //temprature converter to F
        int t = (Temperature);
        String cellcy = String(t);
        t = (t*2)+ 32;
        String temp;
        temp = String(t);

        // Humidity
        int hu = (Humidity);
        String hum ;
        hum=String(hu);

        //Pressure
        int pr = (Pressure);
        String prs ;
        prs = String(pr);

        // Winddirection
        int wnd = (WindDirection);
        String windD; 
        windD = String(wnd);
        //Windspeed
        unsigned int wnds = 0;
        wnds = (Windspeed);
        //String windS; 
        //windS = String(wnds);
        String windS = String( printf("%02d", wnds));
        //Description
        Serial.println(Description);
        //checker
        Serial.println(windS);
        Serial.println(windD);
        Serial.println(prs);
        Serial.println(hum);
        Serial.println(temp);
        Serial.println(Comment);
        //Raw packet that gets sent
        //THE ZEROS ARE WHERE THE DATA GOES
        //winderi
        client.print("5B4ANU-13>APDR15,WIDE1-1:=3506.1 N/03321.5 E_"+windD+"/00"+windS+"g000t"+temp+"r000p000P000h"+hum+"b"+prs+"1L000""The weather today will be "+Description+",RV58,RV48,2802 DMR");
        //client.print("5B4ANU-7>APDR15,WIDE1-1:=3506.1 N/03321.5 E_299/003g005t067r000p000P000h74b10136L000");
        client.println(""); 
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("T:"+cellcy+"c  P:"+prs+"b");
        lcd.setCursor(0, 1);
        lcd.print("H:"+hum+"%  W:"+windD+"");
        
        
     /*
        if (f >= 0)
         {
         client.print("t0"); client.print(f);
         }
          else
          {
        client.print("t"); client.print(f);
        }
        client.print("r...p...P...h"); client.print(h);  
        if (p >= 10000)
        {
        client.print("b"); client.print(p); client.println("testing the arduino"); // 
        }
        else
        {
        client.print("b0"); client.print(p); client.println("testing the arduino"); //
        }
        client.println("5B4ANU-7>APDR15,TCPIP*,qAC,WIDE1-1:> SmallMeteo to APRS"); // 
      
      */
      //  digitalWrite(LED, LOW);
        http.end(); //Close connection
        delay(60000); //10 MIN DELAY 
     //   digitalWrite(LED, HIGH);
        
    }
    
}
void getWeatherData(){   //client function to send/receive GET request data.
  if (client.connect(servername, 80))   
          {                                         //starts client connection, checks for connection
          client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
          client.println("Host: api.openweathermap.org");
          client.println("User-Agent: ArduinoWiFi/1.1");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         result = result+c;
       }

client.stop();                                      //stop client
result.replace('[', ' ');
result.replace(']', ' ');
Serial.println(result);
char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';
StaticJsonBuffer<1024> json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);

if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

String location = root["name"];
String country = root["sys"]["country"];
float temperature = root["main"]["temp"];
float humidity = root["main"]["humidity"];
String weather = root["weather"]["main"];
float pressure = root["main"]["pressure"];
float windspeed = root["wind"]["speed"];
float windDirection = root["wind"] ["deg"];
String description = root["weather"]["description"];
weatherLocation = location;
Country = country;
Description = description;
Temperature = temperature;
Humidity = humidity;
Pressure = pressure;
WindDirection=windDirection;
Windspeed = windspeed;

}
void getAPRSdata(){   //client function to send/receive GET request data.
  if (client.connect(aprsserver, 443))   
          {                                         //starts client connection, checks for connection
          client.println("GET?name=5B4ANU-13&what=loc&apikey=117511.y5T2lut5UFcsj0PY&format=jason");
         // Serial.println("GET name="+Callsign+"&what=wx&apikey="+APIaprs+"");
          client.println("Host: api.aprs.fi/api/");
          client.println("User-Agent: NodeMcuAPRS/1.0");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         result = result+c;
       }

client.stop();                                      //stop client
result.replace('[', ' ');
result.replace(']', ' ');
Serial.println(result);
char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';
StaticJsonBuffer<1024> json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);

if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

String Name = root["name"];
String comment = root["entries"]["comment"];
Comment = comment;

}
