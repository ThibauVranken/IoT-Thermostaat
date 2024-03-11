#define BLYNK_TEMPLATE_ID "user15"
#define BLYNK_TEMPLATE_NAME "user15@server.wyns.it"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHT_PIN 5  
#define LED_PIN 19
#define POTENTIOMETER_PIN 34

char auth[] = "YLxmRCxDmrnbkLqZqjsNiN1EFrSGyiNs";
char ssid[] = "embed";
char pass[] = "weareincontrol";

DHT dht(DHT_PIN, DHT11);

float gewensteTemp = 0;
float gemetenTemp = 0;
bool ketelStatus = false;
bool useSlider = true;

BLYNK_WRITE(V2)  
{
  if (!useSlider) {
    gewensteTemp = param.asFloat();  
  }
}
BLYNK_WRITE(V7) {
  useSlider = !useSlider;
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
}

void loop() {
  Blynk.run();

  gemetenTemp = dht.readTemperature();

  if (useSlider) {
    int potValue = analogRead(POTENTIOMETER_PIN);
    gewensteTemp = map(potValue, 0, 4096, 0, 30);
  }
  if (gemetenTemp < gewensteTemp) {
    digitalWrite(LED_PIN, HIGH);
    ketelStatus = true;
  } else {
    digitalWrite(LED_PIN, LOW);
    ketelStatus = false;
  }

  Serial.print("Gemeten temperatuur: ");
  Serial.print(gemetenTemp);
  Serial.print(" °C, Gewenste temperatuur: ");
  Serial.print(gewensteTemp);
  Serial.print(" °C, Verwarmingsketel: ");
  Serial.println(ketelStatus ? "Aan" : "Uit");

  Blynk.virtualWrite(V1, gemetenTemp);
  Blynk.virtualWrite(V2, gewensteTemp);
  Blynk.virtualWrite(V3, ketelStatus ? "Aan" : "Uit");
}
