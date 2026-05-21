# Monitor de Consumo Elétrico - Arduino & SCT-013 (100A/1V)

Este projeto utiliza um Arduino e um sensor de corrente não invasivo (SCT-013) para monitorar o consumo de energia de equipamentos de alta potência, como fogões e cooktops elétricos. Ele calcula a corrente (A), potência (W), o consumo acumulado (kWh) e o gasto estimado em Reais (R$).

## ⚠️ Atenção ao Modelo do Sensor
Este repositório foi configurado especificamente para o sensor **SCT-013 modelo 100A / 1V**. 

Diferente do modelo padrão da internet (100A / 50mA), a versão de 1V **já possui o resistor de carga (burden resistor) embutido de fábrica**. Por isso, o circuito dispensa o uso do resistor de 330Ω. Se você adicionar um resistor extra em paralelo, as leituras ficarão incorretas.

## Funcionalidades
*   **Leitura em Tempo Real:** Corrente (A) e Potência (W).
*   **Acúmulo de Energia:** Total consumido em kWh durante a sessão.
*   **Cálculo Financeiro:** Custo estimado em Reais (R$) com base na tarifa local.
*   **Filtro de Ruído:** Lógica via código para eliminar leituras fantasmas geradas por interferência eletromagnética quando o equipamento está desligado.

## Componentes Necessários
*   1x Placa Arduino (Uno, Nano, Mega, etc.)
*   1x Sensor de Corrente AC SCT-013 (100A/1V)
*   2x Resistores de 10kΩ
*   1x Capacitor eletrolítico de 100µF
*   Protoboard e Jumpers

## O Circuito (Divisor de Tensão)
O sensor gera um sinal alternado (AC). Como o Arduino só lê tensões positivas (0 a 5V), usamos um circuito divisor de tensão para elevar o ponto zero da onda para 2.5V:

1.  Conecte os dois resistores de 10kΩ em série.
2.  Ligue uma extremidade da série no **5V** do Arduino e a outra no **GND**.
3.  No **ponto central** entre os dois resistores (2.5V), conecte o polo positivo (+) do capacitor de 100µF. O polo negativo (-) vai para o GND.
4.  Conecte um dos fios do SCT-013 no **ponto central** (junto com o capacitor).
5.  Conecte o outro fio do SCT-013 diretamente no pino **A0** do Arduino.

> **Regra de Instalação:** O alicate do sensor deve abraçar **apenas o fio de Fase** do cabo de energia. Abraçar o cabo inteiro (Fase e Neutro) cancela o campo magnético, resultando em uma leitura de 0 Amperes.

## Software e Dependências
Este projeto utiliza a biblioteca **EmonLib**.
1. Abra a IDE do Arduino.
2. Vá em **Ferramentas > Gerenciar Bibliotecas**.
3. Busque por `EmonLib` (da OpenEnergyMonitor) e clique em Instalar.

## Configuração Personalizada
No início do código fonte, você pode (e deve) ajustar os parâmetros da sua rede elétrica:
*   `TENSAO_REDE`: A voltagem da sua tomada (ex: 220.0).
*   `TARIFA_KWH`: O valor que a sua concessionária cobra por cada kWh consumido (ex: 0.95). Pegue o valor total da sua última conta e divida pelos kWh gastos para achar a média exata.
