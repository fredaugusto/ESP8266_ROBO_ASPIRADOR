# Robô Aspirador com ESP8266

Projeto de controle para robô aspirador de pó utilizando ESP8266, com motores controlados por ponte L298N, sensor de colisão (botão) e sensor de tensão para monitorar a bateria.

---

## Funcionalidades

- **Controle via página web:**  
  Controle remoto pelo navegador com dois modos:  
  - **Modo Automático:** O robô anda para frente, ao colidir vira e continua. Após um tempo, entra em modo espiral para evitar ficar preso.  
  - **Modo Manual:** Controle em tempo real dos motores para frente, ré, parar e virar para os lados.

- **Sensor de colisão:**  
  Detecta impacto via botão físico, faz manobra para evitar obstáculos.

- **Sensor de tensão da bateria:**  
  Atua como segurança para desligar o robô automaticamente se a tensão da bateria estiver abaixo de 11V, protegendo a saúde da bateria.

- **Indicador LED (LED_BUILTIN):**  
  - Pisca quando o robô está em movimento (frente ou espiral).  
  - Fica aceso fixo quando está colidido.  
  - Apagado quando parado.

- **Lógica automática inteligente:**  
  Alterna entre andar reto e virar ao colidir, e rodar em espiral após um tempo para evitar ficar preso.

---

## Componentes usados

- ESP8266 (ex: NodeMCU, Wemos D1 mini)
- Ponte H L298N para controle dos motores DC
- Dois motores DC com rodas
- Sensor de colisão (botão mecânico)
- Sensor de tensão DC 0-25V
- LED interno do ESP8266 para status

---

## Esquema de ligação dos componentes

### Motores e Ponte L298N

| Função                | ESP8266 Pino | L298N Pino  |
|-----------------------|--------------|-------------|
| Motor A - IN1         | D1           | IN1         |
| Motor A - IN2         | D2           | IN2         |
| Motor B - IN3         | D5           | IN3         |
| Motor B - IN4         | D6           | IN4         |
| Motor A - ENA (PWM)   | D3           | ENA         |
| Motor B - ENB (PWM)   | D4           | ENB         |

### Sensor de colisão (botão)

- Botão conectado entre o pino D7 do ESP8266 e GND.  
- Pino configurado com resistor pull-up interno.

### Sensor de tensão

- Sensor conectado ao pino A0 do ESP8266 para monitorar a tensão da bateria.

---

## Configuração Wi-Fi

O ESP8266 cria um ponto de acesso com:

- SSID: `RoboAspira`  
- Senha: `12345678`

Acesse o IP mostrado na serial (normalmente `192.168.4.1`) via navegador para controlar o robô.

---

## Uso

1. Faça o upload do código para o ESP8266.  
2. Ligue o robô e conecte no ponto de acesso Wi-Fi criado.  
3. Acesse a página web para controlar o robô no modo manual ou automático.  
4. Observe a tensão da bateria exibida na página para garantir operação segura.

---

## Código e personalização

- Ajuste os valores do divisor de tensão no código para medir corretamente a tensão da sua bateria.  
- Altere os pinos conforme o seu hardware, se necessário.

---

## Autor

**Fred Augusto**  
Email: frederico.augusto@outlook.com

---

**Divirta-se e boa limpeza! 🚀🤖**
