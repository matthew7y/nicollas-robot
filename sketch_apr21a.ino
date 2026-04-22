/*
  Soccer Robot - Nicollas
  Robô controlado por comunicação via Bluetooth:
  - Arduino UNO R3
  - L293D Motor Shield
  - HC-05 Bluetooth Module

  Funcionalides:
  - Movimentos básicos (Frente, Ré, Esquerda e Direita)
  - Comando para reduzir voltagem dos motores (GND)
  - Controlado via comandos recebidos pelo terminal Bluetooth

  Autores:
  - Carlos, Ricardo, Ludwig, Jhonathan e Matheus
*/

// Motor A (1)
#define PIN_MOTOR_A1 5 // Sentido Horário
#define PIN_MOTOR_A2 6 // Sentido Antihorário

// Motor B (2)
#define PIN_MOTOR_B1 9 // Sentido Horário
#define PIN_MOTOR_B2 10 // Sentido Antihorário

// Velocidade inicial para pinos PWM - Controle de velocidade
int initialSpeed = 130;  // Não altere isto durante a primeira versão do Nicollas

void setup() {
  Serial.begin(9600);
  pinMode(PIN_MOTOR_A1, OUTPUT);
  pinMode(PIN_MOTOR_A2, OUTPUT);

  pinMode(PIN_MOTOR_B1, OUTPUT);
  pinMode(PIN_MOTOR_B2, OUTPUT);
}

// Quando paramos os motores do Nicollas não é necessário PWM
// Então ignoramos o parâmetro `mode`, futuramente, iremos implementar isto de forma interativa
void stopSoccerRobot(int mode) {
  analogWrite(PIN_MOTOR_A1, 0);
  analogWrite(PIN_MOTOR_A2, 0);
  analogWrite(PIN_MOTOR_B1, 0);
  analogWrite(PIN_MOTOR_B2, 0);
}

void moveForward(int mode) {
  if (mode == 1) {
    digitalWrite(PIN_MOTOR_A2, LOW);
    digitalWrite(PIN_MOTOR_B2, LOW);
    digitalWrite(PIN_MOTOR_A1, HIGH);
    digitalWrite(PIN_MOTOR_B1, HIGH);
  } else if (mode == 0) {
    // Modo aceleração
    analogWrite(PIN_MOTOR_A2, 0);
    analogWrite(PIN_MOTOR_B2, 0);

    for (int speed = initialSpeed; speed <= 255; speed += 5) {
      analogWrite(PIN_MOTOR_A1, speed);
      analogWrite(PIN_MOTOR_B1, speed);
      delay(20);
    }
  }
}

void moveBackward(int mode) {
  if (mode == 1) {
    // Modo turbo
    digitalWrite(PIN_MOTOR_A1, LOW);
    digitalWrite(PIN_MOTOR_B1, LOW);
    digitalWrite(PIN_MOTOR_A2, HIGH);
    digitalWrite(PIN_MOTOR_B2, HIGH);
  } else if (mode == 0) {
    // Modo aceleração
    analogWrite(PIN_MOTOR_A1, 0);
    analogWrite(PIN_MOTOR_B1, 0);

    for (int speed = initialSpeed; speed <= 255; speed += 5) {
      analogWrite(PIN_MOTOR_A2, speed);
      analogWrite(PIN_MOTOR_B2, speed);
      delay(20);
    }
  }
}

// Mesmo para a função `stopSoccerRobot` - Ignoramos o parâmetro `mode`
// Implementaremos isso na versão final do projeto
void moveLeft(int mode) {
  analogWrite(PIN_MOTOR_A1, 255);
  analogWrite(PIN_MOTOR_A2, 0);
  analogWrite(PIN_MOTOR_B1, 120);
  analogWrite(PIN_MOTOR_B2, 0);
}

// Mesmo para a função `stopSoccerRobot` - Ignoramos o parâmetro `mode`
// Implementaremos isso na versão final do projeto
void moveRight(int mode) {
  analogWrite(PIN_MOTOR_A1, 120);
  analogWrite(PIN_MOTOR_A2, 0);
  analogWrite(PIN_MOTOR_B1, 255);
  analogWrite(PIN_MOTOR_B2, 0);
}

void loop() {
  if (Serial.available()) {
    // F:1, F:0
    String data = Serial.readStringUntil('\n');
    data.trim();  // Remover espaços extras da string recebida

    int index = data.indexOf(':');

    if (index != -1) {
      char direction = data.charAt(0); // Pegamos o primeiro caractere na posição 0 da string recebida
      int mode = data.substring(index + 1).toInt();  // Modo 0: Padrão (Aceleração Progressiva) - Modo 1: Turbo

      switch (direction) {
        case 'S': stopSoccerRobot(mode); break;  // S - Parar (Stop)
        case 'F': moveForward(mode); break;      // F -> Frente (Forward)
        case 'B': moveBackward(mode); break;     // B -> Ré (Backward)
        case 'L': moveLeft(mode); break;         // L -> Esquerda (Left)
        case 'R': moveRight(mode); break;        // R -> Direita (Right)
        default: stopSoccerRobot(mode); break;       // Safe fallback - Para os motores do Nicollas
      }
    }
  }
}
