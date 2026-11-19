#  SmartCity Monitor

Sistema IoT para monitoramento simulado da qualidade do ar e poluição sonora em ambientes urbanos.

O sistema coleta dados simulados de qualidade do ar e ruído, utiliza coordenadas GPS virtuais e transmite as informações por MQTT para visualização em um dashboard Node-RED.

##  Tecnologias utilizadas

- ESP32 e Arduino C++
- Wokwi (simulação)
- ArduinoJson
- MQTT e HiveMQ
- Node-RED e Dashboard 2.0
- Worldmap

##  Sensores simulados

| Sensor de referência | Função | Simulação |
|---|---|---|
| PMS5003 | Qualidade do ar (PM2.5) | Potenciômetro |
| INMP441 | Monitoramento de ruído | Potenciômetro |
| NEO-6M | Localização GPS | Coordenadas virtuais |

##  Funcionamento

1. O ESP32 realiza a leitura dos potenciômetros.
2. Os valores são convertidos em indicadores simulados de qualidade do ar e ruído.
3. O sistema classifica os resultados em bom, médio ou ruim.
4. As coordenadas GPS virtuais são atualizadas a cada 10 segundos.
5. Os dados são organizados em JSON e enviados via MQTT.
6. O Node-RED recebe e apresenta as informações em um dashboard interativo.

##  Comunicação MQTT

- **Broker:** `broker.hivemq.com`
- **Porta:** `1883`
- **Tópico:** `smartcity/projeto001/estacao01/dados`

##  Visualização

O dashboard Node-RED permite acompanhar os indicadores ambientais e visualizar a localização virtual da estação em um mapa.

##  Observação

O projeto foi desenvolvido inteiramente em ambiente de simulação. Os potenciômetros representam os sensores ambientais e as coordenadas GPS são fictícias. Portanto, os valores não representam medições reais.

##  Informações acadêmicas

**Curso:** Engenharia de Software  
**Instituição:** FIAP  
**Projeto:** SmartCity Monitor  
**Ano:** 2026