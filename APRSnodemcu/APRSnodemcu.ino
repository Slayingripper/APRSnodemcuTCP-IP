#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
//#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include "Wire.h"
#include "LiquidCrystal.h"
const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);
int count = 0 ;
///////////////////////////////////////////////////
//#include <DallasTemperature.h>
//#include <stdlib_noniso.h>
//#include <Adafruit_BMP085.h>
//#include <DHT.h>
///////////////////////////////////////////////////
#include <OneWire.h>
//const int LED = D0;
ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial
#define SLEEP_DELAY 3000
#include <ArduinoJson.h>
char airindexs[] = "api.waqi.info";
String STATCY,Airquality,thelocationx,thelocationy,thelocation,Nichumid,Nicwind,Nicpressure,Nictemp;

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
String Message;
float unread;
String Comment ;
String SourceID;
///////////////////////////////////////////////////////
///////////////SATELITE///////////////////////////////
//https://www.n2yo.com/rest/v1/satellite/radiopasses/25544/35.0191/33.74057/0/2/40/&apiKey=DWNZB6-Q6B5GA-M7ND55-4ISG
char satteliteserver[] = "www.n2yo.com";
String Startaz,Endaz,Startutc,Endutc,Satname;

/////planes///////
///https://opensky-network.org/api/states/all?lamin=34.388779&lomin=31.772461&lamax=35.906849&lomax=34.991455
char planeserver[]= "opensky-network.org";
String callsign,posx,posy,origincountry;

/////////////DMR//////////////////////////////////////
//const char DMRAPI = "Yc$hW60YhjAZi.r1MnynE@ee0CSkqPPl.MCVTDR8Wo4cnwd6UMuvLIJMiz3IlS1jCEbte@B7oGjM9xT0gkw@0dLjMO.4E0odEzGTzf$YVhH1Px23Sy2TMO9vz8Ab8YpN";
//char dmrserver[] = "hose.brandmeister.network";
//char newsserver[] = "newsapi.org";
///////////////////////////////////
WiFiClient client;
char servername[]="api.openweathermap.org";              // remote server we will connect to
String result,kaka,pisha,louvin;
byte length;
byte thislength;
byte Temperature;
byte Humidity;
int Pressure;
int WindDirection;
byte Windspeed;
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
  delay(1000);
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
        delay(1000); //  5 second delay 
        getWeatherData();
        delay(1000);
        getAPRSdata();
        delay(1000);
        airindex();
        //getplaneinfo();
        delay(1000);
        getsatdata();
        delay(1000);
        if (!client.connect(host, port)) {
          
            
           return;
        }
 
        client.println("user 5B4ANU pass 15540 vers ESP8266_SM 0.1 filter m/1");  // Insert Callsign and APRS PASSWORD
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
        String temp = String(Temperature);
        Temperature = (Temperature*2)+ 32;
        //String temp;
       // temp = String(Temperature);

        // Humidity
        byte hu = (Humidity);
        String hum ;
        hum=String(hu);

       

        // Winddirection
        int wnd = (WindDirection);
       
        String windD; 
        if( 10 <= wnd && wnd <= 99){
          windD = String(wnd);
          windD = "0" + windD; 
        }
        else if (10 > wnd){
          windD = String(wnd);
          windD = "00" + windD; 
        }
        else{
          windD = String(wnd);
        }
        
        //Windspeed
        byte wnds = 0;
        wnds = (Windspeed);
        //String windS; 
        //windS = String(wnds);
        String windS = String( printf("%02d", wnds));
        //Description
        Serial.print(Airquality);
        Serial.print(thelocationx);
        Serial.print(thelocationy);
        Serial.print(Message);
        Serial.print(Satname);
        Serial.print(Endaz);
        Serial.println(Description);
        Serial.println(windD);
        //checker
        int t;
        Serial.println(origincountry);
        Serial.println(posx);
        Serial.println(posy);
        
        Serial.println(now());

        /*
         
        Serial.println(windS);
        Serial.println(windD);
        Serial.println(Pressure);
        Serial.println(Humidity);
        Serial.println(Temperature);
        Serial.println(Comment);
        Serial.println(SourceID);
        Serial.println(Message);
        Serial.print(unread);
        */
        Serial.print(STATCY);     
     
        int dat = (unread);
        String dis; 
        dis = String(dat);
        //Raw packet that gets sent
        //THE ZEROS ARE WHERE THE DATA GOES
        //winderi
        client.print("5B4ANU-13>APDR15,WIDE1-1:=3506.1 N/03321.5 E_"+windD+"/00"+windS+"g000t"+Temperature+"r000p000P000h"+Humidity+"b"+Pressure+"1L000""The weather today will be "+Description+",RV58,RV48,2802 DMR");
        
        delay(3000);
        client.flush();
       // client.print("5B4ANU-12>APDR15,WIDE1-1:=3506.1 N/03321.5 E_"+windD+"/00"+windS+"g000t"+Temperature+"r000p000P000h"+Humidity+"b"+Pressure+"1L000""The weather today will be "+Description+",RV58,RV48,2802 DMR");

        //client.print("5B4ANU-7>APDR15,WIDE1-1:=3506.1 N/03321.5 E_299/003g005t067r000p000P000h74b10136L000");
        client.println(""); 
       // 35.1520595,33.3476924
        client.println("5B4ANU-7>APDR15,TCPIP*,qAC,T2ITALY:=3510.10N/03320.50E_"+windD+"/00"+Nicwind+"g000t"+Nictemp+"r000p000P000h"+Nichumid+"b"+Nicpressure+"1L000"" The AirQuality index now is "+Airquality+"");
        
        while(count < 10){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T:"+temp+"c  P:"+Pressure+"b");
        lcd.setCursor(0, 1);
        lcd.print("H:"+hum+"%  W:"+windD+""+(char)223+"");
        delay (5000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nicosia quality");
         lcd.setCursor(0, 1);
         lcd.print("Index:"+Airquality+"");
         delay(5000);
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("W:"+Nicwind+"m/s""  T:"+Nictemp+"c");
         lcd.setCursor(0, 1);
         lcd.print("P:"+Nicpressure+"b""   H:"+Nichumid+"%");
        delay(5000);
         lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(""+Satname+"");
        lcd.setCursor(0, 1);
        lcd.print("S:"+Startaz+""+(char)223+" E:"+Endaz+""+(char)223+"");
        delay(5000);
        lcd.clear();
        lcd.print("Messages   "+dis+"");
        delay (2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("From:"+SourceID+"");
        lcd.setCursor(0, 1);
        lcd.print(""+Message+"");
        length = Message.length();
        delay(2000);
        if( length >= 16){ 
        for (int positionCounter = 0; positionCounter < length; positionCounter++) {
           // scroll one position left:
             lcd.scrollDisplayLeft();
            // wait a bit:
             delay(1500);
        }
     
        }
       
        count++;
       
        client.flush();
        Serial.flush();
        }
        
      //  digitalWrite(LED, LOW);
        http.end(); //Close connection
        
        
        delay(6000); //10 MIN DELAY 
     //   digitalWrite(LED, HIGH);
    }
}


void getWeatherData(){   //client function to send/receive GET request data.
  if (client.connect(servername, 80))   
          {                                         //starts client connection, checks for connection
          client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
          client.println("Host: api.openweathermap.org");
          client.println("User-Agent: self.user_agent");
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
StaticJsonBuffer<768>json_buf;
//DynamicJsonBuffer json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);
if (!root.success())
  {
    Serial.println("parseObject() failed");
  }
byte temperature = root["main"]["temp"];
byte humidity = root["main"]["humidity"];
int pressure = root["main"]["pressure"];
byte windspeed = root["wind"]["speed"];
byte windDirection = root["wind"] ["deg"];
String description = root["weather"]["description"];

Description = description;
Temperature = temperature;
Humidity = humidity;
Pressure = pressure;
WindDirection=windDirection;
Windspeed = windspeed;

}

void getAPRSdata(){   //client function to send/receive GET request data.

  if (client.connect(aprsserver, 80 ))   
          {      
            //starts client connection, checks for connection
        client.println("GET /api/get?what=msg&dst=5B4ANU-13&apikey=117511.y5T2lut5UFcsj0PY&format=jason HTTP/1.0");
    //     client.println("GET /api/get?name=5B4ANU-13&what=loc&apikey=117511.y5T2lut5UFcsj0PY&format=jason HTTP/1.0");
        // client.println("GET /api/active/?limit=10&region=2&country=280 HTTP/1.0");
         //client.println("GET /v2/top-headlines?sources=bbc-news&apiKey=09595a6764954900b3a241f586f76630 HTTP/1.1");
        //  client.println("Host: api.aprs.fi");
         
      //    client.println("Host: hose.brandmeister.network");
          client.println("User-Agent: Mozilla/5.0 (compatible; Rigor/1.0.0; http://rigor.com)");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  char endOfHeaders[] = "\r\n\r\n";
 client.setTimeout(10000);
if (!client.find(endOfHeaders)) {
  Serial.println(F("Invalid response"));

    return;
}
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char d = client.read();                     //gets byte from ethernet buffer
         kaka = kaka+d;
       }
       

client.stop();                                      //stop client
//kaka.replace('[', ' ');
//kaka.replace(']', ' ');
Serial.println(kaka);
char jsonArray [kaka.length()+1];
kaka.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[kaka.length()+1] = '\0';
StaticJsonBuffer<2048>json_buf;
//DynamicJsonBuffer  json_buf;
JsonObject& parsed = json_buf.parseObject(jsonArray);


if (!parsed.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

byte Found = parsed["found"];
String sourceID = parsed["entries"][0]["srccall"];
String message = parsed["entries"][0]["message"];

unread = Found;
Message = message;
SourceID = sourceID;

}
void getplaneinfo(){   //client function to send/receive GET request data.

  if (client.connect(planeserver, 80 ))   {

          client.println("GET /api/states/all?lamin=34.388779&lomin=31.772461&lamax=35.906849&lomax=34.991455 HTTP/1.1");
          client.println("Host: opensky-network.org");
          client.println("User-Agent: Mozilla/5.0 (compatible; Rigor/1.0.0; http://rigor.com)");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  char endOfHeaders[] = "\r\n\r\n";
 client.setTimeout(10000);
if (!client.find(endOfHeaders)) {
  Serial.println(F("Invalid response"));

    return;
}
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char e = client.read();                     //gets byte from ethernet buffer
         pisha = pisha+e;
       }
       

client.stop();                                      //stop client
//pisha.replace('[', ' ');
//pisha.replace(']', ' ');
Serial.println(pisha);
char jsonArray [pisha.length()+1];
pisha.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[pisha.length()+1] = '\0';
StaticJsonBuffer<1500>json_buf;
//DynamicJsonBuffer  json_buf;
JsonObject& picked = json_buf.parseObject(jsonArray);


if (!picked.success())
  {
    Serial.println("parseObject() failed 2");
    return;
  }

byte Found = picked["states"];
String Callsign = picked["states"][0][1];
String Posy = picked["states"][0][5];
String Posx = picked["states"][0][6];
String Origincountry = picked["states"][0][2];

callsign = Callsign;
posy = Posy;
posx = Posx;
origincountry = Origincountry;
}

void airindex(){   //client function to send/receive GET request data.

  if (client.connect(airindexs, 80 ))   {

          client.println("GET /feed/Nicosia/?token=65174f79ef3f8ffe9d5af54f4c1ece468a59b6f0 HTTP/1.0");
          client.println("Host: api.waqi.info");
          client.println("User-Agent: Mozilla/5.0 (compatible; Rigor/1.0.0; http://rigor.com)");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  char endOfHeaders[] = "\r\n\r\n";
 client.setTimeout(10000);
if (!client.find(endOfHeaders)) {
  Serial.println(F("Invalid response"));

    return;
}
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char e = client.read();                     //gets byte from ethernet buffer
         pisha = pisha+e;
       }
       

client.stop();                                      //stop client
//pisha.replace('[', ' ');
//pisha.replace(']', ' ');
Serial.println(pisha);
char jsonArray [pisha.length()+1];
pisha.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[pisha.length()+1] = '\0';
StaticJsonBuffer<1500>json_buf;
//DynamicJsonBuffer  json_buf;
JsonObject& picked = json_buf.parseObject(jsonArray);


if (!picked.success())
  {
    Serial.println("parseObject() failed 2");
    return;
  }
//35.1520595,33.3476924
String Statusindex = picked["data"]["city"]["name"];
String aqi = picked["data"]["aqi"];
String location = picked["data"]["city"]["geo"];
String humidity21 = picked["data"]["iaqi"]["h"]["v"];
String nicpressure = picked["data"]["iaqi"]["p"]["v"];
String nicwind = picked["data"]["iaqi"]["w"]["v"];
String nictemp = picked["data"]["iaqi"]["t"]["v"];
Nicwind = nicwind;
Nictemp = nictemp;
Nicpressure = nicpressure;
Nichumid =humidity21;
Airquality = aqi;
thelocation = location;
//thelocationx = getValue(location,',');
//thelocationy = getValue(location,',');
STATCY = Statusindex;
}

void getsatdata(){   //client function to send/receive GET request data.
  if (client.connect(satteliteserver, 80))   
          {                                         //starts client connection, checks for connection
          client.println("GET /rest/v1/satellite/radiopasses/25544/35.0191/33.74057/0/1/40/&apiKey=DWNZB6-Q6B5GA-M7ND55-4ISG HTTP/1.0");
          client.println("Host: www.n2yo.com");
          client.println("User-Agent: Mozilla/5.0 (compatible; Rigor/1.0.0; http://rigor.com)");
          client.println("Connection: close");
          client.println();     
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
         Serial.println();
       }
  while(client.connected() && !client.available())
  delay(100);                                          //waits for data
  char endOfHeaders[] = "\r\n\r\n";
 client.setTimeout(10000);
if (!client.find(endOfHeaders)) {
  Serial.println(F("Invalid response"));

    return;
} 
                                         //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char x = client.read();                     //gets byte from ethernet buffer
         louvin = louvin+x;
       }

client.stop();                                      //stop client
//louvin.replace('[', ' ');
//louvin.replace(']', ' ');
Serial.println(louvin);
char jsonArray [louvin.length()+1];
louvin.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[louvin.length() + 1] = '\0';
StaticJsonBuffer<1024>json_buf;
//DynamicJsonBuffer json_buf;
JsonObject &rootbeer = json_buf.parseObject(jsonArray);

if (!rootbeer.success())
  {
    Serial.println("parseObject() failed");
  }
/*
if (!root.success())
  {
    Serial.println("parseObject() failed");
  }
*/

String satname = rootbeer["info"]["satname"];
String startaz = rootbeer["passes"][0]["startAz"];
String endaz = rootbeer["passes"][0]["endAz"];
String startutc = rootbeer["passes"][0]["startUTC"];
String endutc = rootbeer["passes"][0]["endUTC"];

Startaz =startaz;
Endaz = endaz;
Startutc= startutc;
Endutc = endutc;
Satname = satname;
}
