/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DDSM115_POWER_Pin GPIO_PIN_2
#define DDSM115_POWER_GPIO_Port GPIOE
#define ARM2_POWER_Pin GPIO_PIN_4
#define ARM2_POWER_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOE
#define ARM1_TX_Pin GPIO_PIN_0
#define ARM1_TX_GPIO_Port GPIOA
#define ARM1_RX_Pin GPIO_PIN_1
#define ARM1_RX_GPIO_Port GPIOA
#define Ddsm115Servo_TX_Pin GPIO_PIN_10
#define Ddsm115Servo_TX_GPIO_Port GPIOB
#define Ddsm115Servo_RX_Pin GPIO_PIN_11
#define Ddsm115Servo_RX_GPIO_Port GPIOB
#define STEPPING_TX_Pin GPIO_PIN_6
#define STEPPING_TX_GPIO_Port GPIOC
#define STEPPING_RX_Pin GPIO_PIN_7
#define STEPPING_RX_GPIO_Port GPIOC
#define DDSM115_POWER_CON_Pin GPIO_PIN_8
#define DDSM115_POWER_CON_GPIO_Port GPIOA
#define Ddsm115_TX_Pin GPIO_PIN_9
#define Ddsm115_TX_GPIO_Port GPIOA
#define Ddsm115_RX_Pin GPIO_PIN_10
#define Ddsm115_RX_GPIO_Port GPIOA
#define MICRO_ROS_DM_Pin GPIO_PIN_11
#define MICRO_ROS_DM_GPIO_Port GPIOA
#define MICRO_ROS_DP_Pin GPIO_PIN_12
#define MICRO_ROS_DP_GPIO_Port GPIOA
#define ARM2_TX_Pin GPIO_PIN_12
#define ARM2_TX_GPIO_Port GPIOC
#define ARM2_RX_Pin GPIO_PIN_2
#define ARM2_RX_GPIO_Port GPIOD
#define ARM1_POWER_CON_Pin GPIO_PIN_3
#define ARM1_POWER_CON_GPIO_Port GPIOD
#define ARM2_POWER_CON_Pin GPIO_PIN_4
#define ARM2_POWER_CON_GPIO_Port GPIOD
#define STEPPING_POWER_Pin GPIO_PIN_1
#define STEPPING_POWER_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
