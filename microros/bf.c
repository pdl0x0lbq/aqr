/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "queue.h"
#include "semphr.h"
#include "crc.h"
#include "std_msgs/msg/detail/int16__struct.h"
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
/*#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif
extern const int USED uxTopUsedPriority = configMAX_PRIORITIES - 1;*/
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
union SerialData {
    struct {
        uint8_t data0;
        uint8_t data1;
        uint8_t data2;
        uint8_t data3;  // 第4位，速度高8位
        uint8_t speedHighByte;   // 第5位，速度低8位
        uint8_t speedLowByte;
        uint8_t data6;
        uint8_t data7;
        uint8_t data8;
        uint8_t data9;
    };
    uint8_t raw[10];  // 以字节数组形式访问整个数据
};
volatile union SerialData serialData;
DMA_HandleTypeDef hdma_usart4_rx;
QueueHandle_t  DataQueue;
SemaphoreHandle_t re;
QueueHandle_t  speed_DataQueue;
int16_t ls[1];
bool DataRev(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
union DDSM115 ddsm115;
volatile union DDSM115 re_ddsm115;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern uint8_t ddsm115_dma_buffer[10];
extern TaskHandle_t ddsm115_xTaskHandle;
const osThreadAttr_t ddsm115_attributes={
        .name = "ddsm115Task",
        .stack_size = 3000 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .stack_size = 3000 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
        .name = "myTask02",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);
void RS485(void *argument){

    ddsm115.wheelspeed=0;
    int8_t Data[] = {0x01, 0x64, ddsm115.highByte, ddsm115.lowByte, 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t data_size=sizeof(Data) / sizeof(Data[0]);
    uint8_t crc_result = crc8(Data, data_size);
    int8_t sendData[] = {0x01, 0x64, ddsm115.highByte, ddsm115.lowByte,0x00, 0x00, 0x00, 0x00, 0x00, crc_result};
    while (1){
        // xSemaphoreGive(re);
        xQueueReset(DataQueue);
        HAL_UART_Transmit(&huart4, sendData, sizeof(sendData), HAL_MAX_DELAY);
        DataRev();
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
        osDelay(100);
        //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
        // HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
        // HAL_UART_Transmit(&huart4, sendData, sizeof(sendData), HAL_MAX_DELAY);
    }
}
void vTask1(void *pvParameters) {
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);

    while (1) {
        // 等待任务通知
        // HAL_UART_Transmit(&huart4, ddsm115_dma_buffer, sizeof(ddsm115_dma_buffer), HAL_MAX_DELAY);

        //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);

        //int8_t sendData[] = {0x01, 0x64, serialData.speedHighByte, serialData.speedLowByte,0x00, 0x00, 0x00, 0x00, 0x00};
        // HAL_UART_Transmit(&huart4, ddsm115_dma_buffer, sizeof(ddsm115_dma_buffer), HAL_MAX_DELAY);
        // 执行你的任务代码
        // 处理串口接收到的数据等
        osDelay(10);
    }
}
void UART4_IRQHandler(void)
{

    uint32_t tmp_flag ;
    int len;
    int data[10];
    tmp_flag  = __HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE);

    if( tmp_flag  != RESET)
    {

        __HAL_UART_CLEAR_IDLEFLAG(&huart4);//清除标志位
        HAL_UART_DMAStop(&huart4); //停止DMA接收，防止数据出错

//        len = 10 - __HAL_DMA_GET_COUNTER(&hdma_usart4_rx);// 获取DMA中传输的数据个数
//
//         以下为用户数据处理，将数据拷贝出去
//        if(len == 10)
//        {
//            memcpy(data,USART4_RX_BUF,len);
//            //update_sbus(data);
//            //xQueueSendFromISR(DataQueue,data,0);
//        }
        HAL_UART_Receive_DMA(&huart4,USART4_RX_BUF,1);   //打开DMA接收，数据存入rx_buffer数组中。
        xQueueSendFromISR(DataQueue,USART4_RX_BUF,0);
        //HAL_UART_Transmit(&huart4, USART4_RX_BUF, sizeof(USART4_RX_BUF), HAL_MAX_DELAY);
    }

    HAL_UART_IRQHandler(&huart4); //调用HAL库中断处理公用函数
}
bool DataRev(void)
{
    /* Infinite loop */
    uint8_t buff[1];
    uint8_t string_buff[9];
    int16_t  speed[1];
    uint8_t re_crc=0;
    uint8_t frist_flag=0;
    uint8_t cnt=0;
    std_msgs__msg__Int32 lsspe;
    uint8_t re_crc_result=0;
    uint32_t timer =xTaskGetTickCount();
    while (1){
        if(pdPASS== xQueueReceive(DataQueue,buff,0)){
            //HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);

            if(cnt==9){
                HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
                re_crc=buff[0];
                size_t re_data_size=sizeof(string_buff) / sizeof(string_buff[0]);
                re_crc_result = crc8(string_buff, re_data_size);
                if (re_crc_result==re_crc){
                    re_ddsm115.highByte=string_buff[4];
                    re_ddsm115.lowByte=string_buff[5];
                    lsspe.data=timer;
                    //HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);
                    //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
                    xQueueSend(speed_DataQueue,&lsspe,0);
                    return true;

                } else{
                    return false;
                }
            } else{
                string_buff[cnt]=buff[0];
            }
            cnt++;
            //HAL_UART_Transmit(&huart4, buff, sizeof(buff), HAL_MAX_DELAY);

            if(cnt==10){
//                if (re_crc_result==re_crc){
//                    HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);
//                }

                cnt=0;

            }

        } else{
            if (100 < (unsigned long)(xTaskGetTickCount() - timer)){
                //lsspe.data=250;
                //HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);
                //xQueueSend(speed_DataQueue,&lsspe,0);
                //cnt=0;


                return false;

            }


        }
        //  HAL_UART_Transmit(&huart4, buff, sizeof(buff), HAL_MAX_DELAY);


        //osDelay(1);

    }

}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void DataRecv_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    DataQueue= xQueueCreate(10,1);
    speed_DataQueue= xQueueCreate(10,sizeof(std_msgs__msg__Int32));
    re=xSemaphoreCreateBinary();
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of myTask02 */
    //myTask02Handle = osThreadNew(DataRecv_task, NULL, &myTask02_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    xTaskCreate(RS485,"rs485",128*4,NULL,osPriorityNormal,NULL);
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    // micro-ROS configuration


    rmw_uros_set_custom_transport(
            true,
            (void *) &huart2,
            cubemx_transport_open,
            cubemx_transport_close,
            cubemx_transport_write,
            cubemx_transport_read);

    rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
    freeRTOS_allocator.allocate = microros_allocate;
    freeRTOS_allocator.deallocate = microros_deallocate;
    freeRTOS_allocator.reallocate = microros_reallocate;
    freeRTOS_allocator.zero_allocate =  microros_zero_allocate;

    if (!rcutils_set_default_allocator(&freeRTOS_allocator)) {
        printf("Error on default allocators (line %d)\n", __LINE__);
    }

    // micro-ROS app

    rcl_publisher_t publisher;
    std_msgs__msg__Int32 msg;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    int32_t sspd;
    allocator = rcl_get_default_allocator();

    //create init_options
    rclc_support_init(&support, 0, NULL, &allocator);


    // create node
    rclc_node_init_default(&node, "cubemx_node", "", &support);

    // create publisher
    rclc_publisher_init_default(
            &publisher,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
            "cubemx_publisher");

    msg.data = 0;

    for(;;)
    {
        if(pdPASS == xQueueReceive(speed_DataQueue, &msg, 0)){

        }
        rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
        if (ret != RCL_RET_OK)
        {
            printf("Error publishing (line %d)\n", __LINE__);
        }



        //msg.data++;
        osDelay(10);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_DataRecv_task */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataRecv_task */
void DataRecv_task(void *argument)
{
    /* USER CODE BEGIN DataRecv_task */
    /* Infinite loop */
    uint32_t timer = 0;
    uint8_t buff[1];
    uint8_t string_buff[9];
    int16_t  speed[1];
    uint8_t re_crc=0;
    uint8_t frist_flag=0;
    uint8_t cnt=0;
    std_msgs__msg__Int32 lsspe;
    uint8_t re_crc_result=0;
    while (1){
        if(pdPASS== xQueueReceive(DataQueue,buff,0)){
            //timer = xTaskGetTickCount();
            if(cnt==0){
                HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
                re_crc=buff[0];
                size_t re_data_size=sizeof(string_buff) / sizeof(string_buff[0]);
                uint8_t re_crc_result = crc8(string_buff, re_data_size);
//                HAL_UART_Transmit(&huart4, buff, sizeof(buff), HAL_MAX_DELAY);
//                buff[0]=re_crc_result;
//                HAL_UART_Transmit(&huart4, buff, sizeof(buff), HAL_MAX_DELAY);
                if (re_crc_result!=re_crc){
                    // HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
                    //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
                    re_ddsm115.highByte=string_buff[4];
                    re_ddsm115.lowByte=string_buff[5];
                    lsspe.data=re_ddsm115.wheelspeed;
                    //HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);
                    xQueueSend(speed_DataQueue,&lsspe,0);
                    if(speed[0]==1543){
                        //HAL_UART_Transmit(&huart4, (uint8_t*)speed, sizeof(speed), HAL_MAX_DELAY);

                    } else if(speed[0]==1798){
                        // HAL_UART_Transmit(&huart4, (uint8_t*)speed, sizeof(speed), HAL_MAX_DELAY);

                    }

                    //HAL_UART_Transmit(&huart4, (uint8_t*)speed, sizeof(speed), HAL_MAX_DELAY);

                }
            } else{
                string_buff[cnt]=buff[0];
            }
            cnt++;
            //HAL_UART_Transmit(&huart4, buff, sizeof(buff), HAL_MAX_DELAY);

            if(cnt==10){
//                if (re_crc_result==re_crc){
//                    HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);
//                }

                cnt=0;
            }

        } else{
            if (1 < (unsigned long)(xTaskGetTickCount() - timer)){
                //lsspe.data=250;
                //HAL_UART_Transmit(&huart4, string_buff, sizeof(string_buff), HAL_MAX_DELAY);
                //xQueueSend(speed_DataQueue,&lsspe,0);
                //cnt=0;
                HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);

            }


        }
        //  HAL_UART_Transmit(&huart4, buff, sizeof(buff), HAL_MAX_DELAY);


        osDelay(1);

    }
    for(;;)
    {        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
        osDelay(100);
    }
    /* USER CODE END DataRecv_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

