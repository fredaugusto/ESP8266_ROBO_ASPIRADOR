#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- PINOS DO MOTOR E SENSOR ---
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6
#define ENA D3
#define ENB D4
#define BOTOEIRA D7 // Sensor de colisão (botão)
#define PINO_ADC A0 // Entrada para leitura da tensão

ESP8266WebServer server(80);

// --- VARIÁVEIS LED E MOVIMENTO ---
bool emMovimento = false;
bool colidido = false;

unsigned long ultimoBlink = 0;
bool estadoLED = false;
const unsigned long intervaloBlink = 500; // pisca a cada 500ms

// --- VARIÁVEIS DE ESTADO ---
bool modoManual = false;

enum SubModoAuto {
  MODO_BATER_VIRAR,
  MODO_ESPIRAL
};

SubModoAuto estadoAuto = MODO_BATER_VIRAR;

unsigned long tempoUltimaMudanca = 0;
const unsigned long tempoBaterVirar = 30000; // 30s
const unsigned long tempoEspiral = 20000;    // 20s

// --- CONSTANTES PARA LEITURA DA TENSÃO ---
const float ADC_MAX = 1023.0;
const float TENS_MAX_SENSOR = 16.0;   // sensor mede até 25V
const float TENS_MAX_ADC = 1.0;        // tensão máxima no ADC (após divisor)
const float TENS_LIMIAR = 5.0;        // limite de corte da bateria (em volts)

// --- VARIÁVEL PARA TENSÃO ATUAL ---
float tensaoBateriaAtual = 0.0;

// --- FUNÇÕES DE CONTROLE DE MOVIMENTO ---
void parar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  emMovimento = false;
  Serial.println("Parando...");
}

void frente() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  emMovimento = true;
  Serial.println("Indo pra frente...");
}

void re() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  emMovimento = true;
  Serial.println("Recuando...");
}

void virarDireita() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  emMovimento = true;
  Serial.println("Virando à direita...");
}

void virarEsquerda() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  emMovimento = true;
  Serial.println("Virando à esquerda...");
}

// --- LÓGICAS DE MOVIMENTO AUTOMÁTICO ---
void logicaBaterEVirar() {
  if (digitalRead(BOTOEIRA) == LOW) {
    Serial.println(">> Colisão detectada (modo bater e virar)");
    colidido = true;
    parar();
    delay(300);
    re(); delay(800);
    virarDireita(); delay(700);
    frente();
    colidido = false;
  } else {
    frente();
  }
}

void logicaEspiral() {
  if (digitalRead(BOTOEIRA) == LOW) {
    Serial.println(">> Colisão detectada no modo espiral! Executando evasão...");
    colidido = true;
    parar(); delay(300);
    re(); delay(800);
    virarEsquerda(); delay(700);
    colidido = false;
  }

  Serial.println(">> Executando movimento em espiral...");
  analogWrite(ENA, 1023);  // Motor A em velocidade total
  analogWrite(ENB, 700);   // Motor B mais lento = curva
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  emMovimento = true;
}

// --- LEITURA DA TENSÃO DA BATERIA ---
float lerTensaoBateria() {
  int valorADC = analogRead(PINO_ADC);
  float tensaoADC = (valorADC / ADC_MAX) * TENS_MAX_ADC; // tensão no ADC (0-1V)
  float tensaoBateria = tensaoADC * (TENS_MAX_SENSOR / TENS_MAX_ADC);
  return tensaoBateria;
}

// --- INTERFACE WEB ---
const char pagina[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Controle do Robô</title>
  <meta charset="utf-8" />
  <style>
    body { font-family: Arial; text-align:center; margin-top: 30px;}
    button { padding: 15px; margin: 5px; font-size: 16px; width: 120px;}
    #controlesManual { margin-top: 20px;}
  </style>
</head>
<body>
  <h1>Controle do Robô</h1>
  <button onclick="enviar('auto')">Modo Automático</button>
  <button onclick="enviar('manual')">Modo Manual</button>
  <div id="controlesManual" style="display:none;">
    <br>
    <button onclick="enviar('frente')">Frente</button><br>
    <button onclick="enviar('esquerda')">Esquerda</button>
    <button onclick="enviar('parar')">Parar</button>
    <button onclick="enviar('direita')">Direita</button><br>
    <button onclick="enviar('re')">Ré</button>
  </div>
  <br>
  <div><strong>Tensão da bateria: </strong><span id="tensao">Carregando...</span> V</div>
<script>
function enviar(cmd) {
  fetch('/cmd?acao=' + cmd);
  if(cmd === 'manual') {
    document.getElementById('controlesManual').style.display = 'block';
  } else if(cmd === 'auto') {
    document.getElementById('controlesManual').style.display = 'none';
  }
}

function atualizarTensao() {
  fetch('/tensao')
    .then(response => response.text())
    .then(data => {
      document.getElementById('tensao').innerText = data;
    })
    .catch(() => {
      document.getElementById('tensao').innerText = 'Erro';
    });
}

setInterval(atualizarTensao, 2000); // atualiza a cada 2s
atualizarTensao();
</script>
</body>
</html>
)rawliteral";

// --- CONFIGURAÇÕES DE WI-FI ---
const char* ssid = "RoboAspira";
const char* password = "12345678";

void handleRoot() {
  server.send_P(200, "text/html", pagina);
}

void handleCmd() {
  String acao = server.arg("acao");
  Serial.print("Comando recebido via web: ");
  Serial.println(acao);

  if (acao == "frente") frente();
  else if (acao == "re") re();
  else if (acao == "esquerda") virarEsquerda();
  else if (acao == "direita") virarDireita();
  else if (acao == "parar") parar();
  else if (acao == "manual") {
    modoManual = true;
    parar();
    Serial.println(">> MODO MANUAL ativado");
  }
  else if (acao == "auto") {
    modoManual = false;
    estadoAuto = MODO_BATER_VIRAR;
    tempoUltimaMudanca = millis();
    Serial.println(">> MODO AUTOMÁTICO ativado (iniciando com bater e virar)");
  }

  server.send(200, "text/plain", "OK");
}

void handleTensao() {
  server.send(200, "text/plain", String(tensaoBateriaAtual, 2));
}

void atualizarLED() {
  if (colidido) {
    digitalWrite(LED_BUILTIN, LOW); // aceso fixo (LED é ativo LOW no ESP8266)
    return;
  }

  if (emMovimento) {
    unsigned long agora = millis();
    if (agora - ultimoBlink >= intervaloBlink) {
      ultimoBlink = agora;
      estadoLED = !estadoLED;
      digitalWrite(LED_BUILTIN, estadoLED ? LOW : HIGH); // pisca
    }
  } else {
    digitalWrite(LED_BUILTIN, HIGH); // apagado
  }
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando robô...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // apagado inicialmente

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(BOTOEIRA, INPUT_PULLUP);

  parar();

  analogWriteFreq(1000);
  analogWrite(ENA, 1023);
  analogWrite(ENB, 1023);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Wi-Fi criado! SSID: "); Serial.println(ssid);
  Serial.print("Acesse via navegador: http://"); Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/tensao", handleTensao);
  server.begin();
  Serial.println("Servidor web iniciado!");
}

// --- LOOP PRINCIPAL ---
void loop() {
  server.handleClient();

  tensaoBateriaAtual = lerTensaoBateria();
  Serial.print("Tensão da bateria: ");
  Serial.print(tensaoBateriaAtual);
  Serial.println(" V");

  if (tensaoBateriaAtual < TENS_LIMIAR) {
    Serial.println("!! Tensão abaixo de 11V! Desligando motores para proteger bateria.");
    parar();  // para os motores
  } else {
    if (!modoManual) {
      unsigned long agora = millis();

      switch (estadoAuto) {
        case MODO_BATER_VIRAR:
          logicaBaterEVirar();
          if (agora - tempoUltimaMudanca > tempoBaterVirar) {
            estadoAuto = MODO_ESPIRAL;
            tempoUltimaMudanca = agora;
            Serial.println(">> Mudando para modo espiral...");
          }
          break;

        case MODO_ESPIRAL:
          logicaEspiral();
          if (agora - tempoUltimaMudanca > tempoEspiral) {
            estadoAuto = MODO_BATER_VIRAR;
            tempoUltimaMudanca = agora;
            Serial.println(">> Voltando para modo bater e virar...");
          }
          break;
      }
    }
  }
  atualizarLED();
}
