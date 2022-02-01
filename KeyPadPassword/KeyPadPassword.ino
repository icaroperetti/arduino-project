#include <Password.h> // Inclui biblioteca Password.h
#include <Keypad.h>   // Inclui biblioteca Keypa.h
#include <Servo.h>

Servo servo;
Password senha = Password("123"); // Senha para liberação de acesso
Password password2 = Password("20123345");

const byte linha = 4;  // Define número de linhas
const byte coluna = 4; // Define número de colunas

// Relaciona linha e colunas para determinação dos caracteres
char keys[linha][coluna] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pinolinha[linha] = {3, 4, 5, 6};     // Declara os pinos de interpretação das linha
byte pinocoluna[coluna] = {8, 9, 10, 11}; // Declara os pinos de interpretação das coluna

//INICIALIZAÇÃO DO TECLADO
Keypad keypad = Keypad(makeKeymap(keys), pinolinha, pinocoluna, linha, coluna);

void setup()
{
  pinMode(7, OUTPUT);
  servo.attach(7);
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(5); // Tempo de atraso para leitura das teclas.
  servo.write(0);
}

void loop()
{
  keypad.getKey();
}

// Captura teclas pressionadas e aguarda confirmação para verificar
void keypadEvent(KeypadEvent eKey)
{
  switch (keypad.getState())
  {
  case PRESSED:
    Serial.print("Digitado: ");
    Serial.println(eKey);
    delay(50);
    switch (eKey)
    {
    case 'C':
      verificasenha();
      break;
    case '#':
      Serial.flush();
      break;
    default:
      senha.append(eKey);
      password2.append(eKey);
    }
  }
}

// Função que verifica se a senha está correta
void verificasenha()
{
  Serial.println("Verificando, aguarde...");
  servo.write(0);
  if (senha.evaluate() || password2.evaluate())
  {
    Serial.print("Senha Correta\n");
    servo.write(82);
  }
  else
  {
    Serial.println("Senha incorreta");
    servo.write(0);
    senha.reset();
    password2.reset();
  }
}
