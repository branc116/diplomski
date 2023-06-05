#include "SensorTile_BlueNRG.h"
//#include "gp_timer.h"

#define HEADER_SIZE 5
#define MAX_BUFFER_SIZE 255
#define TIMEOUT_DURATION 15

SPI_HandleTypeDef SpiHandle;

void us150Delay(void);


/**
* @brief  This function is used for low level initialization of the SPI
*         communication with the BlueNRG Expansion Board.
* @param  hspi: SPI handle.
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==BNRG_SPI_INSTANCE)
  {
    /* Enable peripherals clock */

    /* Enable GPIO Ports Clock */
    BNRG_SPI_RESET_CLK_ENABLE();
    BNRG_SPI_SCLK_CLK_ENABLE();
    BNRG_SPI_MISO_CLK_ENABLE();
    BNRG_SPI_MOSI_CLK_ENABLE();
    BNRG_SPI_CS_CLK_ENABLE();
    BNRG_SPI_IRQ_CLK_ENABLE();

    /* Enable SPI clock */
    BNRG_SPI_CLK_ENABLE();

    /* Reset */
    GPIO_InitStruct.Pin = BNRG_SPI_RESET_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_RESET_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_RESET_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_RESET_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_RESET_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_RESET_PORT, &GPIO_InitStruct);	
    HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);	/*Added to avoid spurious interrupt from the BlueNRG */

    /* SCLK */
    GPIO_InitStruct.Pin = BNRG_SPI_SCLK_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_SCLK_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_SCLK_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_SCLK_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_SCLK_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_SCLK_PORT, &GPIO_InitStruct);

    /* MISO */
    GPIO_InitStruct.Pin = BNRG_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MISO_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MISO_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MISO_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MISO_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MISO_PORT, &GPIO_InitStruct);

    /* MOSI */
    GPIO_InitStruct.Pin = BNRG_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MOSI_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MOSI_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MOSI_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MOSI_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MOSI_PORT, &GPIO_InitStruct);

    /* NSS/CSN/CS */
    GPIO_InitStruct.Pin = BNRG_SPI_CS_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_CS_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_CS_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_CS_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_CS_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_CS_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);

    /* IRQ -- INPUT */
    GPIO_InitStruct.Pin = BNRG_SPI_IRQ_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_IRQ_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_IRQ_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_IRQ_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_IRQ_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStruct);

    HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStruct);

    /* Configure the NVIC for SPI */
    HAL_NVIC_SetPriority(BNRG_SPI_EXTI_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
  }
}

/**
* @brief  Initializes the SPI communication with the BlueNRG
*         Expansion Board.
*/
void BNRG_SPI_Init(void)
{
  SpiHandle.Instance = BNRG_SPI_INSTANCE;
  SpiHandle.Init.Mode = BNRG_SPI_MODE;
  SpiHandle.Init.Direction = BNRG_SPI_DIRECTION;
  SpiHandle.Init.DataSize = BNRG_SPI_DATASIZE;
  SpiHandle.Init.CLKPolarity = BNRG_SPI_CLKPOLARITY;
  SpiHandle.Init.CLKPhase = BNRG_SPI_CLKPHASE;
  SpiHandle.Init.NSS = BNRG_SPI_NSS;
  SpiHandle.Init.FirstBit = BNRG_SPI_FIRSTBIT;
  SpiHandle.Init.TIMode = BNRG_SPI_TIMODE;
  SpiHandle.Init.CRCPolynomial = BNRG_SPI_CRCPOLYNOMIAL;
  SpiHandle.Init.BaudRatePrescaler = BNRG_SPI_BAUDRATEPRESCALER;
  SpiHandle.Init.CRCCalculation = BNRG_SPI_CRCCALCULATION;

  HAL_SPI_MspInit(&SpiHandle);
  HAL_SPI_Init(&SpiHandle);
}

/**
* @brief  Resets the BlueNRG.
*/
void BlueNRG_RST(void)
{
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
}

/* end BlueNRG_DataPresent() */

/**
* @brief  Activate internal bootloader using pin.
*/
void BlueNRG_HW_Bootloader(void)
{
  Disable_SPI_IRQ();
  BlueNRG_RST();
  Enable_SPI_IRQ();
}

/**
* @brief  Reads from BlueNRG SPI buffer and store data into local buffer.
* @param  hspi     : SPI handle
* @param  buffer   : Buffer where data from SPI are stored
* @param  buff_size: Buffer size
* @retval int32_t  : Number of read bytes
*/
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer,
                             uint16_t buff_size)
{
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char;

  uint8_t header_master[HEADER_SIZE] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[HEADER_SIZE];

  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);

  /* Read the header */
  HAL_SPI_TransmitReceive(&SpiHandle, header_master, header_slave, HEADER_SIZE, TIMEOUT_DURATION);

  if (header_slave[0] == 0x02) {
    /* device is ready */
    byte_count = (header_slave[4]<<8)|header_slave[3];
    if (byte_count > 0) {

      /* avoid to read more data that size of the buffer */
      if (byte_count > buff_size){
        byte_count = buff_size;
      }

      for (len = 0; len < byte_count; len++){
        __disable_irq();
        HAL_SPI_TransmitReceive(&SpiHandle, &char_ff, (uint8_t*)&read_char, 1, TIMEOUT_DURATION);
        __enable_irq();
        buffer[len] = read_char;
      }
    }
  }
  /* Release CS line */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);

  // Add a small delay to give time to the BlueNRG to set the IRQ pin low
  // to avoid a useless SPI read at the end of the transaction
  for(volatile int i = 0; i < 2; i++)__NOP();

  return len;
}


static int blue_number_of_writes = 0;
int32_t BlueNRG_Write(uint8_t* data, uint16_t size)
{
  blue_number_of_writes++;
  int32_t result = 0;

  unsigned char header_master[HEADER_SIZE] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[HEADER_SIZE]  = {0xaa, 0x00, 0x00, 0x00, 0x00};

  unsigned char read_char_buf[MAX_BUFFER_SIZE];

  /*
  If the SPI_FIX is enabled the IRQ is set in Output mode, then it is pulled
  high and, after a delay of at least 112us, the CS line is asserted and the
  header transmit/receive operations are started.
  After these transmit/receive operations the IRQ is reset in input mode.
  */

  /* Assert CS line after at least 112us */
  //us150Delay();

  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);

  /* Exchange header */
  __disable_irq();
  result = HAL_SPI_TransmitReceive(&SpiHandle, header_master, header_slave, HEADER_SIZE, TIMEOUT_DURATION);
  __enable_irq();


  if (header_slave[0] == 0x02) {
    /* SPI is ready */
    if (header_slave[1] >= (size)) {
      /*  Buffer is big enough */
      if (size > 0) {
        __disable_irq();
        result = HAL_SPI_TransmitReceive(&SpiHandle, data, read_char_buf, size, TIMEOUT_DURATION);
        __enable_irq();
      }
    } else {
      /* Buffer is too small */
      result = -2;
    }
  } else {
    /* SPI is not ready */
    result = -1;
  }

  /* Release CS line */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);

  Clear_SPI_EXTI_Flag();

  return result;
}

/**
* @brief  Utility function for delay
* NOTE: TODO: implement with clock-independent function.
*/
void us150Delay(void)
{
#if SYSCLK_FREQ == 4000000
  for(volatile int i = 0; i < 35; i++)__NOP();
#elif SYSCLK_FREQ == 32000000
  for(volatile int i = 0; i < 420; i++)__NOP();
#elif SYSCLK_FREQ == 80000000
  for(volatile int i = 0; i < 1072; i++)__NOP();
#elif SYSCLK_FREQ == 84000000
  for(volatile int i = 0; i < 1125; i++)__NOP();
#elif SYSCLK_FREQ == 168000000
  for(volatile int i = 0; i < 2250; i++)__NOP();
#else
#error Implement delay function.
#endif
}

/**
* @brief  Enable SPI IRQ.
*/
void Enable_SPI_IRQ(void)
{
  HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
* @brief  Disable SPI IRQ.
*/
void Disable_SPI_IRQ(void)
{
  HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
* @brief  Clear Pending SPI IRQ.
*/
void Clear_SPI_IRQ(void)
{
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
* @brief  Clear EXTI (External Interrupt) line for SPI IRQ.
*/
void Clear_SPI_EXTI_Flag(void)
{
  __HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);
}

