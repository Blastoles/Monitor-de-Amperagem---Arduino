#include "EmonLib.h"

EnergyMonitor emon1;

// --- CONFIGURAÇÕES DA SUA REDE ELÉTRICA ---
// A maioria dos fogões usa 220V. Se o seu for 127V, mude aqui.
const float TENSAO_REDE = 127.0; 

// Valor cobrado por cada kWh na sua conta de luz (em Reais).
// Um valor médio no Brasil é em torno de R$ 0.95 a R$ 1.05.
const float TARIFA_KWH = 1.12585890; 

// Variáveis para acumular o consumo ao longo do tempo
float consumoAcumulado_kWh = 0.0;
unsigned long tempoUltimaLeitura = 0;

void setup() {
  Serial.begin(9600);
  
  // Calibração do seu sensor (SCT-013 100A/1V)
  emon1.current(A0, 100.0); 
  
  // Marca o tempo inicial
  tempoUltimaLeitura = millis();
  
  Serial.println("Iniciando monitoramento do Fogao...");
  Serial.println("-----------------------------------");
}

void loop() {
  // 1. LÊ A CORRENTE (1480 amostras)
  double Irms = emon1.calcIrms(1480);
  
  // 2. APLICA O FILTRO DE RUÍDO (Ignora leituras abaixo de 0.40A)
  if (Irms < 0.40) {
    Irms = 0.0;
  }
  
  // 3. CALCULA A POTÊNCIA (Watts)
  // Potência = Corrente x Tensão
  double potencia_W = Irms * TENSAO_REDE;
  
  // 4. CALCULA O TEMPO PASSADO (em horas)
  unsigned long tempoAtual = millis();
  unsigned long tempoDecorrido_ms = tempoAtual - tempoUltimaLeitura;
  tempoUltimaLeitura = tempoAtual;
  
  // Converte milissegundos para horas
  float tempoDecorrido_horas = tempoDecorrido_ms / 3600000.0;
  
  // 5. ACUMULA O CONSUMO EM kWh
  // kWh = (Watts * horas) / 1000
  consumoAcumulado_kWh += (potencia_W * tempoDecorrido_horas) / 1000.0;
  
  // 6. CALCULA O CUSTO (R$)
  float custo_Reais = consumoAcumulado_kWh * TARIFA_KWH;
  
  // --- MOSTRA OS DADOS NO MONITOR SERIAL ---
  Serial.print("Corrente: ");
  Serial.print(Irms, 2); // Exibe com 2 casas decimais
  Serial.print(" A  |  ");
  
  Serial.print("Potencia: ");
  Serial.print(potencia_W, 0); // Exibe sem casas decimais (Watts redondos)
  Serial.print(" W  |  ");
  
  Serial.print("Consumo: ");
  Serial.print(consumoAcumulado_kWh, 4);
  Serial.print(" kWh  |  ");
  
  Serial.print("Gasto: R$ ");
  Serial.println(custo_Reais, 4);
  
  // Aguarda 1 segundo para a próxima leitura
  delay(1000);
}