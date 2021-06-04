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

 
//Tasks
TaskHandle_t lineHandle;
TaskHandle_t stopHandle;
TaskHandle_t driverHandle; 

int ledPin = 13;
 
void setup() {
 //Inicializa Serial
 Serial.begin(9600);
 pinMode(ledPin, OUTPUT);
 
 
 //Cria tarefa lineAGVTask
xTaskCreate(lineAGVTask, //Funcao
 "Task1", //Nome
 100, //Pilha
 NULL, //Parametro
 1, //Prioridade
 &lineHandle);
 
 //Cria tarefa driverAGVTask
 xTaskCreate(driverAGVTask,
 "Task2",
 100,
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
 

 
 
void lineAGVTask (void *param) {
 
 (void) param;
 
 while(1){
    Serial.println("Ligado");
    delay(3000);
    Serial.println("Desligado");
    delay(3000);
 
 }
}
 
 
void driverAGVTask (void *param) {
 
 (void) param;
 while(1){
  digitalWrite(ledPin, HIGH);
  delay(3000);
  digitalWrite(ledPin, LOW);
  delay(3000);
 }
 
}
