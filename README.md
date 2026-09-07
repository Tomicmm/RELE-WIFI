<p align="center">
  <img src="docs/images/banner.svg" alt="RELE-WIFI banner" width="100%">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/plataforma-ESP32--WROOM--32-22d3ee?style=flat-square" alt="Plataforma ESP32">
  <img src="https://img.shields.io/badge/protocolo-MQTT%20(Mosquitto)-34d399?style=flat-square" alt="Protocolo MQTT">
  <img src="https://img.shields.io/badge/IDE-Arduino-00979D?style=flat-square" alt="Arduino IDE">
  <img src="https://img.shields.io/badge/licencia-MIT-f59e0b?style=flat-square" alt="Licencia MIT">
</p>

# RELE-WIFI

**Escuela de Educación Secundaria Técnica N° 5 "2 de Abril" — Temperley, Buenos Aires**
**Materia:** Montajes de Proyectos Electrónicos
**Grupo:** Relay WiFi
**Alumno:** Tomás Colicchia
**Profesor:** Ing. Martín Leguizamón

## Descripción

Relé WiFi controlado mediante un **ESP32-WROOM-32**, capaz de encender y apagar una carga de forma remota a través de una conexión WiFi y un servidor **MQTT (Mosquitto)**.

El ESP32 se conecta a la red WiFi y se comunica con el broker Mosquitto mediante el protocolo MQTT, enviando y recibiendo comandos para controlar el estado del relé. Para el accionamiento seguro se utiliza un **optoacoplador PC817** y un **transistor MMBT2222A**, que permiten controlar el **relé SDR-05VDC-SL-C** desde el microcontrolador sin exponerlo directamente a la carga de 220V.

Además de la salida de relé, la placa incluye **dos entradas digitales aisladas ópticamente** (cada una con su propio PC817), pensadas para leer el estado de sensores o señales externas de 3.3V/5V sin acoplar eléctricamente esa parte del circuito al ESP32.

La alimentación del circuito se realiza mediante una **fuente switching de 220V a 5V**, con un regulador **AMS1117 de 3,3V** para alimentar correctamente al ESP32. Se incorporan además resistencias, capacitores y diodos de protección.

## Aplicación final

Dispositivo de control remoto para manejar cualquier carga de 220V desde cualquier lugar con conexión a internet (por ejemplo, encender o apagar un artefacto de casa estando de viaje).

## Arquitectura

```
[App/Cliente MQTT] <--> [Broker Mosquitto] <--> [ESP32-WROOM-32] --GPIO--> [PC817] --> [MMBT2222A] --> [Relé SDR-05VDC-SL-C] --> [Carga 220V]
```

<p align="center">
  <img src="docs/images/architecture.svg" alt="Diagrama de arquitectura RELE-WIFI" width="100%">
</p>

| Tópico | Dirección | Valores | Descripción |
|---|---|---|---|
| `rele/wifi/set` | entrante | `ON` / `OFF` | Comando para accionar el relé |
| `rele/wifi/state` | saliente (retained) | `ON` / `OFF` | Estado actual del relé |
| `rele/wifi/entrada1` | saliente (retained) | `ON` / `OFF` | Estado de `entrada1` (vía PC817, GPIO19) |
| `rele/wifi/entrada2` | saliente (retained) | `ON` / `OFF` | Estado de `entrada2` (vía PC817, GPIO18) |

## Diseño de hardware (KiCad)

### Esquemático

<p align="center">
  <img src="docs/images/schematic.png" alt="Esquemático KiCad de RELE-WIFI" width="100%">
</p>

- **Etapa de relé:** `R14 (330Ω)` → `U5 PC817` → `R9 (1k)` → `Q2 MMBT2222A` → bobina de `K1 (SANYOU SRD-05VDC-SL-C)`, con `D1 1N4148` como diodo de protección (flyback) y salida a bornera `salidarele1`.
- **Entradas aisladas:** `entrada1` → `R8 (220Ω)` → `U4 PC817` → `R4 (10k)` → GPIO `IO19`; `entrada2` → `R10 (220Ω)` → `U1 PC817` → `R3 (10k)` → GPIO `IO18`.
- **Alimentación:** entrada de fuente → `D2 SS14` → regulador `U3 AMS1117-3.3` (con `C1 100nF` y `C2 100µF`) → riel de 3.3V que alimenta al ESP32-WROOM-32.
- **Programación/depuración:** botones `SW1 (BOOT)` y `SW2 (RST)` con sus resistencias asociadas, y bornera `J2` para TX/RX/IO0.

### PCB

<p align="center">
  <img src="docs/images/pcb_layout.png" alt="Diseño de PCB de RELE-WIFI en KiCad" width="100%">
</p>

Placa de **89,5 mm x 46,6 mm**, con una zona de exclusión (*keep-out*) reservada junto a la antena del ESP32-WROOM-32 para no afectar la recepción WiFi.

> Los archivos fuente de KiCad (`.kicad_sch` / `.kicad_pcb`) todavía no están en este repo — por ahora estas imágenes son capturas de referencia del diseño.

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
