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
#include "ddsm115_c_api.h"
#include "usart.h"
#include "usbd_cdc_if.h"
#include "usbd_cdc.h"
#include "queue.h"
#include "semphr.h"
#include "crc.h"
#include "std_msgs/msg/detail/int16__struct.h"
#include "std_msgs/msg/detail/int16_multi_array__struct.h"
#include "std_msgs/msg/detail/int16_multi_array__type_support.h"
#include "sensor_msgs/msg/detail/joint_state__struct.h"
#include "sensor_msgs/msg/joint_state.h"
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
#include "armquadrover_msgs//msg/ddsm115.h"
#include "armquadrover_msgs/msg/ddsm115_servo.h"
#include "arm_servo_c_api.h"
#include "stm32f4xx_it.h"
#include "servo_c_api.h"
#include "Emm_V5.h"
#include "fifo.h"

/*#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif
extern const int USED uxTopUsedPriority = configMAX_PRIORITIES - 1;*/
void USART1_CALL(void);
void USART3_CALL(void);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//DMA_HandleTypeDef hdma_usart1_rx;
__IO bool rxFrameFlag = false;
extern USBD_HandleTypeDef hUsbDeviceFS;
QueueHandle_t  R485_DataQueue;
QueueHandle_t  Arm_Servo_DataQueue;
QueueHandle_t  stepping_DataQueue;
QueueHandle_t  DDSM115_Servo_DataQueue;
rcl_publisher_t publisher;
rcl_publisher_t  ArmStatesPubisher;
SemaphoreHandle_t re;
QueueHandle_t  speed_DataQueue;
int16_t ls[1];
uint8_t lsdata[10];
uint8_t rx_buffer[10];

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
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 3000 * 4,
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

void USART1_CALL(void)
{
    uint32_t tmp_flag ;
    int len;
    uint8_t data[10];
    tmp_flag  = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE);
    if( tmp_flag  != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_DMAStop(&huart1);
       len = 10 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        if(len == 10)
        {
           memcpy(data,USART1_RX_BUF,len);
           xQueueSendFromISR(R485_DataQueue, data, 0);
       }
        HAL_UART_Receive_DMA(&huart1,USART1_RX_BUF,10);
    }
}
void USART3_CALL(void)
{
    uint32_t tmp_flag ;
    int len;
    uint8_t data[8];
    tmp_flag  = __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE);
    if( tmp_flag  != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);
        len = 8 - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
        if(len == 8)
        {
            memcpy(data,USART3_RX_BUF,len);
            xQueueSendFromISR(DDSM115_Servo_DataQueue, data, 0);
        }
        HAL_UART_Receive_DMA(&huart3,USART3_RX_BUF,8);
    }
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

 /*   uint8_t uart6_len=0;
    uint8_t uart6_data[10];
    uint8_t uart3_len=0;
    uint8_t uart3_data[10];
    uint8_t uart4_len=0;
    uint8_t uart4_data[10];

    if(huart->Instance == USART3)
    {

        while (USART3_RX_BUF[uart3_len] != '\0')	uart3_len++;
        memcpy(uart3_data,USART3_RX_BUF,uart3_len);
        //HAL_UART_Transmit(&huart3,uart3_data,uart3_len,0xff);
        //xQueueSendFromISR(DDSM115_Servo_DataQueue, uart3_data, 0);
        xQueueOverwriteFromISR(DDSM115_Servo_DataQueue, uart3_data, 0);



        memset(USART3_RX_BUF, 0, 10);
        memset(uart3_data, 0, sizeof(uart3_data));

        HAL_UARTEx_ReceiveToIdle_IT(&huart3,USART3_RX_BUF,10);
    } else if (huart->Instance == USART6){

        while (USART6_RX_BUF[uart6_len] != '\0')	uart6_len++;
        memcpy(uart6_data,USART6_RX_BUF,uart6_len);

        xQueueOverwriteFromISR(stepping_DataQueue, uart6_data, 0);

        memset(USART6_RX_BUF, 0, 10);
        memset(uart6_data, 0, sizeof(uart6_data));
        rxFrameFlag = true;
        HAL_UARTEx_ReceiveToIdle_IT( &huart6,USART6_RX_BUF,10);
    }
    else if (huart->Instance == UART4){

        while (UART4_RX_BUF[uart4_len] != '\0')	uart4_len++;
        memcpy(uart4_data,UART4_RX_BUF,uart4_len);

        xQueueOverwriteFromISR(Arm_Servo_DataQueue, uart4_data, 0);

        memset(UART4_RX_BUF, 0, 10);
        memset(uart4_data, 0, sizeof(uart4_data));
        rxFrameFlag = true;
        HAL_UARTEx_ReceiveToIdle_IT( &huart4,UART4_RX_BUF,10);
    }*/
}

void rs485_subscription_callback(const void * msgin);
void stepping_subscription_callback(const void * msgin);
void rs485pub_timer_callback(rcl_timer_t * timer, int64_t last_call_time);
std_msgs__msg__Int16MultiArray arm_servo_states_data;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
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
    R485_DataQueue= xQueueCreate(10, sizeof(lsdata));
    //ARM1_DataQueue= xQueueCreate(10, sizeof(rx_buffer));
    DDSM115_Servo_DataQueue= xQueueCreate(10, sizeof(rx_buffer));
   stepping_DataQueue= xQueueCreate(1, sizeof(rx_buffer));
    Arm_Servo_DataQueue=xQueueCreate(10, sizeof(rx_buffer));
    speed_DataQueue= xQueueCreate(10,sizeof(std_msgs__msg__Int32));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
    // micro-ROS configuration
    rmw_uros_set_custom_transport(
            true,
            NULL,
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
    rcl_subscription_t rs485sub;
    rcl_subscription_t stepping;
    rcl_timer_t rs485pub_timer;
    std_msgs__msg__Int32 msg;
    armquadrover_msgs__msg__DDSM115 ddsm115_rmsg;
    std_msgs__msg__Int32  stepping_rmsg;


    rclc_support_t support;
    rcl_allocator_t allocator;
    rclc_executor_t executor;
    rcl_node_t node;

    allocator = rcl_get_default_allocator();

    //create init_options
    rclc_support_init(&support, 0, NULL, &allocator);

    // create node
    rclc_node_init_default(&node, "base_node", "", &support);
    // create publisher
    rclc_publisher_init_default(
            &publisher,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(armquadrover_msgs, msg, DDSM115),
            "base_states");
    // 创建订阅
    rclc_subscription_init_default(
            &rs485sub,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(armquadrover_msgs, msg, DDSM115),
            "base_control"
            );
    rclc_subscription_init_default(
            &stepping,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
            "stepping_control"
    );
    rclc_timer_init_default(
            &rs485pub_timer,
            &support,
            RCL_MS_TO_NS(10),
            rs485pub_timer_callback);
    msg.data = 0;
    executor = rclc_executor_get_zero_initialized_executor();
    rclc_executor_init(&executor,&support.context,3,&allocator);
    rclc_executor_add_timer(&executor,&rs485pub_timer);
    rclc_executor_add_subscription(&executor, &rs485sub ,&ddsm115_rmsg, rs485_subscription_callback,ON_NEW_DATA);
    rclc_executor_add_subscription(&executor, &stepping, &stepping_rmsg, stepping_subscription_callback, ON_NEW_DATA);

    for(;;)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
        osDelay(10);
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
armquadrover_msgs__msg__DDSM115  ddsm115Servo;
void rs485pub_timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
    armquadrover_msgs__msg__DDSM115  ddsm11;
    ddsm11=getCurrentSpeed();
    rcl_publish(&publisher,&ddsm11,NULL);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
void rs485_subscription_callback(const void * msgin)
{
    const armquadrover_msgs__msg__DDSM115  * msg = (const armquadrover_msgs__msg__DDSM115 *)msgin;
    setTargetSpeed(msg->front_left,msg->rear_left,msg->rear_right,msg->front_right,msg->front_left_servo,msg->rear_left_servo,msg->rear_right_servo,msg->front_right_servo);
}
void stepping_subscription_callback(const void * msgin)
{

    const std_msgs__msg__Int32* msg = (const std_msgs__msg__Int32*)msgin;
    if(msg->data>0) {
        Emm_V5_Pos_Control(1, 0, 1000, 10, msg->data / 72.0 * 3200, 1, 0);
    } else{
        Emm_V5_Pos_Control(1, 1, 1000, 10, msg->data / 72.0 * 3200, 1, 0);
    }
}

/* USER CODE END Application */

