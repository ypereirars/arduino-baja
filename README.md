# Código Arduino
Este projeto consiste em conectar um Arduino - que será responsável pela leitura de sensores em um veículo Baja - ao módulo ESP8266 - o qual transmitirá os dados via MQTT para um [Broker](https://github.com/ypereirars/mqtt-broker).

## Requisitos
* Arduino
* ESP8266
* FTDI (para programar o ESP8266)

## Como configurar a IDE do Arduino para programar o ESP8266
Para programar o ESP8266 utilizando a IDE do Arduino, será necessário realizar as seguintes etapas:
* Baixar e instalar o esptool.py (seguir os passos em): [https://github.com/themadinventor/esptool]
* Incluir "http://arduino.esp8266.com/stable/package_esp8266com_index.json" em: Arquivo>Preferências>'Urls Adicionais de Genrenciadores de placas";
* Fazer o download do gerenciador de placas: Ferramentas>Placa>Gerenciador de Placas, e procurar por "ESP8266";
* Fazer o download da biblioteca [PubSubClient](https://github.com/knolleary/pubsubclient);
* Instalar a biblioteca recém baixada em: Sketch>Incluir Biblioteca>Adicionar Biblioteca .ZIP.
Feito isso, a IDE do Arduino já está configurada para programar o ESP8266.
IMPORTANTE: 
* Deve-se selecionar a placa do ESP8266: Ferramenta>Placa>Generic Module
* "Upload Speed" deve ser a mesma que "Serial.begin(...)"

## Como ligar a placa ao FTDI
![Conexão ESP8266 - FTDI](https://cloud.githubusercontent.com/assets/4396233/16360601/f2f50206-3b41-11e6-9ae1-f17e4855c2fb.png)

IMPORTANTE: O ESP8266 deve resetado antes de fazer o upload do código. Todo o circuito deve ser ligado em 3.3V.

![Pinos ESP8266](https://cloud.githubusercontent.com/assets/4396233/16360656/e4278e04-3b43-11e6-8be6-44a411517cc0.png)

Pinos ESP8266

## Referências
* https://github.com/douglaszuqueto/ESP8266_MQTT
