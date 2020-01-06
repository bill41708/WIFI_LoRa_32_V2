// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <LoRa.h>

#define DHTPIN 2     // what pin we're connected to
#define relay_pin 12
#define SensorPin 27
#define inPin  21

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

/*Notice: The DHT10 is different from other DHT* sensor ,it uses i2c interface rather than one wire*/
          /*So it doesn't require a pin.*/

char currentrs[64]; int cnt=0; int num=0; String tmp=""; char currentid[64]; String tmp1="";
String payload="";
String receivedText;
String receivedRssi;
float sensorValue = 0; 
int val = LOW;
int flag = 0;


#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

//#define DHTTYPE DHT10

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
    LoRa.setPins(SS, RST, DI0);
    while (!Serial);  //if just the the basic function, must connect to a computer
    
    Serial.begin(115200); 
    pinMode(relay_pin, OUTPUT);
    pinMode(inPin, INPUT);
    Serial.println("DHTxx test!");
    Wire.begin();

    /*if using WIO link,must pull up the power pin.*/
    // pinMode(PIN_GROVE_POWER, OUTPUT);
    // digitalWrite(PIN_GROVE_POWER, 1);

    dht.begin();
    

    if (!LoRa.begin(BAND)) {
      Serial.println("Starting LoRa failed!");
      //u8x8.drawString(0, 1, "Starting LoRa failed!");
      while (1);
  }
    
  
}

void loop() 
{
    delay(10);
    float temp_hum_val[2] = {0};
    digitalWrite(relay_pin, LOW);
    if(!dht.readTempAndHumidity(temp_hum_val)){
        Serial.print("Humidity: "); 
        Serial.print(temp_hum_val[0]);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(temp_hum_val[1]);
        Serial.println(" *C");
        delay(10);
        
        tmp1 = "Humidity: "+ String(temp_hum_val[0]) + " %\t" + "Temperature: "+ String(temp_hum_val[1]) + " *C";


        //if(int(temp_hum_val[0]) > 90){

    }
    else{
       Serial.println("Failed to get temprature and humidity value.");
    }

    
    delay(10);
    //val = digitalRead(inPin);
    
    if (digitalRead(inPin) == LOW)
    {
      Serial.println("LOW");
      flag = 0;
    }
    else
    {
      Serial.println("HIGH");
      flag = 1;
    }
    
    /*
    val = digitalRead(inPin);
    Serial.println(val);
    */
    
    
    
    delay(1000);
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

    sensorValue = 0;

    sensorValue = analogRead(SensorPin); 
    delay(100); 

    //sensorValue = sensorValue/100.0;
    Serial.print("soil moisture: ");
    Serial.println(sensorValue); 
    delay(30);
    
    if (flag > 0){

      payload="Water flower";
      cnt=(cnt+1) % 10;
      
      for(int i=1; i<4; i++)
      { 
        LoRa.beginPacket(); 
        
        tmp=payload+"_"+String(cnt)+" ";
        LoRa.print(tmp);
        LoRa.print(tmp1);
        //LoRa.endPacket(); //retorno= 1:sucesso | 0: falha
         
        Serial.println("LoraSend:"+tmp+" "+tmp1);
        delay(100);
      }
      

      digitalWrite(relay_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(5000);                       // wait for a second
      digitalWrite(relay_pin, LOW);    // turn the LED off by making the voltage LOW
      delay(100); 
    }





   delay(1500);
}
