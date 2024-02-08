int light;

//Librerias a usar
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Definir las credenciales de acceso wifi (no debe ser 5g) */
#define WIFI_SSID "monica2.4"
#define WIFI_PASSWORD "sebastian23"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Escribir la API Key (esta se encuentra en firebase, en configuracion del proyecto, ventana general) */
#define API_KEY "AIzaSyDelNZNxn62IZuo9RLVcZbtPCwuF2AquTM"

/* 3. escribir RTDB URL (Se encuentra en firebase en la seeccion de realtime database)*/
#define DATABASE_URL "ldr-lectura-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Escribir mail y contraseña creada en la autentificacion de firebase, esta puede ser inventada, no es necesario que sea real */
#define USER_EMAIL "seba@gmail.com"
#define USER_PASSWORD "123456"

// Definiendo los objetos de fire base
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void setup() {
  Serial.begin(9600);         // Initialise the serial monitor

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado: ");
  Serial.println(WiFi.localIP());
  //configuracion de FireBase
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Asignar la api key (required) */
  config.api_key = API_KEY;

  /* Asignar las credenciales de los usuarios*/
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* asignar la RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Asignar la función de devolución de llamada para la tarea de generación de token de larga duración */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Comenta o pasa el valor falso cuando la reconexión WiFi será controlada por tu código o una biblioteca de terceros, como WiFiManager
  Firebase.reconnectNetwork(true);

  // Desde la versión 4.4.x, se utilizó el motor BearSSL, por lo que se debe establecer el búfer SSL.
  // Las transmisiones de datos grandes pueden requerir un búfer RX más grande; de lo contrario, pueden ocurrir problemas de conexión o tiempos de espera en la lectura de datos.
  fbdo.setBSSLBufferSize(4096 /* Tamaño del búfer Rx en bytes de 512 a 16384 */, 1024 /* Tamaño del búfer Tx en bytes de 512 a 16384 */);

  // Se requieren las credenciales de WiFi para Pico W
  // debido a que no tiene la función de reconexión.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  config.wifi.clearAP();
  config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

  // O utiliza el método de autenticación heredado
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<secreto de la base de datos>";

  // Para conectar sin autenticación en el modo de prueba, consulta Authentications/TestMode/TestMode.ino

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Asegúrate de que la memoria libre del dispositivo no sea inferior a 80 KB para ESP32 y 10 KB para ESP8266,
  // de lo contrario, la conexión SSL fallará.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);
}

void loop() {
  
  light = analogRead(A0);    // Read the analog value of the LDR
  //float voltaje=light*(5.0/1024.0);
  Serial.println(light);         // Show the value in the serial monitor
  Firebase.setFloat(fbdo,"/LDR", light);

  delay(5000);                     // Pause 100ms
  
}