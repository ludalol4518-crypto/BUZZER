/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Merry-Go-Round of Life (Howl's Moving Castle)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    uint16_t frequency;
    uint16_t duration;
} Note;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// 박자 정의
#define WHOLE     1800
#define HALF      900
#define QUARTER   450
#define EIGHTH    225
#define SIXTEENTH 112

// 음표 정의 (D6, DS6 등 누락된 모든 음표 포함)
#define NOTE_G4   392
#define NOTE_A4   440
#define NOTE_AS4  466
#define NOTE_D5   587
#define NOTE_G5   784
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_C6   1047
#define NOTE_CS6  1109
#define NOTE_D6   1175
#define NOTE_DS6  1245
#define NOTE_E6   1319
#define NOTE_F6   1397
#define NOTE_G6   1568
#define NOTE_A6   1760
#define NOTE_AS6  1865
#define REST      0
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
const Note howl_theme[] = {
    {NOTE_D5, QUARTER}, {NOTE_G5, QUARTER}, {NOTE_AS5, QUARTER},
    {NOTE_D6, HALF}, {NOTE_C6, QUARTER},
    {NOTE_AS5, HALF}, {NOTE_A5, QUARTER},
    {NOTE_G5, HALF}, {NOTE_D5, QUARTER},
    {NOTE_G4, EIGHTH}, {NOTE_AS4, EIGHTH}, {NOTE_D5, EIGHTH}, {NOTE_G5, EIGHTH}, {NOTE_AS5, EIGHTH}, {NOTE_D6, EIGHTH},
    {NOTE_G6, HALF}, {NOTE_F6, QUARTER},
    {NOTE_DS6, HALF}, {NOTE_D6, QUARTER},
    {NOTE_C6, HALF}, {NOTE_AS5, QUARTER},
    {NOTE_A5, HALF}, {NOTE_G5, QUARTER},
    {NOTE_D6, HALF}, {NOTE_G6, QUARTER},
    {NOTE_AS6, HALF}, {NOTE_A6, QUARTER},
    {NOTE_G6, HALF}, {NOTE_D6, QUARTER},
    {NOTE_AS5, HALF}, {NOTE_A5, QUARTER},
    {NOTE_G5, HALF}, {REST, QUARTER}
};

const int howl_theme_length = sizeof(howl_theme) / sizeof(howl_theme[0]);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void play_tone(uint16_t frequency, uint16_t duration);
void play_howl_theme(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void play_howl_theme(void) {
    for (int i = 0; i < howl_theme_length; i++) {
        play_tone(howl_theme[i].frequency, howl_theme[i].duration);
    }
}

void play_tone(uint16_t frequency, uint16_t duration) {
    if (frequency == 0) {
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    } else {
        uint32_t arr_value = 1000000 / frequency - 1;
        __HAL_TIM_SET_AUTORELOAD(&htim1, arr_value);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, arr_value / 2);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    }
    HAL_Delay(duration);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_Delay(40);
}
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();

  while (1)
  {
      play_howl_theme();
      HAL_Delay(3000);
  }
}

/* 아래는 시스템 설정 코드 (수정하지 마세요) */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 63;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim1);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
  HAL_TIM_PWM_Init(&htim1);
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_MspPostInit(&htim1);
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  HAL_UART_Init(&huart2);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void) { while(1); }
