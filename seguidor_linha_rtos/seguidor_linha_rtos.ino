#include <Arduino_FreeRTOS.h>
#include <croutine.h>
#include <event_groups.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOSVariant.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <portmacro.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <StackMacros.h>
#include <task.h>
#include <timers.h>
 
// Declaraçôes para os enviar para centro de controle
 bool controller_moveForward = LOW;
 bool controller_moveBack = LOW;
 bool controller_turnRight = LOW;
 bool controller_turnLeft = LOW;
 bool myboll = true;
 
//Declarações para sensor seguidor de linha
const int linha_d1 = A1; //Sensor fora da linha
const int linha_d2 = 2; //Sensor fora da linha
const int linha_d3 = 3; //*____ Sensor dentro da linha ____*
const int linha_d4 = 4; // Senso dentro da linha
const int linha_d5 = 5; // Sensor dentro da linha
const int linha_d6 = 6; //*____ Sensor dentro da linha ____*
const int linha_d7 = 7; //Sensor fora da linha
const int linha_d8 = A9; //Sensor fora da linha
 
String direcao = "u";
 
const int range_sensor = 980;
 
//-------------------------------------------------- \\
// Declarações para drivers
const int steppin1 = 7;
const int steppin2 = 9;
const int dirpin1 = 13;
const int dirpin2 = 11;
// ------------------------------------------------------ \\
 
//Variável para o controle do loop
int x = 0;
 
//Tasks
TaskHandle_t lineHandle;
TaskHandle_t stopHandle;
TaskHandle_t driverHandle; 
 
void setup() {
 //Inicializa Serial
 Serial.begin(9600);
 
 //Declarando as entradas dos sensores seguidor de linhas
 pinMode(linha_d1, INPUT);
 pinMode(linha_d2, INPUT);
 pinMode(linha_d3, INPUT);
 pinMode(linha_d4, INPUT);
 pinMode(linha_d5, INPUT);
 pinMode(linha_d6, INPUT);
 pinMode(linha_d7, INPUT);
 pinMode(linha_d8, INPUT);
 
 //Declaração de IO, pulso dos dois motores, e a direção dos dois motores
 pinMode(steppin1, OUTPUT);
 pinMode(steppin2, OUTPUT);
 pinMode(dirpin1, OUTPUT);
 pinMode(dirpin2, OUTPUT);
 
 //Cria tarefa lineAGVTask
xTaskCreate(lineAGVTask, //Funcao
 "Task1", //Nome
 128, //Pilha
 NULL, //Parametro
 1, //Prioridade
 &lineHandle);
 
 //Cria tarefa driverAGVTask
 xTaskCreate(driverAGVTask,
 "Task2",
 128,
 NULL,
 1,
 &driverHandle); 
 
 /* //Cria tarefa stopAGVTask
 xTaskCreate(stopAGVTask,
 "Task3",
 100,
 NULL,
 1,
 &stopHandle); 
 */ 
 
}
 
void loop() {
 // Nada é feito aqui, Todas as funções são feitas em Tasks
}
 
//FUNÇÃO PARA ANDAR PARA A FRENTE
void moveForward()
{
 //Setando a direção de rotação de cada motor
 digitalWrite(dirpin1, LOW);
 digitalWrite(dirpin2, HIGH);
 
 //Mandando os pulsos para o driver mover os motores
 for(x = 0; x < 100; x++) {
 digitalWrite(steppin1, HIGH);
 digitalWrite(steppin2, HIGH);
 delayMicroseconds(1700);
 digitalWrite(steppin1, LOW);
 digitalWrite(steppin2, LOW);
 delayMicroseconds(2000);
 } 
}
 
//FUNÇÃO PARA ANDAR PARA A TRÀS
void moveBack()
{
 //Setando a direção de rotação de cada motor
 digitalWrite(dirpin1, HIGH);
 digitalWrite(dirpin2, LOW);
 
 //Mandando os pulsos para o driver mover os motores
 for(x = 0; x < 100; x++) {
 digitalWrite(steppin1, HIGH);
 digitalWrite(steppin2, HIGH);
 delayMicroseconds(1700);
 digitalWrite(steppin1, LOW);
 digitalWrite(steppin2, LOW);
 delayMicroseconds(1700);
 }
}
 
//FUNÇÃO PARA VIRAR A DIREITA
void turnRight()
{
 //Setando a direção de rotação de cada motor
 digitalWrite(dirpin1, LOW);
 digitalWrite(dirpin2, LOW);
 delay(1);
 
 //Mandando os pulsos para o driver mover os motores
 for(x = 0; x < 10; x++) {
 digitalWrite(steppin1, HIGH);
 digitalWrite(steppin2, HIGH);
 delayMicroseconds(1700);
 digitalWrite(steppin1, LOW);
 digitalWrite(steppin2, LOW);
 delayMicroseconds(1700);
 } 
}
 
//FUNÇÃO PARA VIRAR A ESQUERDA
void turnLeft()
{
 //Setando a direção de rotação de cada motor
 digitalWrite(dirpin1, HIGH);
 digitalWrite(dirpin2, HIGH);
 delay(1);
 
 //Mandando os pulsos para o driver mover os motores
 for(x = 0; x < 10; x++) {
 digitalWrite(steppin1, HIGH);
 digitalWrite(steppin2, HIGH);
 delayMicroseconds(1700);
 digitalWrite(steppin1, LOW);
 digitalWrite(steppin2, LOW);
 delayMicroseconds(1700);
 }
}
 
 
void lineAGVTask (void *param) {
 
 (void) param;
 
 while(1){
 // Se todos os sensores centrais, conseguirem detectar a linha, e a variavel de controle false, então mover o robo para frente
 if ( analogRead(linha_d1) > range_sensor && analogRead(linha_d8) > range_sensor )
 {
 //Manda para os controladores para qualquer outro comando nos drivers dos motores
 //----
 controller_moveBack = LOW ;
 controller_turnRight = LOW ;
 controller_turnLeft = LOW ;
 //---
 controller_moveForward = HIGH; //Mover para frente
 direcao = "f";
 }
 
 // Se um dos sensores da esquerda indentificar a linha, então firar para a direita
 if ( analogRead(linha_d8) < range_sensor )
 {
 //Manda para os controladores para qualquer outro comando nos drivers dos motores
 //----
 controller_moveBack = LOW ;
 controller_moveForward = LOW ;
 controller_turnRight =  LOW ;
 //---
 controller_turnLeft = HIGH; //Virar para Esquerda
 direcao = "e";
 }
 
 if ( analogRead(linha_d1) < range_sensor )
 {
 //Manda para os controladores para qualquer outro comando nos drivers dos motores
 //----
 controller_moveBack = LOW ;
 controller_moveForward = LOW ;
 controller_turnLeft =  LOW ;
 //---
 controller_turnRight = HIGH; //Virar para Direita
 direcao = "d";
 }
 
  Serial.println("Lendo informações dos sensores");
  Serial.print("S1 - ");
  Serial.print(analogRead(linha_d1));
  Serial.print(" ");
  Serial.print("S8 - ");
  Serial.print(analogRead(linha_d8));
  Serial.println("");
  Serial.println("-----------------------------------");
  Serial.print(" direção = ");
  Serial.print(direcao);
  Serial.println("");
  Serial.println("");
  Serial.println("");
 
 }
}
 
 
void driverAGVTask (void *param) {
 
 (void) param;
 
 while(1){
 
 //Fazendo com que o robo ande, dependendo do controlador que estiver ativado
 while(true)
 {
  //Setando a direção de rotação de cada motor
 digitalWrite(dirpin1, LOW);
 digitalWrite(dirpin2, HIGH);
 
 //Mandando os pulsos para o driver mover os motores
 for(x = 0; x < 100; x++) {
 digitalWrite(steppin1, HIGH);
 digitalWrite(steppin2, HIGH);
 delayMicroseconds(1000);
 digitalWrite(steppin1, LOW);
 digitalWrite(steppin2, LOW);
 delayMicroseconds(1000);
 } 
 Serial.print("FRENTE");
 }
 
 while( controller_moveBack == HIGH )
 {
 moveBack();
 Serial.print("TRAS");
 }
 
 if (controller_turnRight == HIGH)
 {
 turnRight();
 delay(500);
 Serial.print("DIREITA");
 }
 
 if (controller_turnLeft == HIGH)
 {
 turnLeft();
 delay(500);
 Serial.print("ESQUERDA");
 } 
 
 Serial.println("Drivers funcionando!");
 
 }
}
