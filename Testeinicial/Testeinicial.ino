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

int count = 0;
int count_hora = 0;
 
//Tasks
TaskHandle_t lineHandle;
TaskHandle_t stopHandle;
TaskHandle_t driverHandle; 
 
void setup() {
 //Inicializa Serial
 Serial.begin(9600);
 
 
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
    Serial.println("Tesk1");
    delay(1000);
    Serial.print(count_hora);
    Serial.print(" : ");
    Serial.print(count);
    Serial.println("");
 
 }
}
 
 
void driverAGVTask (void *param) {
 
 (void) param;
 
 while(1){
      count++;

      if (count > 10)
      {
        count_hora++;
        count = 0;
      }
      
     Serial.println("Tesk2");
     delay(1000);
 }
 
}
