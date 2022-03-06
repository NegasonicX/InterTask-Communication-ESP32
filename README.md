# InterTask Communication using Multiple Queues
This code shows a basic logic to perform inter-task communication using multiple queues

## GPIO Functions:
* GPIO2 : output ( built-in led on Devkit-V1 )

## Understanding the Flow:
* This code is developed for ESP32 on Embedded C Language using FreeRTOS.
* FreeRTOS task don't have the provision to return values.
* That said, one can either use global variables to track changes in values but is not the most appropriate way.
* This is where "Queue" comes into picture, enabling inter-task communications.

## Queues in FreeRTOS:
* As the name suggests, Queues acts like a stacked list and mostly preferred in FIFO ( First In First Out ).
* There are various Queue operations available, but in this program I will cover the basic functions.
### Create a Queue-
* **xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );**
* **uxQueueLength** --> _The maximum number of items the queue can hold at any one time._
* **uxItemSize**    --> _The size, in bytes, required to hold each item in the queue._
* **Example**       --> _xQueue1 = xQueueCreate( 10, sizeof( unsigned long ) );_
* If created successfully, returns the handle or "NULL" otherwise.

### Delete a Queue-
* **vQueueDelete( QueueHandle_t xQueue );**
* Deletes the queue with the provided handle "xQueue".

### Send messages on Queue-
* **xQueueSend( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );**
* **xQueue** --> 	_The handle to the queue on which the item is to be posted._
* **pvItemToQueue** --> 	_A pointer to the item that is to be placed on the queue._
* **xTicksToWait** -->  	_The maximum amount of time the task should block waiting for space to become available on the queue, should it already be full._
* Returns "pdTRUE" if the item was successfully posted, otherwise "errQUEUE_FULL".

### Receive messages on Queue-
* **xQueueReceive( QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait );**
* **xQueue** --> 	_The handle to the queue on which the item is to be received._
* **pvBuffer** --> 	_Pointer to the buffer into which the received item will be copied._
* **xTicksToWait** -->  	_The maximum amount of time the task should block waiting for an item to receive should the queue be empty at the time of the call._
* Returns "pdTRUE" if an item was successfully received from the queue, otherwise "pdFALSE".
