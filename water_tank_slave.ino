/*****************************************************************************************************************************
**********************************    Author  : Ehab Magdy Abdullah                      *************************************
**********************************    Linkedin: https://www.linkedin.com/in/ehabmagdyy/  *************************************
**********************************    Youtube : https://www.youtube.com/@EhabMagdyy      *************************************
******************************************************************************************************************************/

#include <ESP8266WiFi.h>

#include <Firebase_ESP_Client.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Dialog Buju PocketBu"
#define WIFI_PASSWORD "33080066"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBhfT2c5X0G44Ma3JMyvdiv1PYw8shKwas"

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "madsampath94@gmail.com"
#define USER_PASSWORD "1qaz2wsx@DSMAX071`"

/* 4. Define the RTDB URL */
#define DATABASE_URL "water-tank-99673-default-rtdb.firebaseio.com" 

#define FLAME_SENSOR_PIN        34
#define DHT_SENSOR_PIN          32


// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void setup()
{
  
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    /* Assign the api key (required) */
    config.api_key = API_KEY;
    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;
    // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
    Firebase.reconnectNetwork(true);
    // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
    // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
    fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);
    Firebase.begin(&config, &auth);
    Firebase.setDoubleDigits(5);
    config.timeout.serverResponse = 10 * 1000;
}

void loop()
{
    
    // Firebase.ready() should be called repeatedly to handle authentication tasks.
    if (Firebase.ready())
    {

        bool statusData = Firebase.RTDB.getInt(&fbdo,"master_tank_motor_status");

        if(statusData){
          if (fbdo.dataType() == "int") {   
          int status = fbdo.intData();
          Serial.print("Master tank status :");
          Serial.println(status);
          }
        }

        bool masterTankWaterLevel = Firebase.RTDB.getString(&fbdo,"master_tank_water_level");

         if(masterTankWaterLevel){
          Serial.println(fbdo.dataType());
          if (fbdo.dataType() == "float" || fbdo.dataType() == "int") {   
          double waterLevel = fbdo.floatData();
          Serial.print("Master tank water level :");
          Serial.println(waterLevel);
          }
        }

         bool slaveTankWaterLevel = Firebase.RTDB.getString(&fbdo,"slave_tank1_water_level");

         if(slaveTankWaterLevel){
          Serial.println(fbdo.dataType());
          if (fbdo.dataType() == "float" || fbdo.dataType() == "int") {   
          double waterLevel = fbdo.floatData();
          Serial.print("Slave tank1 water level :");
          Serial.println(waterLevel);
          }
        }

        if(Firebase.RTDB.setInt(&fbdo, "master_tank_water_level", 10.5)){
            // Serial.print("Unawatuna master water tank current level: ");
            // Serial.println(10.5);
            // Serial.println("");
        }
        else{
            Serial.println("FAILED: " + fbdo.errorReason());
        }

        if(Firebase.RTDB.setInt(&fbdo, "slave_tank1_water_level", 20)){
          //  Serial.print("Unawatuna master water tank current level: ");
          //   Serial.println(10.5);
          //   Serial.println("");
        }
        else{
            Serial.println("FAILED: " + fbdo.errorReason());
        }

    }
    delay(4000);
}