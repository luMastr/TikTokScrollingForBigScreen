#include <SupabaseArduino.h>
const int treadmillPin = 2; // D2
volatile int controller = 0;
const float distancePerCheck = 0.134; // expressed in meters; 13.4 cm is the distance of one cycle
unsigned long previousTime;
float velocityMs;
double velocityKmh;
double km;
unsigned long currentTime;
// Replace with your Supabase URL, table name, and API key
#define supabaseUrl  "https://yvwdzbtvywhdwmbvosch.supabase.co"
#define tableRealtime  "Speed"
#define supabaseApiKey  "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inl2d2R6YnR2eXdoZHdtYnZvc2NoIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MDk2MjgyOTMsImV4cCI6MjAyNTIwNDI5M30.0TD7s5E8ZOk7L1kCJScuDF9niI65zW-kExOSVPDz1nE"
const char *ssid = "FabulousNet";
const char *password = "25jan2022";
// Setup the Supabase client
Supabase supabaseClient(ssid, password, supabaseUrl, supabaseApiKey, tableRealtime);
// Replace with your network credentials
void setup() {
  pinMode(treadmillPin, INPUT);
  // Begin the connection to the database
  supabaseClient.begin();
}
void loop() {
  if (digitalRead(treadmillPin) == HIGH && controller == 0) {
    currentTime = micros();
    unsigned long timeDifference = currentTime - previousTime;
    velocityMs = distancePerCheck / (timeDifference / 1000000.0);
    velocityKmh = velocityMs * 3.6;
    km = km + (distancePerCheck/1000);
    
    Serial.print("Actual velocity: ");
    Serial.print(velocityKmh);
    Serial.println(" km/h");
    Serial.print("Actual distance: ");
    Serial.print(km);
    Serial.println(" km");

    controller = 1;
    previousTime = currentTime;
  } else if (digitalRead(treadmillPin) == LOW) {
    controller = 0;
  }

  static unsigned long lastUpdateTime = 0;
  if (currentTime - lastUpdateTime >= 500000) {
    supabaseClient.update(1, "speed", velocityKmh);
    supabaseClient.update(1, "distance", km);
    Serial.println("Sent!");
    lastUpdateTime = currentTime;
  }
}