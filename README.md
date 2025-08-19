# 🤖 RoboAspira — Robô Aspirador DIY com ESP8266

Projeto de um robô aspirador inteligente feito com componentes simples e um ESP8266 (NodeMCU).  

---

## 📦 Funcionalidades

- 🔌 Cria rede Wi-Fi local (Access Point)
- 🌐 Interface web de controle no navegador
- 🕹️ Dois modos de operação:
  - **Manual**: controle direto dos motores via navegador
  - **Automático**: alterna entre dois padrões:
    - **Bater e virar** (detecção de obstáculos com botão de colisão)
    - **Movimento em espiral** (para cobertura maior do ambiente)
- 🔄 Alternância automática entre padrões
- 💥 Detecção de colisões também durante o modo espiral
- 🧠 Lógica simples, eficiente e personalizável
- 📟 Feedback completo via porta Serial

---

## 🛠️ Componentes utilizados

| Componente        | Modelo sugerido         |
|-------------------|--------------------------|
| ESP8266           | NodeMCU / Wemos D1 Mini  |
| Ponte H           | Módulo L298N             |
| Motores DC        | 2x 5V/6V com rodinhas     |
| Botão de colisão  | Interruptor normalmente aberto (NO) |
| Fonte de energia  | Bateria 7.4V ou 5V USB Power Bank |
| Aspirador         | Motor adicional (opcional) |

---

## ⚙️ Ligações dos pinos

| Função              | Pino ESP8266 |
|---------------------|--------------|
| Motor A IN1         | D1           |
| Motor A IN2         | D2           |
| Motor B IN3         | D5           |
| Motor B IN4         | D6           |
| ENA (PWM Motor A)   | D3           |
| ENB (PWM Motor B)   | D4           |
| Botão de colisão    | D7 (com pull-up interno) |

---

## 🌐 Como usar

2. Conecte seu celular ou PC ao Wi-Fi `RoboAspira` (senha: `12345678`)
3. Acesse `http://192.168.4.1` pelo navegador
4. Use a interface para controlar o robô

---

## 🧠 Estrutura do código

- `setup()`:
  - Configura motores, Wi-Fi e servidor web
- `loop()`:
  - Alterna entre modos automáticos com base em tempo
  - Responde aos comandos do navegador
- `logicaBaterEVirar()`:
  - Anda para frente até bater
  - Ao colidir, recua e vira aleatoriamente
- `logicaEspiral()`:
  - Anda em espiral (um motor mais lento)
  - Também reage a colisões

---

## 📝 Melhorias futuras (ideias)

- Sensor ultrassônico em vez de botão
- Mapeamento de ambiente
- Controle por app (Blynk, MQTT, etc)
- Registro de percurso
- Integração com Alexa ou Google Assistant

---

## 📖 Licença

Este projeto é open-source sob a licença MIT.  
Sinta-se livre para usar, modificar e compartilhar!

---

## 🙌 Créditos

Desenvolvido por Fred Augusto (frederico.augusto@outlook.com).  
Inspirado em projetos DIY de robótica com ESP8266.

