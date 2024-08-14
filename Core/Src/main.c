/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "quadspi.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifndef XENSIV_BGT60TRXX_CONF_IMPL
#define XENSIV_BGT60TRXX_CONF_IMPL
#endif
#include "xensiv_bgt60trxx.h"
#include "presence_radar_settings.h"
#include <inttypes.h>
#include <stdio.h>
#include "usbd_cdc_if.h"

//Ai include
#include "stdio.h"
#include "math.h"
#include "time.h"
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"

//MATLAB include

#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "matlab_feature_extract.h"
#include "rt_nonfinite.h"
#include "start_judge.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define XENSIV_BGT60TRXX_SPI_FREQUENCY (25000000UL)
#define NUM_SAMPLES_PER_FRAME (XENSIV_BGT60TRXX_CONF_NUM_RX_ANTENNAS *      \
                              XENSIV_BGT60TRXX_CONF_NUM_CHIRPS_PER_FRAME * \
                              XENSIV_BGT60TRXX_CONF_NUM_SAMPLES_PER_CHIRP)
#define NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA (NUM_SAMPLES_PER_FRAME * 3 / 2)
#define NUM_FRAMES_BUFFER 15
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static xensiv_bgt60trxx_t sensor;
static volatile bool data_available = false;
static volatile bool dma_flag = true;
static volatile bool dma_flag_uart = true;

/* Allocate enough memory for the radar dara frame. */
//static uint16_t samples[NUM_SAMPLES_PER_FRAME];
static uint8_t adcRawData[NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA];// 64x32x1.5x3 采数据buffer到底怎么为? 
static uint8_t *framept = 0;

static uint16_t resultArray[NUM_SAMPLES_PER_FRAME];//64x32x3
static uint16_t *resultpt = 0;
static uint16_t *judgept = 0;
//uint8_t testArray[4];// used to test bit12 to bit16 function

bool tranformSpeed = false;
uint32_t frame_idx = 0;
uint32_t frame_stop = 0;

uint32_t last_irq_tick = 0;
uint32_t irq_tick = 0;
uint32_t spi_enable_tick = 0;
uint32_t usart_dma_start = 0;
uint32_t spi_dma_cost = 0;
uint32_t usart_dma_cost = 0;
//uint32_t wait_time = 0;

// AI parameter
ai_handle network;
//float aiInData[AI_NETWORK_IN_1_SIZE]={-9.298612677079564,-8.86344077881372,-11.08269874047248,-9.270826398365454,-10.672292530904508,-9.188027646976918,-10.27581200912054,-8.675486063269718,-7.447736380801032,-7.803294279997628,-7.895103838183733,-6.450533829811868,-6.481157021452547,-7.3656509196386715,-6.065664863862057,22.48991611087293,20.775957005925,27.07810255942578,31.758055492959848,41.979656567324525,52.22430684218063,55.060375875843675,37.483191287575906,43.63150679288669,30.709718200966023,31.58083633186333,33.88890309505801,19.30882870912667,11.115082341890556,26.28208050746487,10.593622416136668,8.269338193183755,12.760632545701025,12.45853620977311,14.065016506403426,6.65523659620928,4.005883969835484,9.88960227950114,24.914375290176007,12.434666493981167,13.296421579749007,16.639425496604815,-0.7599367212720368,-11.791494294596248,-13.347109220088301,12.0,10.0,12.0,12.0,13.0,12.0,14.0,11.0,16.0,14.0,13.0,16.0,15.0,13.0,18.0,9.71464301421261,9.51747764034695,8.622936173900973,8.304078469372175,8.32048840330877,7.903490795459883,7.347093191043699,7.884230117261198,8.10409225687522,7.069524413199044,6.377353668600602,6.913473996706376,6.165036807793203,6.670487207277613,5.357349487120304,0.0,-0.9944018467528082,-0.9952813269678167,-0.9905601462849846,-0.9898838144193726,-0.9726568482187672,-0.9144928039317419,-0.9130732943192493,-0.7335775773917104,-0.7135953449894658,-0.6812950820682971,-0.5914069008091457,-0.4857954383416314,-0.3017532443382474};
float aiOutData[AI_NETWORK_OUT_1_SIZE];
ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

ai_buffer * ai_input;
ai_buffer * ai_output;

static void AI_Init(void);
static void AI_Run(float *pIn, float *pOut);


clock_t start, stop;
double duration_Init, duration_Run;
double StartTime = 0, EndTime = 0;

volatile uint32_t TIM_start = 0;
volatile uint32_t TIM_end = 0;
volatile uint32_t TIM = 0;


// Matlab parameter

uint16_t matlabInData[6144];
float matlabOutData[6] = {1,2,3,4,5,6};
float featuresMatrix[90];

//uint16_t meanBuffer[64*3*15];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
//void BIT12_TO_BIT16(uint8_t *A, int16_t *B, uint16_t Alen);
void BIT12_TO_BIT16(uint8_t *A, uint16_t *B, uint16_t Alen);

/* Matlab Function Declarations */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_QUADSPI_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(5000);

  TIM_start = HAL_GetTick();    
  AI_Init();
  TIM_end = HAL_GetTick();
  TIM = TIM_end - TIM_start;
  HAL_Delay(1);
  usb_printf("Model Initiate time=%u\r\n", TIM);
  HAL_Delay(1);


  HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin | LED_RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN_LDO_GPIO_Port, EN_LDO_Pin, GPIO_PIN_SET);
  static xensiv_bgt60trxx_st_iface_t spiface;
  spiface.spi = &hspi1;
  spiface.selpin.GPIOx = SPI1_NSS_GPIO_Port;
  spiface.selpin.GPIO_Pin = SPI1_NSS_Pin;
  spiface.rstpin.GPIOx = BGT_RST_GPIO_Port;
  spiface.rstpin.GPIO_Pin = BGT_RST_Pin;  

  uint32_t rslt;
  rslt = xensiv_bgt60trxx_init(&sensor, &spiface, tranformSpeed); // Radar Init
  xensiv_bgt60trxx_get_device(&sensor);

  uint32_t Nchipid;
  uint32_t NADC0;
  HAL_Delay(3000);
  xensiv_bgt60trxx_get_reg(&sensor, XENSIV_BGT60TRXX_REG_CHIP_ID, &Nchipid);// 
  usb_printf("chipid:%08X\n", Nchipid);
  xensiv_bgt60trxx_get_reg(&sensor, XENSIV_BGT60TRXX_REG_ADC0, &NADC0);
  usb_printf("ADC0:%08X\n", NADC0);

  if (rslt == XENSIV_BGT60TRXX_STATUS_OK)
  {
  xensiv_bgt60trxx_hard_reset(&sensor);
  }
  if (rslt == XENSIV_BGT60TRXX_STATUS_OK)
  {
  rslt = xensiv_bgt60trxx_config(&sensor, register_list, XENSIV_BGT60TRXX_CONF_NUM_REGS); // config
  }
  if (rslt != XENSIV_BGT60TRXX_STATUS_OK)
  {
  goto Error_Assert;
  }
  if (xensiv_bgt60trxx_enable_data_test_mode(&sensor, false) != XENSIV_BGT60TRXX_STATUS_OK) // test mode
  {
  goto Error_Assert;
  }
  if (xensiv_bgt60trxx_set_fifo_limit(&sensor, NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA * 1) != XENSIV_BGT60TRXX_STATUS_OK)
  {
  goto Error_Assert;
  }
  if (xensiv_bgt60trxx_start_frame(&sensor, true) != XENSIV_BGT60TRXX_STATUS_OK)
  {
  goto Error_Assert;
  }
  data_available = false;

  uint8_t headflag[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};


  uint8_t UserTxBuffer[] = "STM32 Virtual COM Port Driver \r\n";
  usb_printf("Hello world, %s\r\n", UserTxBuffer);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 while (1)
 {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
   //wait_time = HAL_GetTick();
   
   /* Wait for the radar device to indicate the availability of the data to fetch. */
   while (data_available == false)
     ;
   data_available = false;
#ifdef DMA_ENABLE
   while (dma_flag == false)
     ;
   dma_flag = false;
   HAL_UART_Transmit(&huart2, headflag, 8, 100);
#else
   // start = clock();

   // AI_Run(aiInData, aiOutData);
 
   // HAL_Delay(1000);
   // CDC_Transmit_FS(headflag, 8);    
   // CDC_Transmit_FS(UserTxBuffer, sizeof(UserTxBuffer));
   // HAL_Delay(1000);
   // usb_printf("Hello world, %s\r\n", UserTxBuffer);
#endif
   //framept = &adcRawData[(frame_idx % NUM_FRAMES_BUFFER) * NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA];
   framept = &adcRawData[0]; // read 9KB each time
   spi_enable_tick = HAL_GetTick();   
   if (xensiv_bgt60trxx_get_fifo_data(&sensor, (uint8_t *)framept,
                                   NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA) == XENSIV_BGT60TRXX_STATUS_OK)// read data
   {
     usb_printf("\r\nFrame %d data collection complete\r\n", frame_idx);
     //resultpt = &resultArray[(frame_idx % 15) * NUM_SAMPLES_PER_FRAME];
     resultpt = &resultArray[0];// output 12KB each time
     BIT12_TO_BIT16(framept, resultpt, NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA);
     HAL_Delay(1);
     usb_printf("Frame %d data convert complete\n", frame_idx);
     
    // Check bit12 to bit16 is right
    // testArray[0] = (resultArray[(frame_idx % NUM_FRAMES_BUFFER) * NUM_SAMPLES_PER_FRAME] >> 8) & 0xff;
    // testArray[1] = resultArray[(frame_idx % NUM_FRAMES_BUFFER) * NUM_SAMPLES_PER_FRAME] & 0xff;
    // testArray[2] = (resultArray[(frame_idx % NUM_FRAMES_BUFFER) * NUM_SAMPLES_PER_FRAME+1] >> 8) & 0xff;
    // testArray[3] = resultArray[(frame_idx % NUM_FRAMES_BUFFER) * NUM_SAMPLES_PER_FRAME+1] & 0xff;
    // CDC_Transmit_FS(headflag, 8); 
    // HAL_Delay(1);
    // CDC_Transmit_FS(testArray, 4); //传输雷达数据
    // HAL_Delay(1);
    // CDC_Transmit_FS(framept, 3); //传输雷达数据

     //input 64x32x3 = 6144, output 6; Repeat 15 times!
     judgept = &resultArray[4096];
     // using frame_stop here to read 15 frames when judge true
    if (frame_stop >= frame_idx + 1 && frame_stop != 0)
    {

     TIM_start = HAL_GetTick();    
     matlab_feature_extract(resultArray, 15 - (frame_stop - frame_idx), matlabOutData);
     TIM_end = HAL_GetTick();
     TIM = TIM_end - TIM_start;
     HAL_Delay(1);
     usb_printf("\r\nMatlab Processing time= %u ms\n", TIM);
     HAL_Delay(1);

     usb_printf("current frame is %d\n", frame_idx);
     HAL_Delay(1);
     usb_printf("%f, %f, %f, %f, %f, %f\r\n",matlabOutData[0],matlabOutData[1],matlabOutData[2],matlabOutData[3],matlabOutData[4],matlabOutData[5]);
     HAL_Delay(1);

		 // Wait for 15 frames
     if (15 - (frame_stop - frame_idx) < 14)
     {
       featuresMatrix[((15 - (frame_stop - frame_idx)) % NUM_FRAMES_BUFFER)] = matlabOutData[0];
       featuresMatrix[((15 - (frame_stop - frame_idx)) % NUM_FRAMES_BUFFER) + NUM_FRAMES_BUFFER] = matlabOutData[1];
       featuresMatrix[((15 - (frame_stop - frame_idx)) % NUM_FRAMES_BUFFER) + NUM_FRAMES_BUFFER*2] = matlabOutData[2];
       featuresMatrix[((15 - (frame_stop - frame_idx)) % NUM_FRAMES_BUFFER) + NUM_FRAMES_BUFFER*3] = matlabOutData[3];
       featuresMatrix[((15 - (frame_stop - frame_idx)) % NUM_FRAMES_BUFFER) + NUM_FRAMES_BUFFER*4] = matlabOutData[4];
       featuresMatrix[((15 - (frame_stop - frame_idx)) % NUM_FRAMES_BUFFER) + NUM_FRAMES_BUFFER*5] = matlabOutData[5];   
     }      
        

    //  if (frame_idx >= 14)
    //  {
     //   // featuresmatrix is fulfilled, do AI processing
			 if ((15 - (frame_stop - frame_idx)) == 14){		 
					 AI_Run(featuresMatrix, aiOutData);					 
				 }
      //  else
      //  {
      //    //float featuresMatrix_new[90];

      //    for (int i = 0; i < 14; i++)// delete the first frame, fifo
      //    {
      //      featuresMatrix[i] = featuresMatrix[i + 1];
      //      featuresMatrix[NUM_FRAMES_BUFFER + i] = featuresMatrix[NUM_FRAMES_BUFFER + i + 1];
      //      featuresMatrix[NUM_FRAMES_BUFFER*2 + i] = featuresMatrix[NUM_FRAMES_BUFFER*2 + i + 1];
      //      featuresMatrix[NUM_FRAMES_BUFFER*3 + i] = featuresMatrix[NUM_FRAMES_BUFFER*3 + i + 1];
      //      featuresMatrix[NUM_FRAMES_BUFFER*4 + i] = featuresMatrix[NUM_FRAMES_BUFFER*4 + i + 1];
      //      featuresMatrix[NUM_FRAMES_BUFFER*5 + i] = featuresMatrix[NUM_FRAMES_BUFFER*5 + i + 1];

      //    }
      //    // add the new frame
      //    featuresMatrix[14] = matlabOutData[0];
      //    featuresMatrix[14 + NUM_FRAMES_BUFFER] = matlabOutData[1];
      //    featuresMatrix[14 + NUM_FRAMES_BUFFER*2] = matlabOutData[2];
      //    featuresMatrix[14 + NUM_FRAMES_BUFFER*3] = matlabOutData[3];
      //    featuresMatrix[14 + NUM_FRAMES_BUFFER*4] = matlabOutData[4];
      //    featuresMatrix[14 + NUM_FRAMES_BUFFER*5] = matlabOutData[5];

      //    AI_Run(featuresMatrix, aiOutData);
      //  }
       
     //}

    } 
    else {
      if (start_judge(judgept)){
        frame_stop = frame_idx + 16;
      }
    }
    


     
     //time measure start
     //bit12 to bit16() -> DONE!
     //matlab feature extract, output 6 
     // for
     // repeat 15 times, output 6 * 15 = 90 -> DONE!
     //
     //Ai_Run() -> DONE!
     //time measure stop

     //fifo next frame, feature -> DONE!
     //Ai_Run()



#ifdef DMA_ENABLE
     printf("wait radar time:%ld ms ", HAL_GetTick() - wait_time);
     printf("spi transfer interval:%ld ms ", spi_dma_cost);
     printf("usart transfer interval:%ld ms ", usart_dma_cost);
     printf("frame repetion time: %d ms  ", (int)(XENSIV_BGT60TRXX_CONF_FRAME_REPETION_TIME_S * 1000 + 0.5));
     printf("radar irq interval:%ld ms\n", irq_tick);
#else
     

     // CDC_Transmit_FS(resultpt, NUM_SAMPLES_PER_FRAME); //传输雷达数据
     // HAL_Delay(5000);
     // if (resultpt == &resultArray[0])
     // {
     //   CDC_Transmit_FS(&resultArray[(NUM_FRAMES_BUFFER - 1) * NUM_SAMPLES_PER_FRAME], NUM_SAMPLES_PER_FRAME);
     // }
     // else
     // {
     //   CDC_Transmit_FS((uint16_t *)(resultpt - NUM_SAMPLES_PER_FRAME), NUM_SAMPLES_PER_FRAME);
     // }
     
     // CDC_Transmit_FS(framept, NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA); //传输雷达数据
     // if (framept == &adcRawData[0])
     // {
     //   CDC_Transmit_FS(&adcRawData[(NUM_FRAMES_BUFFER - 1) * NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA], NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA);
     // }
     // else
     // {
     //   CDC_Transmit_FS((uint8_t *)(framept - NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA), NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA);
     // }
#endif
   }
   else
   {
     usb_printf("Error: xensiv_bgt60trxx_get_fifo_data\n");
     goto Error_Assert;
   }
   frame_idx++;
 }
Error_Assert:
 while (true)
 {
   HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
   HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
   usb_printf("Error_Assert\n");
   HAL_Delay(100);
 };
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 data_available = true;
 irq_tick = HAL_GetTick() - last_irq_tick;
 last_irq_tick = HAL_GetTick();
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
 if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY)
 {
#ifdef DMA_ENABLE
   if (framept == &adcRawData[0])
   {
     HAL_UART_Transmit_DMA(&huart2, &adcRawData[(NUM_FRAMES_BUFFER - 1) * NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA], NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA);
   }
   else
   {
     HAL_UART_Transmit_DMA(&huart2, (uint8_t *)(framept - NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA), NUM_SAMPLES_PER_FRAME_ADC_RAW_DATA);
   }
   dma_flag = true;
   HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, SET);
   usart_dma_start = HAL_GetTick();
#endif
   if (frame_idx % 15 == 0)
   {
     HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
   }
 }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef DMA_ENABLE
 dma_flag_uart = true;
#endif
 if (frame_idx % 15 == 0)
 {
   HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
 }
}

/*void BIT12_TO_BIT16(uint8_t *A, int16_t *B, uint16_t Alen)
{
 uint32_t tmp = 0;
 int16_t *samplesPt = B;
 int16_t i_left_12, i_right_12;
 uint16_t u_left_12, u_right_12;
 for (size_t i = 0; i < Alen; i = i + 3)
 {
   tmp = ((A[i] << 16) | (A[i + 1] << 8) | A[i + 2]);

   u_left_12 = (tmp >> 12) & 0xfff;
   u_right_12 = tmp & 0xfff;

   if (u_left_12 >= 2096) // 0x800
   {
     i_left_12 = u_left_12 | 0xf000;
   }
   else
   {
     i_left_12 = u_left_12;
   }

   if (u_right_12 >= 2096) // 0x800
   {
     i_right_12 = u_right_12 | 0xf000;
   }
   else
   {
     i_right_12 = u_right_12;
   }

   *samplesPt = i_left_12;
   samplesPt++;
   *samplesPt = i_right_12;
   samplesPt++;
 }
}
*/
void BIT12_TO_BIT16(uint8_t *A, uint16_t *B, uint16_t Alen)
{
	uint32_t tmp = 0;
	uint16_t *samplesPt = B;
	for (size_t i = 0; i < Alen; i = i + 3)
	{
		tmp = ((A[i] << 16) | (A[i + 1] << 8) | A[i + 2]);
		*samplesPt = ((tmp >> 12) & 0x0fff);
		samplesPt++;
		*samplesPt = tmp & 0x0fff;
		samplesPt++;
	}
}





/**
 * @brief AI Model Initialization Function
 * @param None
 * @retval None
 */
static void AI_Init(void)
{
 ai_error err;

 /* Create a local array with the addresses of the activations buffers */
 const ai_handle act_addr[] = { activations };
 /* Create an instance of the model */
 err = ai_network_create_and_init(&network, act_addr, NULL);
 if (err.type != AI_ERROR_NONE) {
   usb_printf("ai_network_create error - type=%d code=%d\r\n", err.type, err.code);
   Error_Handler();
 }
	else{
		usb_printf("Model Init Successfully\r\n");
	}
 ai_input = ai_network_inputs_get(network, NULL);
 ai_output = ai_network_outputs_get(network, NULL);
}

/**
 * @brief AI Model Run Function
 * @param pIn, pOut
 * @retval None
 */
static void AI_Run(float *pIn, float *pOut)
{
	char logStr[100];
	int count = 0;
	float max = 0;
 ai_i32 batch;
 ai_error err;

 /* Update IO handlers with the data payload */
 ai_input[0].data = AI_HANDLE_PTR(pIn);
 ai_output[0].data = AI_HANDLE_PTR(pOut);

 TIM_start = HAL_GetTick();   
 // AI Model Processing
 batch = ai_network_run(network, ai_input, ai_output);
 TIM_end = HAL_GetTick();
 TIM = TIM_end - TIM_start;
 
 if (batch != 1) {
   err = ai_network_get_error(network);
   usb_printf("AI ai_network_run error - type=%d code=%d\r\n", err.type, err.code);
   Error_Handler();
 }
	else{
		usb_printf("Model Processing Successfully\nModel Processing time=%ums\r\n", TIM);
	}
 // Softmax: Calculate output weights to probabilities
	float softmax = exp(aiOutData[0])+exp(aiOutData[1])+exp(aiOutData[2])+exp(aiOutData[3])+exp(aiOutData[4])+exp(aiOutData[5]);
	//printf("%8.6f %8.6f\r\n",aiInData[0], aiOutData[0]); 
 
 // Print probability for each kind & given model predict result
 for (uint32_t i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {

     sprintf(logStr,"gesture %d  %8.6f\r\n",i,exp(aiOutData[i])/softmax);
     usb_printf("%s\r\n",logStr);
     HAL_Delay(1);
     if(max<aiOutData[i])
     {
       count = i;
       max= aiOutData[i];
     }
 }
 switch (count)
 {
 case 0:
   sprintf(logStr,"当前手势为 前\r\n");
   usb_printf("%s\r\n",logStr);
   break;
 case 1:
   sprintf(logStr,"当前手势为 后\r\n");
   usb_printf("%s\r\n",logStr);
   break;
 case 2:
   sprintf(logStr,"当前手势为 左\r\n");
   usb_printf("%s\r\n",logStr);
   break;
 case 3:
   sprintf(logStr,"当前手势为 右\r\n");
   usb_printf("%s\r\n",logStr);
   break;
 case 4:
   sprintf(logStr,"当前手势为 上\r\n");
   usb_printf("%s\r\n",logStr);
   break;
 case 5:
   sprintf(logStr,"当前手势为 下\r\n");
   usb_printf("%s\r\n",logStr);
   break;
 }
 // sprintf(logStr,"current number is gesture %d\r\n",count);
 // usb_printf("%s\r\n",logStr);
 
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

#ifdef  USE_FULL_ASSERT
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
