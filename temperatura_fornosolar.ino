#include <SPI.h>
#include <SD.h>

// Defina os pinos para cada sensor
const int ambientePin = A0;      // Sensor de Temperatura Ambiente no pino A0
const int internaPin = A1;       // Sensor de Temperatura Interna no pino A1
const int referenciaPin = A2;    // Sensor de Temperatura de Referência no pino A2
const int amostraPin = A3;       // Sensor de Temperatura de Amostra no pino A3

// Constantes para o cálculo
const float BETA = 3950;                  // Coeficiente beta do termistor
const float SERIES_RESISTOR = 10000;      // Resistor em série com o sensor (10k ohms)
const float NOMINAL_RESISTANCE = 10000;   // Resistência do termistor a 25ºC

unsigned long lastMillis = 0;  // Armazenar o tempo da última gravação
const unsigned long interval = 300000; // 5 minutos (300.000 ms)

File dataFile; // Objeto para escrever no arquivo do cartão SD

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial

  // Inicia o cartão SD
  if (!SD.begin(4)) { // Pino 4 é o pino CS (Chip Select) para o cartão SD
    Serial.println("Falha ao inicializar o cartão SD!");
    return;
  }
  Serial.println("Cartão SD inicializado.");

  // Cria ou abre o arquivo "temperaturas.txt"
  dataFile = SD.open("temperaturas.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Erro ao abrir o arquivo!");
    return;
  }

  // Escreve um cabeçalho no arquivo
  dataFile.println("Data e Hora,Temperatura Ambiente,Temperatura Interna,Temperatura de Referência,Temperatura de Amostra");
  dataFile.close(); // Fecha o arquivo após o cabeçalho
}

void loop() {
  unsigned long currentMillis = millis();

  // Se passaram 5 minutos (300000 ms)
  if (currentMillis - lastMillis >= interval) {
    lastMillis = currentMillis;  // Atualiza o tempo da última gravação

    // Lê cada sensor e converte para temperatura em Celsius
    String tempAmbiente = readTemperature(ambientePin);
    String tempInterna = readTemperature(internaPin);
    String tempReferencia = readTemperature(referenciaPin);
    String tempAmostra = readTemperature(amostraPin);

    // Armazena as leituras no cartão SD
    storeTemperature(tempAmbiente, tempInterna, tempReferencia, tempAmostra);

    // Exibe as temperaturas no monitor serial
    Serial.print("Temperatura Ambiente: ");
    Serial.println(tempAmbiente);

    Serial.print("Temperatura Interna: ");
    Serial.println(tempInterna);

    Serial.print("Temperatura de Referência: ");
    Serial.println(tempReferencia);

    Serial.print("Temperatura de Amostra: ");
    Serial.println(tempAmostra);

    Serial.println("------------------------");
  }
}

String readTemperature(int pin) {
  int sensorValue = analogRead(pin); // Lê o valor analógico do sensor

  // Converte o valor analógico para resistência
  float resistance = (1023.0 / sensorValue) - 1.0;
  resistance = SERIES_RESISTOR / resistance;

  // Converte a resistência para temperatura em Celsius usando a equação de Steinhart-Hart
  float temperature;
  temperature = resistance / NOMINAL_RESISTANCE;      // (R/Ro)
  temperature = log(temperature);                     // ln(R/Ro)
  temperature /= BETA;                                // 1/B * ln(R/Ro)
  temperature += 1.0 / (25.0 + 273.15);               // + (1/To)
  temperature = 1.0 / temperature;                    // Invertendo
  temperature -= 273.15;                              // Convertendo Kelvin para Celsius

  // Converte a temperatura para uma string e retorna
  return String(temperature, 2) + " °C";
}

void storeTemperature(String tempAmbiente, String tempInterna, String tempReferencia, String tempAmostra) {
  // Abre o arquivo para escrever as temperaturas
  dataFile = SD.open("temperaturas.txt", FILE_APPEND);
  if (dataFile) {
    // Adiciona a data e hora (pode ser modificado para incluir um relógio de tempo real)
    String currentTime = String(millis() / 1000); // Simulação do tempo em segundos desde o início
    dataFile.print(currentTime);
    dataFile.print(",");  // Separador para os dados

    // Armazena as temperaturas no arquivo
    dataFile.print(tempAmbiente);
    dataFile.print(",");
    dataFile.print(tempInterna);
    dataFile.print(",");
    dataFile.print(tempReferencia);
    dataFile.print(",");
    dataFile.println(tempAmostra);

    // Fecha o arquivo após gravar
    dataFile.close();
  } else {
    Serial.println("Erro ao abrir o arquivo para gravação!");
  }
}
