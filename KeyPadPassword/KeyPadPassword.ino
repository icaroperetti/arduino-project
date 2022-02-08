//Inclusão de bibliotecas
#include <Password.h>  
#include <Keypad.h>    
#include <Servo.h> 
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
Servo servo;
bool isLocked = true;
Password password = Password("123");
Password password2 = Password("112233");
 
 
//Código detectado por meio de uma varredura procurando dispositivos I2c
LiquidCrystal_I2C lcd(0x27, 16, 2); //Inicialização do lcd
 
const byte linha = 4;   // Define número de linhas
const byte coluna = 4;  // Define número de colunas
 
// Relaciona linha e colunas para determinar os caracteres
char keys[linha][coluna] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
 
byte pinolinha[linha] = { 3, 4, 5, 6 };      // Declara os pinos das linha
byte pinocoluna[coluna] = { 8, 9, 10, 11 };  // Declara os pinos das coluna
 
//Inicializar o teclado.
Keypad keypad = Keypad(makeKeymap(keys), pinolinha, pinocoluna, linha, coluna);
 
void setup() {
  //Pino do servo
  pinMode(7, OUTPUT);
 
  servo.attach(7);
  servo.write(0);
 
  Serial.begin(9600);
 
  lcd.init();
  lcd.backlight();
  lcd.print("Digite sua senha:");

 
  keypad.addEventListener(getInput);
  keypad.setDebounceTime(5);  // Delay para leitura das teclas.
}
 
void loop() {
  keypad.getKey();
}
 
// Captura teclas pressionadas e aguarda confirmação para verificar a senha
void getInput(KeypadEvent eKey) {
  Serial.print("isLocked");
  Serial.println(isLocked);
 
  switch (keypad.getState()) {
    case PRESSED:
      delay(50);
      switch (eKey) {
        case 'C':
          checkPassword();
          break;
        case '#':  //Caso pressionado o simbolo #, simula como se fosse a tranca fechando para tornar o estado do cofre como trancado.
          lockSafe();
          break;
        default:
          password.append(eKey);
          password2.append(eKey);
          if ((eKey != '#' || eKey != 'C') && (isLocked == 1 )) {
            lcd.setCursor(0, 1);
            lcd.print(eKey);
          }
 
      }
  }
}
 
//Função para trancar o cofre
void lockSafe() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cofre trancado");
  isLocked = true;
  servo.write(0);
  delay(2000);
 
  lcd.setCursor(0, 0);
  lcd.print("Digite sua senha:");
 
  password.reset();
  password2.reset();
}
 
// Função que verifica se a senha está correta
void checkPassword() {
  lcd.clear();
  if (isLocked == true) {
    if (password.evaluate() || password2.evaluate()) {
      lcd.setCursor(0, 0);
      Serial.print("Senha Correta\n");
      lcd.print("Acesso Permitido!");
      servo.write(90);
      isLocked = false;
    } else {
      if (isLocked != false) {
        lcd.clear();
        Serial.println("Senha incorreta");
 
        lcd.print("Senha incorreta ");
        delay(2000);
 
        lcd.clear();
        lcd.print("Digite sua senha:");
        password.reset();
        password2.reset();
      }
    }
  }else{
    lcd.clear();
    lcd.print("Cofre ja aberto");
  }
}
