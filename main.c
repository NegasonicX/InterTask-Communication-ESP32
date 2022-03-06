#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/**
 * Brief:
 * This code shows a basic logic to perform inter-task communication using multiple queues
 *
 * GPIO status:
 * GPIO2   : output ( built-in LED on Devkit V1 )
 *
 */

#define builtin 2                   //++ Define the built-in LED on Devkit V1

QueueHandle_t  q=NULL;              //++ Create a handle for Queue
QueueHandle_t  p=NULL;              //++ Create a handle for Queue

void led_task(void *pvParameter)    //++ Queue recieve task 
{
    unsigned long counter;
    bool flag = false;
    
    if(q == NULL){
        printf("Counter Queue is not ready \n");
        return;
    }
    if(p == NULL){
        printf("Flag Queue is not ready \n");
        return;
    }
    
    while(1){
        xQueueReceive(q,&counter,(TickType_t )(250/portTICK_PERIOD_MS));    //++ Receive the queue counter value
        xQueueReceive(p,&flag,(TickType_t )(250/portTICK_PERIOD_MS));       //++ Receive the queue flag status
        
        if(counter%10 == 0){                                                //++ For every count number divisible by 10, enter the condition
            printf("value received on queue: %lu \n",counter);
            gpio_set_level(builtin,1);                                      //++ Turn on built-in LED
            vTaskDelay(500/portTICK_PERIOD_MS);
        }
        
        if(flag==true)
        {
            printf("value received on queue was multiple of 5\n");
            flag = false;                                                   //++ Reset the flag for next queue
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
        gpio_set_level(builtin,0);                                          //++ Turn off built-in LED
    }
}

void count_task(void *pvParameter){                                         //++ Queue sending task
    
    unsigned long counter=1;
    bool flag = false;

    if(q == NULL){
        printf("Counter Queue is not ready \n");
        return;
    }
    if(p == NULL){
        printf("Flag Queue is not ready \n");
        return;
    }

    while(1){
         printf("value sent on queue: %lu \n",counter);
         xQueueSend(q,(void *)&counter,(TickType_t )0);                     //++ Add counter value on the Queue
         counter++;
         
         if(counter%5 == 0)
         {
             flag = true;
             xQueueSend(p,(void *)&flag,(TickType_t )0);                    //++ Add Flag status on the Queue
         }
         vTaskDelay(1000/portTICK_PERIOD_MS); 
    }
}

void app_main()
{
    gpio_reset_pin(builtin);                            
    gpio_set_direction(builtin, GPIO_MODE_OUTPUT);                          //++ Set built-in LED direction

    q=xQueueCreate(20,sizeof(unsigned long));                               //++ Create first queue
    p=xQueueCreate(5,sizeof(bool));                                         //++ Create second queue
    
    if(q != NULL && p != NULL){
        printf("Queue is created\n");
        vTaskDelay(1000/portTICK_PERIOD_MS); 
        xTaskCreate(&led_task,"led_task",2048,NULL,5,NULL);
        printf("led task  started\n");
        xTaskCreate(&count_task,"count_task",2048,NULL,5,NULL);
        printf("counter task  started\n");
    }else{
        printf("Queue creation failed");
    }    
}
