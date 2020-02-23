/*DATA LOG HUMIDITY, TEMPERATURE, AND PRESSURE
Ramadhita Umitaibatin, 5/7/2019
Connected to microSD and serial monitor (if connected to IDE)
The data will be transmitted in csv file
*/
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 pressure;
#define ALTITUDE 1655.0 // Altitude in meters
#define DHTPIN D4
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
 
const int chipSelect = D8;
File myFile;
 
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; 
  }
 
  Serial.print("Initializing SD card...");
 
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  dht.begin();

//inisiasi pressure
   if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail,matikan alat dan nyalakan lagi\n\n");
    while(1); // Pause forever.
  }
  pinMode(D0,OUTPUT);
  digitalWrite(D0, HIGH);
  delay(5000);
}
 
void loop()
{ 
  char status;
  double T,P,p0,a;
  myFile = SD.open("dht22.csv", FILE_WRITE);
      // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 // float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) // || isnan(f) 
  {
    Serial.println("Failed to read from DHT sensor!");
   // return;
  } else
    {
      Serial.print("Humidity (%) = ");
      Serial.println(h);
      Serial.print("Temperature (C) = ");
      Serial.println(t);
    }
  //Masukan tekanan
  status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
      
  // if the file opened okay, write to it:
  if (myFile) 
  {
    Serial.print("opened dht22.csv...");
    myFile.print(h);
    myFile.print(",");
    myFile.print(t);
    myFile.print(",");
    myFile.print(P);
    myFile.print(",");
    myFile.println(P*0.0295333727,2);
    //myFile.println(f);
    // close the file:
    myFile.close();
    Serial.println("closed dht22.csv.");
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening dht22.csv");
  }
  delay(3600000);
}
