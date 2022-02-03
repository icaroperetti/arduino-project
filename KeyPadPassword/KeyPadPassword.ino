#include <Password.h>  // Inclui biblioteca Password.h
#include <Keypad.h>    // Inclui biblioteca Keypa.h
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo servo;
bool isLocked;
Password senha = Password("123");  // Senha para liberação de acesso
Password password2 = Password("20123345");

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte linha = 4;   // Define número de linhas
const byte coluna = 4;  // Define número de colunas

// Relaciona linha e colunas para determinação dos caracteres
char keys[linha][coluna] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte pinolinha[linha] = { 3, 4, 5, 6 };      // Declara os pinos de interpretação das linha
byte pinocoluna[coluna] = { 8, 9, 10, 11 };  // Declara os pinos de interpretação das coluna

//INICIALIZAÇÃO DO TECLADO
Keypad keypad = Keypad(makeKeymap(keys), pinolinha, pinocoluna, linha, coluna);

void setup() {
  pinMode(7, OUTPUT);

  servo.attach(7);
  servo.write(0);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.print("Icaro Peretti");
  delay(3000);
  lcd.clear();

  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(5);  // Tempo de atraso para leitura das teclas.
}

void loop() {
  keypad.getKey();
}

// Captura teclas pressionadas e aguarda confirmação para verificar
void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
      Serial.print("Digitado: ");
      Serial.println(eKey);
      delay(50);
      switch (eKey) {
        case 'C':
          verificasenha();
          break;
        case '#':
          lockSafe();
          break;
        default:
          senha.append(eKey);
          password2.append(eKey);
      }
      if (eKey != '#') {
        lcd.print(eKey);
      }
  }
}

void lockSafe() {
  lcd.setCursor(0, 0);
  lcd.print("Cofre trancado");
  isLocked = true;
  servo.write(0);
  delay(2000);
  lcd.clear();
  senha.reset();
  password2.reset();
}

// Função que verifica se a senha está correta
void verificasenha() {
  Serial.println("Verificando, aguarde...");
  if (senha.evaluate() || password2.evaluate()) {
    lcd.setCursor(0, 0);
    Serial.print("Senha Correta\n");
    lcd.print("Acesso Permitido!");
    isLocked = false;
    servo.write(90);
  } else {
    if (isLocked != false) {
      Serial.println("Senha incorreta");
      lcd.setCursor(0, 0);
      lcd.print("Senha incorreta ");
      senha.reset();
      password2.reset();
    }
  }
}