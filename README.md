# Cap-1---Um-Mapa-do-Tesouro
🌱 Sistema de Irrigação Inteligente com ESP32
📌 Descrição

Este projeto implementa um sistema de irrigação automatizado utilizando o microcontrolador ESP32.
O sistema monitora a umidade do solo, temperatura ambiente, pH simulado e a presença de nutrientes essenciais (N, P e K) para determinar quando acionar a bomba de irrigação.

A decisão de irrigar é baseada nos parâmetros específicos da cultura selecionada, como o tomate, por exemplo.

⚙️ Funcionalidades

Leitura de Umidade e Temperatura: Utiliza o sensor DHT22 para monitorar as condições ambientais.

Simulação de pH: Um LDR (Light Dependent Resistor) é usado para estimar o pH do solo.

Controle de Nutrientes: Botões físicos representam a adição de Nitrogênio (N), Fósforo (P) e Potássio (K).

Decisão de Irrigação: Lógica baseada na cultura selecionada para determinar quando irrigar.

Controle de Bomba com Histerese: Evita acionamentos frequentes da bomba.

Interface LCD: Exibe informações em tempo real sobre o sistema.

🌾 Sobre a Cultura

A cultura define as regras de irrigação:

Tomate: umidade mínima de 65%, pH entre 6.0 e 6.8, exige N, P e K.

O sistema só liga a bomba quando:

A umidade está abaixo do mínimo ideal.

O pH está dentro da faixa ideal.

Os nutrientes necessários já foram adicionados (botões pressionados).

Cada planta tem exigências diferentes, garantindo eficiência no uso da água e produtividade da lavoura.

📦 Materiais Utilizados

ESP32

Sensor DHT22 (Temperatura e Umidade)

LDR (Simulação de pH)

Botões (N, P, K)

Relé de 5V

Bomba de Água 5V

LCD I2C 16x2

Protoboard e Jumpers

🔌 Esquema de Ligação

🎥 Demonstração

Assista ao funcionamento do sistema no vídeo abaixo:
https://youtu.be/rFgAwwCuDpw

🛠️ Como Utilizar

Clone o repositório:

git clone https://github.com/britoluana1102-web/Cap-1---Um-Mapa-do-Tesouro.git


Conecte os componentes conforme o esquema de ligação.

Carregue o código no ESP32 utilizando a IDE Arduino.

Abra o Monitor Serial para visualizar os dados em tempo real.

Pressione os botões N, P e K para simular a adição de nutrientes.

Observe o LCD para informações sobre temperatura, umidade, pH e status da bomba.

👥 Autora

Luana Brito da Silva

LDR (Simulação de pH)

Botões (N, P, K)

Relé de 5V

Bomba de Água 5V

LCD I2C 16x2

Protoboard e Jumpers

🔌 Esquema de Ligação

🎥 Demonstração

Assista ao funcionamento do sistema no vídeo abaixo:
https://youtu.be/rFgAwwCuDpw

🛠️ Como Utilizar

Clone o Repositório:

git clone https://github.com/britoluana1102-web/Cap-1---Um-Mapa-do-Tesouro.git


Conecte os Componentes conforme o esquema de ligação.

Carregue o Código no ESP32 utilizando a IDE Arduino.

Abra o Monitor Serial para visualizar os dados em tempo real.

Pressione os Botões N, P e K para simular a adição de nutrientes.

Observe o LCD para informações sobre temperatura, umidade, pH e status da bomba.

👥 Autora
Luana Brito da Silva
