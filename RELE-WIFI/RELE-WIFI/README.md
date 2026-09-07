# RELE-WIFI

**Escuela de Educación Secundaria Técnica N° 5 "2 de Abril" — Temperley, Buenos Aires**
**Materia:** Montajes de Proyectos Electrónicos
**Grupo:** Relay WiFi
**Alumno:** Tomás Colicchia
**Profesor:** Ing. Martín Leguizamón

## Descripción

Relé WiFi controlado mediante un **ESP32-WROOM-32**, capaz de encender y apagar una carga de forma remota a través de una conexión WiFi y un servidor **MQTT (Mosquitto)**.

El ESP32 se conecta a la red WiFi y se comunica con el broker Mosquitto mediante el protocolo MQTT, enviando y recibiendo comandos para controlar el estado del relé. Para el accionamiento seguro se utiliza un **optoacoplador PC817** y un **transistor MMBT2222A**, que permiten controlar el **relé SDR-05VDC-SL-C** desde el microcontrolador sin exponerlo directamente a la carga de 220V.

La alimentación del circuito se realiza mediante una **fuente switching de 220V a 5V**, con un regulador **AMS1117 de 3,3V** para alimentar correctamente al ESP32. Se incorporan además resistencias, capacitores y diodos de protección.

## Aplicación final

Dispositivo de control remoto para manejar cualquier carga de 220V desde cualquier lugar con conexión a internet (por ejemplo, encender o apagar un artefacto de casa estando de viaje).

## Arquitectura

```
[App/Cliente MQTT] <--> [Broker Mosquitto] <--> [ESP32-WROOM-32] --GPIO--> [PC817] --> [MMBT2222A] --> [Relé SDR-05VDC-SL-C] --> [Carga 220V]
```

- Tópico `rele/wifi/set` (entrante): `"ON"` / `"OFF"`
- Tópico `rele/wifi/state` (saliente, retained): `"ON"` / `"OFF"`

## Hardware / Lista de materiales

| # | Componente | Motivo de elección |
|---|---|---|
| 1 | Módulo ESP32-WROOM-32 | Barato, confiable y fácil de conseguir |
| 2 | Fuente switching 220V a 5V 1A | Tamaño, potencia y precio; ya conocida |
| 3 | Optoacoplador PC817 | Único disponible en SMD y en pack de 10 |
| 4 | Relé SDR-05VDC-SL-C | El más fácil de conseguir y montar |
| 5 | Placa epoxi | Evita problemas de despegue de cobre del fenólico |
| 6 | AMS1117 3.3V | Indispensable para alimentar el ESP embebido |
| 7 | Resistencias: 2x 1k, 1x 330, 1x 10k (SMD) | Ahorro de espacio |
| 8 | Capacitores: 100nF y 100uF (SMD) | Ahorro de espacio |
| 9 | Diodo 1N4148 y diodo SS14 | Protección del circuito |
| 10 | Transistor MMBT2222A | Mejor relación calidad-precio disponible |
| 11 | 2 borneras de 2 polos | Fáciles de modificar; evitan soldar cables directo |

### Costos aproximados (ARS)

| Componente | Costo |
|---|---|
| ESP32 | $9.000 |
| Fuente switching 220V a 5V 1A | $4.000 |
| Optoacoplador PC817 | $500 |
| Relay SDR-05VDC-SL-C | $1.800 |
| Placa epoxi | $10.000 |
| AMS1117 3.3V | $270 |
| Resistencias (2x 1k, 1x 330, 1x 10k) | $4.000 |
| Capacitores (100nF y 100uF) | $500 |
| Diodo 1N4148 y diodo SS14 | $500 |
| Transistor MMBT2222A | $160 |
| 2 borneras de 2 polos | $1.500 |
| **TOTAL ESTIMADO** | **~$30.000 – $40.000** |

## Software (Arduino IDE)

1. Instalar el soporte de placas ESP32 (Boards Manager → buscar "esp32" → paquete de Espressif).
2. Seleccionar la placa (ej. "ESP32 Dev Module").
3. Instalar la librería **PubSubClient** (por Nick O'Leary) desde el Administrador de Librerías.
4. Abrir `rele_wifi/rele_wifi.ino` y completar:
   ```cpp
   const char* WIFI_SSID     = "TU_RED_WIFI";
   const char* WIFI_PASSWORD = "TU_PASSWORD";
   const char* MQTT_BROKER   = "IP_O_DOMINIO_DEL_BROKER";
   ```
5. Cargar el sketch al ESP32.

## Servidor Mosquitto

Se necesita un broker MQTT accesible (Mosquitto instalado en un servidor propio, Raspberry Pi, VPS, o un broker público para pruebas). Configuración mínima recomendada para pruebas locales:

```bash
sudo apt install mosquitto mosquitto-clients
sudo systemctl enable mosquitto
```

Para pruebas manuales:
```bash
mosquitto_sub -h IP_DEL_BROKER -t rele/wifi/state
mosquitto_pub -h IP_DEL_BROKER -t rele/wifi/set -m "ON"
```

## Tareas y cronograma (aprox.)

**Tomás Colicchia:** conexión ESP32, cableado, montaje físico, diseño y programación, servidor Mosquitto, integración final, ajustes del servidor.

| Semana | Tareas | Horas |
|---|---|---|
| 1 | Diseño de placa (5h) + Compra de materiales (1h) | 6h |
| 2 | Montaje mecánico (4h) + Cableado inicial (3h) | 7h |
| 3 | Programación base (6h) + Pruebas WiFi (4h) + Control del relay (3h) | 13h |
| 4 | Integración total (6h) + Ajustes (4h) | 10h |

**Duración total estimada:** 4 semanas (48–60 horas).

## Documentación

El TP original de la materia está en [`docs/TP_Relay_WiFi_Tomas_Colicchia.pdf`](docs/TP_Relay_WiFi_Tomas_Colicchia.pdf).

## Licencia

MIT
