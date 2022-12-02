#define ESP32
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

//liberia de sensores
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "INFINITUM0657"
#define WIFI_PASSWORD "Elsi123321"

//#define WIFI_SSID "Sr.'s Galaxy A52s 5G"
//#define WIFI_PASSWORD "wotq7673"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBqD8lhbP7WJxGLNddJ70lvDZqoLiHbp_s"

/* 3. Define the RTDB URL */
#define DATABASE_URL "esp32mv-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "vilbom@outlook.es"
#define USER_PASSWORD "CONTRASEÑA"

#define LDR 34
#define DHTPIN 4
#define DHTTYPE DHT11
#define DS18B20 21

DHT dht(DHTPIN , DHTTYPE);
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);

Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

void setup(){

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop(){
  sensors.requestTemperatures();
  int luz = analogRead(LDR);
  float h = dht.readHumidity();
  float t = sensors.getTempCByIndex(0);

  delay(5000);

  Serial.println("dht: " + String(h));
  Serial.println("ds18: " + String(t));
  Serial.println("LDR: " + String(luz));

  Firebase.pushInt(fbdo, "/light", luz);
  Firebase.pushFloat(fbdo, "/humidity", h);
  Firebase.pushFloat(fbdo, nodo + "/temperature", t);
}