/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAX_VAL 255

uint8_t rx_buff[1];

const uint8_t imagem[32][92] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 106, 99, 97, 88, 93, 101, 98, 96, 90, 88, 91, 86, 86, 83, 82, 85, 88, 86, 86, 88, 83, 84, 91, 92, 87, 90, 90, 91, 98, 94, 99, 96, 93, 95, 88, 91, 91, 97, 95, 99, 105, 99, 86, 88, 88, 91, 94, 94, 96, 87, 86, 86, 86, 86, 84, 81, 80, 80, 81, 84, 87, 89, 87, 79, 79, 78, 75, 74, 75, 78, 80, 83, 77, 81, 82, 77, 75, 80, 84, 85, 79, 75, 70, 69, 70, 70, 74, 75, 69, 70, 0},
    {0, 112, 105, 96, 92, 95, 97, 103, 99, 96, 99, 90, 84, 86, 87, 92, 95, 94, 85, 92, 97, 93, 88, 93, 103, 103, 100, 99, 100, 103, 101, 94, 99, 108, 104, 95, 98, 100, 105, 105, 114, 113, 103, 104, 109, 114, 127, 134, 128, 112, 113, 120, 110, 99, 94, 88, 80, 81, 82, 85, 92, 84, 87, 90, 81, 81, 88, 84, 83, 84, 88, 96, 92, 87, 95, 87, 83, 83, 83, 83, 78, 76, 74, 68, 71, 73, 70, 71, 73, 73, 70, 0},
    {0, 107, 104, 96, 96, 86, 87, 96, 97, 94, 94, 87, 79, 80, 85, 92, 93, 96, 95, 108, 113, 106, 91, 91, 95, 95, 101, 105, 105, 110, 113, 116, 114, 108, 102, 103, 114, 115, 119, 130, 125, 116, 105, 117, 128, 128, 144, 131, 124, 124, 125, 129, 122, 114, 111, 100, 96, 97, 97, 97, 99, 82, 82, 89, 83, 87, 90, 88, 89, 87, 87, 87, 86, 94, 101, 94, 89, 81, 80, 79, 80, 76, 77, 74, 75, 74, 72, 71, 75, 79, 73, 0},
    {0, 108, 108, 101, 96, 88, 93, 97, 95, 91, 88, 83, 80, 81, 84, 92, 94, 98, 101, 109, 112, 109, 94, 94, 99, 95, 98, 103, 108, 110, 120, 119, 119, 116, 115, 111, 123, 127, 131, 139, 147, 143, 128, 133, 132, 129, 131, 131, 126, 126, 129, 130, 136, 130, 126, 117, 111, 110, 112, 94, 90, 85, 92, 87, 77, 80, 77, 83, 83, 80, 82, 80, 79, 83, 87, 93, 91, 83, 82, 79, 77, 77, 79, 81, 81, 76, 73, 73, 78, 79, 73, 0},
    {0, 117, 112, 110, 109, 98, 104, 100, 89, 91, 96, 96, 89, 90, 85, 86, 87, 90, 102, 101, 100, 100, 100, 104, 105, 103, 96, 101, 107, 109, 118, 114, 114, 116, 117, 114, 113, 134, 153, 145, 120, 88, 62, 49, 38, 34, 30, 34, 34, 37, 46, 60, 83, 107, 133, 138, 120, 113, 106, 103, 98, 96, 102, 90, 77, 78, 77, 83, 87, 85, 81, 80, 85, 87, 86, 89, 86, 85, 79, 81, 78, 80, 88, 92, 87, 82, 79, 76, 77, 77, 76, 0},
    {0, 104, 104, 109, 110, 116, 118, 102, 94, 92, 103, 103, 101, 93, 86, 87, 87, 93, 97, 98, 99, 108, 101, 101, 98, 102, 107, 103, 107, 115, 117, 114, 121, 120, 113, 115, 124, 124, 86, 43, 22, 16, 15, 15, 17, 17, 16, 14, 13, 14, 14, 11, 9, 16, 44, 79, 119, 145, 125, 103, 96, 103, 102, 89, 83, 85, 87, 91, 92, 97, 90, 87, 86, 89, 98, 103, 92, 91, 92, 85, 84, 87, 96, 101, 95, 87, 88, 81, 77, 83, 83, 0},
    {0, 118, 104, 103, 111, 115, 111, 104, 98, 90, 100, 106, 111, 101, 94, 94, 103, 111, 97, 97, 104, 115, 106, 100, 98, 100, 102, 98, 115, 125, 123, 121, 124, 131, 134, 131, 86, 36, 16, 15, 20, 30, 27, 20, 19, 18, 17, 17, 15, 18, 19, 18, 17, 14, 10, 9, 29, 89, 140, 132, 126, 118, 108, 102, 96, 101, 95, 98, 100, 108, 109, 106, 99, 110, 126, 121, 103, 90, 100, 100, 100, 93, 96, 96, 92, 88, 92, 92, 91, 89, 81, 0},
    {0, 118, 117, 109, 104, 112, 112, 115, 110, 103, 112, 116, 112, 96, 96, 110, 119, 119, 115, 119, 116, 117, 103, 105, 99, 97, 98, 111, 120, 119, 118, 124, 128, 142, 130, 58, 18, 16, 16, 13, 17, 29, 16, 14, 15, 15, 14, 17, 16, 17, 18, 17, 16, 17, 17, 16, 13, 7, 55, 137, 154, 144, 132, 117, 110, 107, 104, 104, 104, 101, 95, 99, 112, 126, 127, 123, 102, 93, 104, 110, 103, 100, 93, 88, 99, 99, 99, 103, 105, 94, 80, 0},
    {0, 125, 138, 139, 134, 134, 129, 125, 126, 120, 121, 116, 101, 91, 101, 119, 122, 120, 123, 130, 128, 129, 120, 120, 113, 105, 110, 124, 126, 127, 135, 142, 149, 143, 60, 9, 18, 26, 37, 53, 72, 82, 69, 51, 43, 37, 30, 24, 19, 16, 16, 15, 18, 19, 17, 15, 16, 17, 8, 35, 133, 159, 140, 122, 115, 112, 108, 118, 119, 113, 106, 112, 117, 125, 120, 111, 97, 106, 114, 102, 102, 113, 102, 94, 100, 104, 105, 112, 112, 106, 89, 0},
    {0, 122, 134, 150, 152, 140, 123, 118, 129, 136, 132, 121, 113, 114, 117, 125, 125, 128, 129, 133, 134, 132, 128, 119, 123, 120, 118, 132, 141, 141, 147, 148, 148, 72, 13, 32, 56, 106, 152, 167, 185, 188, 177, 146, 113, 92, 80, 57, 40, 31, 25, 21, 19, 20, 20, 18, 16, 17, 18, 9, 39, 138, 146, 137, 124, 121, 114, 129, 130, 132, 136, 134, 131, 128, 124, 118, 107, 110, 113, 103, 97, 106, 108, 105, 102, 109, 103, 117, 125, 124, 100, 0},
    {0, 139, 139, 136, 133, 133, 121, 116, 121, 129, 145, 148, 138, 137, 133, 135, 139, 141, 135, 142, 135, 136, 129, 122, 130, 134, 133, 140, 146, 148, 151, 150, 87, 18, 31, 82, 157, 217, 231, 238, 246, 250, 246, 234, 210, 185, 158, 111, 72, 49, 34, 28, 26, 23, 20, 19, 14, 17, 19, 18, 9, 55, 147, 149, 128, 117, 114, 117, 114, 120, 136, 133, 139, 137, 128, 125, 113, 114, 119, 118, 108, 98, 104, 108, 102, 109, 107, 112, 123, 121, 104, 0},
    {0, 153, 144, 151, 145, 133, 131, 144, 142, 145, 160, 152, 146, 153, 145, 142, 142, 142, 134, 142, 148, 148, 140, 142, 149, 136, 128, 126, 130, 150, 150, 101, 21, 18, 59, 131, 213, 237, 239, 248, 254, 254, 250, 245, 234, 218, 182, 132, 83, 56, 43, 37, 31, 27, 22, 17, 15, 17, 19, 18, 16, 9, 98, 159, 142, 109, 93, 101, 118, 131, 134, 134, 140, 142, 136, 122, 116, 120, 123, 130, 124, 114, 120, 120, 120, 123, 126, 127, 126, 115, 108, 0},
    {0, 153, 154, 180, 168, 140, 158, 157, 184, 197, 186, 167, 169, 176, 160, 159, 159, 165, 148, 151, 160, 156, 151, 147, 153, 144, 135, 137, 139, 141, 106, 32, 13, 29, 79, 133, 187, 219, 222, 223, 226, 228, 230, 228, 217, 186, 139, 102, 76, 63, 55, 43, 32, 29, 27, 20, 16, 18, 19, 18, 17, 9, 40, 134, 105, 87, 87, 100, 122, 101, 121, 131, 144, 127, 141, 125, 119, 118, 120, 125, 125, 129, 140, 141, 136, 139, 138, 134, 133, 123, 109, 0},
    {0, 141, 151, 168, 157, 142, 147, 143, 181, 173, 179, 181, 188, 196, 177, 179, 197, 192, 175, 183, 167, 162, 166, 156, 145, 139, 146, 159, 185, 128, 39, 21, 18, 37, 87, 137, 175, 200, 202, 198, 193, 197, 214, 220, 205, 164, 123, 107, 93, 80, 70, 61, 51, 37, 27, 23, 20, 18, 19, 19, 15, 14, 16, 59, 63, 73, 84, 80, 83, 67, 82, 121, 110, 107, 130, 113, 128, 126, 129, 133, 127, 133, 143, 143, 141, 143, 139, 142, 137, 125, 105, 0},
    {0, 117, 110, 114, 114, 110, 104, 105, 151, 161, 162, 201, 194, 159, 135, 130, 185, 209, 214, 205, 168, 170, 190, 172, 159, 177, 192, 202, 193, 68, 21, 18, 18, 56, 106, 149, 186, 206, 208, 203, 198, 200, 221, 236, 229, 203, 171, 154, 142, 125, 103, 80, 66, 44, 28, 24, 19, 18, 18, 17, 16, 15, 14, 35, 57, 55, 67, 63, 59, 61, 73, 92, 78, 96, 93, 89, 149, 142, 144, 149, 146, 144, 142, 137, 134, 145, 148, 147, 134, 113, 95, 0},
    {0, 112, 81, 65, 76, 99, 96, 92, 94, 128, 139, 151, 140, 117, 105, 98, 108, 170, 157, 179, 207, 212, 206, 208, 206, 219, 226, 242, 153, 34, 20, 18, 17, 54, 112, 153, 166, 166, 184, 206, 212, 220, 204, 163, 130, 126, 136, 134, 131, 117, 96, 73, 60, 44, 32, 27, 19, 18, 18, 18, 16, 16, 17, 28, 47, 38, 51, 62, 66, 65, 64, 57, 70, 87, 76, 76, 118, 140, 143, 147, 144, 142, 142, 140, 136, 143, 132, 125, 100, 90, 80, 0},
    {0, 77, 73, 68, 69, 78, 90, 91, 80, 90, 104, 91, 90, 94, 78, 72, 78, 66, 60, 82, 126, 189, 192, 196, 202, 198, 202, 197, 95, 21, 22, 20, 17, 47, 94, 92, 69, 58, 85, 165, 191, 169, 86, 29, 23, 38, 60, 59, 52, 47, 41, 34, 35, 37, 32, 29, 19, 17, 18, 18, 19, 18, 17, 24, 37, 34, 42, 48, 54, 61, 58, 58, 62, 73, 59, 68, 66, 115, 149, 144, 139, 138, 143, 143, 147, 140, 117, 105, 85, 86, 68, 0},
    {0, 66, 73, 69, 72, 94, 88, 76, 82, 90, 83, 71, 70, 63, 55, 54, 53, 54, 68, 67, 47, 107, 180, 191, 180, 149, 151, 142, 76, 23, 23, 21, 18, 25, 27, 40, 47, 32, 25, 83, 159, 101, 27, 17, 41, 64, 45, 29, 20, 16, 15, 27, 41, 38, 32, 28, 21, 16, 17, 18, 19, 19, 17, 18, 34, 36, 36, 37, 36, 48, 50, 52, 62, 46, 36, 57, 64, 97, 153, 144, 127, 130, 130, 132, 121, 111, 106, 94, 95, 88, 65, 0},
    {0, 68, 74, 76, 78, 86, 80, 84, 87, 77, 73, 67, 69, 68, 52, 51, 54, 52, 64, 70, 66, 142, 169, 152, 98, 101, 118, 136, 94, 23, 20, 20, 17, 54, 59, 71, 75, 34, 41, 76, 152, 88, 24, 39, 86, 109, 52, 31, 31, 49, 88, 105, 77, 48, 32, 26, 19, 18, 18, 16, 18, 19, 17, 17, 33, 36, 35, 36, 36, 36, 38, 37, 47, 43, 37, 43, 51, 82, 144, 143, 125, 117, 117, 115, 101, 91, 97, 93, 87, 74, 67, 0},
    {0, 75, 76, 77, 85, 85, 79, 84, 87, 75, 72, 73, 72, 69, 42, 52, 54, 47, 57, 56, 65, 101, 108, 72, 58, 56, 63, 111, 63, 21, 21, 18, 17, 98, 165, 143, 115, 95, 122, 139, 165, 83, 39, 75, 124, 125, 129, 125, 133, 143, 143, 128, 80, 44, 27, 23, 19, 18, 19, 17, 16, 17, 19, 16, 28, 31, 31, 34, 37, 40, 46, 44, 40, 38, 39, 43, 49, 66, 115, 146, 120, 111, 102, 100, 99, 94, 94, 90, 82, 73, 71, 0},
    {0, 75, 56, 69, 79, 77, 77, 78, 72, 66, 62, 67, 58, 49, 48, 52, 52, 39, 40, 46, 44, 47, 52, 53, 63, 56, 47, 56, 28, 26, 20, 18, 19, 91, 173, 206, 210, 211, 204, 196, 182, 82, 58, 114, 193, 217, 231, 235, 219, 171, 117, 81, 62, 39, 27, 24, 19, 19, 19, 17, 14, 16, 19, 15, 30, 50, 43, 39, 40, 46, 47, 42, 41, 40, 32, 31, 35, 46, 74, 116, 115, 102, 92, 91, 98, 107, 99, 88, 86, 85, 77, 0},
    {0, 58, 39, 65, 75, 72, 72, 74, 70, 55, 51, 63, 60, 52, 52, 56, 48, 42, 41, 44, 45, 43, 32, 39, 57, 49, 46, 34, 22, 23, 21, 21, 17, 54, 143, 195, 208, 206, 175, 192, 193, 92, 66, 105, 170, 192, 205, 207, 182, 136, 86, 62, 51, 36, 29, 25, 20, 19, 19, 18, 16, 17, 18, 15, 36, 71, 66, 66, 84, 85, 87, 74, 74, 78, 77, 77, 77, 81, 81, 83, 100, 104, 95, 94, 90, 89, 93, 90, 90, 95, 84, 0},
    {0, 37, 31, 49, 73, 67, 59, 57, 64, 53, 54, 66, 67, 62, 53, 53, 45, 43, 46, 46, 46, 43, 42, 42, 47, 44, 38, 22, 20, 20, 20, 20, 16, 35, 111, 167, 191, 160, 110, 194, 196, 95, 66, 77, 164, 172, 173, 167, 138, 100, 65, 51, 39, 31, 30, 26, 20, 17, 18, 17, 18, 18, 18, 16, 20, 31, 33, 38, 50, 58, 59, 51, 50, 53, 60, 67, 75, 83, 89, 102, 115, 112, 96, 80, 66, 62, 56, 64, 94, 105, 69, 0},
    {0, 36, 34, 38, 64, 71, 63, 57, 58, 54, 55, 58, 60, 60, 58, 61, 55, 49, 54, 52, 47, 45, 45, 45, 49, 45, 39, 25, 21, 19, 20, 19, 14, 23, 66, 116, 142, 149, 114, 120, 106, 47, 35, 39, 146, 181, 146, 128, 91, 67, 53, 41, 34, 32, 32, 29, 21, 17, 16, 17, 15, 18, 19, 16, 17, 27, 30, 34, 35, 34, 31, 32, 34, 33, 31, 34, 37, 44, 58, 63, 62, 59, 59, 55, 55, 50, 47, 45, 50, 48, 45, 0},
    {0, 33, 34, 32, 42, 72, 68, 62, 58, 60, 58, 54, 53, 53, 57, 64, 58, 50, 52, 51, 49, 54, 49, 48, 51, 46, 40, 26, 21, 20, 19, 19, 17, 19, 51, 77, 92, 157, 222, 150, 35, 26, 37, 55, 97, 107, 88, 81, 70, 60, 51, 41, 35, 33, 32, 27, 21, 17, 16, 16, 17, 19, 19, 17, 15, 27, 37, 39, 41, 40, 38, 35, 35, 33, 33, 39, 41, 46, 49, 43, 42, 46, 52, 56, 58, 51, 58, 65, 56, 52, 51, 0},
    {0, 34, 35, 34, 39, 68, 69, 63, 60, 57, 53, 53, 54, 54, 54, 58, 55, 51, 51, 52, 55, 58, 49, 47, 50, 44, 42, 30, 20, 20, 21, 23, 19, 15, 37, 77, 86, 74, 95, 129, 87, 55, 53, 56, 46, 54, 77, 85, 79, 63, 54, 46, 38, 33, 29, 24, 19, 18, 19, 17, 17, 16, 16, 18, 18, 25, 37, 38, 44, 37, 39, 40, 43, 35, 35, 36, 36, 44, 56, 57, 57, 58, 62, 66, 65, 59, 54, 55, 45, 40, 39, 0},
    {0, 37, 34, 34, 49, 69, 67, 62, 60, 56, 56, 59, 58, 57, 55, 49, 46, 48, 50, 51, 48, 46, 48, 47, 43, 41, 44, 34, 21, 20, 22, 22, 17, 16, 18, 56, 102, 120, 147, 114, 82, 64, 51, 50, 66, 86, 93, 89, 74, 59, 50, 45, 37, 30, 26, 22, 17, 18, 20, 19, 18, 16, 16, 18, 18, 20, 36, 41, 43, 42, 46, 47, 44, 44, 45, 49, 50, 52, 63, 69, 68, 68, 67, 62, 54, 49, 43, 36, 39, 46, 43, 39, 0},
    {0, 35, 31, 39, 48, 56, 63, 62, 60, 56, 56, 56, 53, 55, 49, 45, 47, 47, 44, 45, 43, 40, 43, 45, 47, 47, 50, 35, 23, 21, 20, 19, 19, 18, 15, 18, 46, 99, 155, 138, 90, 69, 62, 69, 80, 83, 82, 72, 56, 47, 41, 42, 33, 29, 26, 22, 19, 19, 21, 20, 18, 16, 17, 17, 19, 18, 38, 47, 41, 47, 55, 53, 52, 46, 46, 47, 49, 46, 48, 48, 41, 40, 37, 31, 31, 35, 38, 54, 61, 49, 43, 0},
    {0, 34, 33, 34, 34, 34, 48, 58, 60, 57, 53, 53, 50, 51, 47, 48, 45, 47, 49, 55, 57, 55, 55, 59, 58, 58, 63, 43, 22, 24, 20, 19, 20, 19, 18, 13, 14, 32, 87, 183, 220, 198, 137, 98, 85, 77, 68, 52, 39, 39, 38, 35, 31, 27, 24, 23, 19, 20, 21, 18, 17, 18, 19, 17, 17, 17, 35, 55, 52, 51, 57, 60, 57, 50, 47, 47, 46, 50, 52, 50, 45, 47, 46, 47, 50, 52, 53, 61, 68, 60, 47, 0},
    {0, 31, 33, 27, 27, 29, 27, 41, 61, 56, 52, 51, 50, 52, 46, 44, 48, 47, 48, 53, 51, 54, 53, 57, 61, 59, 62, 54, 24, 21, 21, 21, 20, 21, 18, 18, 16, 13, 22, 75, 132, 136, 100, 76, 63, 52, 45, 41, 38, 35, 32, 30, 27, 27, 28, 24, 19, 18, 18, 19, 17, 18, 20, 19, 17, 16, 34, 53, 57, 54, 55, 61, 56, 55, 51, 54, 53, 49, 54, 52, 56, 61, 55, 53, 53, 48, 45, 61, 62, 53, 41, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

uint8_t imagem_padd[32][92];
uint8_t linhas_finais[2][92];
uint8_t suavizado[30][90];
uint8_t filtrado[30][90];
uint8_t altura = 30, largura = 90, max_val;
uint8_t altura_ini, largura_ini, altura_fin, largura_fin;

uint8_t fatia = 0;
uint8_t total_fatias = 5;
float amp = 2.0f;

int receber_linhas(uint8_t *line_buffer)
{
	uint8_t rx_byte;
	int pixel_count = 0;
	int valor_atual = 0;
	int has_digit = 0;

	while (pixel_count < 90)
	{
		if (HAL_UART_Receive(&huart2, &rx_byte, 1, 3600000) == HAL_OK)
		{
			if (rx_byte >= '0' && rx_byte <= '9')
			{
				valor_atual = valor_atual * 10 + (rx_byte - '0');
				has_digit = 1;
			}
			else if (rx_byte == ' ' || rx_byte == '\n' || rx_byte == '\r')
			{
				if (has_digit)
				{
					line_buffer[pixel_count++] = (uint8_t)valor_atual;
					valor_atual = 0;
					has_digit = 0;
					printf(valor_atual);
				}
				if (rx_byte == '\n')
					break;
			}
		}
		else
		{
			return 0; // Timeout
		}
	}
	return (pixel_count == 90) ? 1 : 0;
}





/*
 * Copias as linhas finais, caso a condicao seja 0
 * Caso condicao maior que 0, cola as linhas finais no topo da imagem e copia as ultimas
 */
void copiar_linhas_finais(uint8_t condicao) {
	if (condicao == 0) {
		for (int i = altura - 1; i < altura + 1; i++) {
			for (int j = 0; j < largura + 2; j++) {
				linhas_finais[i][j] = imagem_padd[i][j];
			}
		}
	} else if (condicao > 0) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < largura + 2; j++) {
				imagem_padd[i][j] = linhas_finais[i][j];
			}
		}

		for (int i = altura - 1; i < altura + 1; i++) {
			for (int j = 0; j < largura + 2; j++) {
				linhas_finais[i][j] = imagem_padd[i][j];
			}
		}
	}
}



// Lembrar de colocar no loop !!!!!!!!!!!!!!!!!
void high_boost() {
	// Aplica o zero padding antes da imagem ser recebida
	for (int i = 0; i < altura + 2; i++) {
		for (int j = 0; j < largura + 2; j++) {
			imagem_padd[i][j] = 0;
		}
	}



	//Recebendo a imagem/matriz --> A FAZER
	for (int i = 0; i < 32; i++) {
		receber_linhas(imagem_padd[i + 1]);
	}

	//Copiando as duas ultimas linhas
	//copiar_linhas_finais(fatia);


	// Aplica filtro de suavizacao
	// A imagem e percorrida pixel a pixel enquanto calcula a media
	// somando o valor dos pixels ao redor (3x3) e dividindo por 9
	for (int i = 1; i < altura + 1; i++) {
		for (int j = 1; j < largura + 1; j++) {
			// Calcula media 3x3
			int sum = 0;
			for (int di = -1; di <= 1; di++) {
				for (int dj = -1; dj <= 1; dj++) {
					sum += imagem_padd[i + di][j + dj];
				}
			}
			int avg = sum / 9;

			// Garante que os valores da matriz estejam no intervalo [0, 255]
			if (avg < 0) avg = 0;
			if (avg > MAX_VAL) avg = MAX_VAL;

			suavizado[i - 1][j - 1] = avg;
		}
	}

	for (int i = 0; i < altura; i++) {
		for (int j = 0; j < largura; j++) {
			int resultado;

			// Variavel refernte a (imagem - suavizado)
			int high_pass = imagem_padd[i + 1][j + 1] - suavizado[i][j];

			resultado = (amp - 1.0f) * imagem_padd[i + 1][j + 1] + high_pass;

			// Garante que os valores da matriz estejam no intervalo [0, 255]
			if (resultado < 0) resultado = 0;
			if (resultado > MAX_VAL) resultado = MAX_VAL;

			//Insere o resultado na matriz filtrado
			printf("%d ", resultado);
		}
		printf("\n");
		HAL_Delay(300);
	}
	printf("\n--- Processamento Completo ---\n");
	printf("Dimensões: %dx%d\n", altura, largura);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  high_boost();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

int __io_putchar(int ch)
{
  // Usa um buffer de 8 bits para o caractere
  uint8_t data = (uint8_t)ch;

  // Transmite o caractere via UART2 (huart2)
  // O último parâmetro (100) é o timeout em ms.
  HAL_UART_Transmit(&huart2, &data, 1, 100);

  return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
