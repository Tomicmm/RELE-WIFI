/*
  RELE-WIFI
  ----------
  Escuela de Educación Secundaria Técnica N° 5 "2 de Abril" - Temperley
  Materia: Montajes de Proyectos Electrónicos
  Alumno: Tomás Colicchia

  Relé WiFi controlado por ESP32-WROOM-32 mediante protocolo MQTT
  contra un servidor Mosquitto, permitiendo encender/apagar una carga
  de 220V de forma remota desde cualquier lugar con acceso al broker.

  Etapa de potencia (hardware, fuera del alcance del código):
    GPIO -> Optoacoplador PC817 -> Transistor MMBT2222A -> Relé SDR-05VDC-SL-C -> Carga 220V
    Alimentación: fuente switching 220V->5V + regulador AMS1117 3.3V para el ESP32

  Librerías necesarias (Arduino IDE -> Administrador de Librerías):
    - WiFi.h        (incluida con el core de ESP32)
    - PubSubClient  (por Nick O'Leary)

  Tópicos MQTT:
    rele/wifi/set    <- comando entrante: "ON" / "OFF"
    rele/wifi/state  -> estado publicado (retained): "ON" / "OFF"
*/

#include <WiFi.h>
#include <PubSubClient.h>

// ---------- CONFIGURACIÓN WIFI ----------
const char* WIFI_SSID     = "TU_RED_WIFI";
const char* WIFI_PASSWORD = "TU_PASSWORD";

// ---------- CONFIGURACIÓN MQTT ----------
const char* MQTT_BROKER   = "IP_O_DOMINIO_DEL_BROKER"; // ej: broker Mosquitto propio o público
const int   MQTT_PORT     = 1883;
const char* MQTT_USER     = "";      // dejar vacío si el broker no pide auth
const char* MQTT_PASS     = "";
const char* MQTT_CLIENT_ID = "esp32-rele-wifi";

const char* TOPIC_SET   = "rele/wifi/set";
const char* TOPIC_STATE = "rele/wifi/state";

// ---------- CONFIGURACIÓN DEL RELÉ ----------
const int RELAY_PIN = 26;           // salida hacia el optoacoplador PC817
const bool RELAY_ACTIVE_LOW = false; // ajustar según cómo quede polarizado el optoacoplador/transistor

// -----------------------------------------

WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool relayState = false;

void setRelay(bool on) {
  relayState = on;
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(RELAY_PIN, level ? HIGH : LOW);
  mqttClient.publish(TOPIC_STATE, on ? "ON" : "OFF", true); // retained
}

void connectWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado. IP: ");
  Serial.println(WiFi.localIP());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();
  msg.toUpperCase();

  if (String(topic) == TOPIC_SET) {
    if (msg == "ON") setRelay(true);
    else if (msg == "OFF") setRelay(false);
  }
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando a MQTT...");
    bool ok;
    if (strlen(MQTT_USER) > 0) {
      ok = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);
    } else {
      ok = mqttClient.connect(MQTT_CLIENT_ID);
    }

    if (ok) {
      Serial.println("conectado");
      mqttClient.subscribe(TOPIC_SET);
      mqttClient.publish(TOPIC_STATE, relayState ? "ON" : "OFF", true);
    } else {
      Serial.print("fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" reintentando en 3s");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  setRelay(false);

  connectWiFi();

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
}
