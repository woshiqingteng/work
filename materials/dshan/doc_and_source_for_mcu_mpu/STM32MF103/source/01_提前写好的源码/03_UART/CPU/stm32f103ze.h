#ifndef __STM32F103ZE_H
#define __STM32F103ZE_H

/* exact-width signed integer types */
typedef   signed           char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;

/* exact-width unsigned integer types */
typedef unsigned           char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

/* 外设内存地址映射 */
#define PERIPH_BASE           0x40000000UL

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)

/****************** GPIO Start ********************/
/****************** GPIO Start ********************/
// GPIO 状态
typedef enum
{
  GPIO_PIN_RESET = 0u,
  GPIO_PIN_SET
} GPIO_PinState;

// GPIO 寄存器
typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;    
    volatile uint32_t IDR;
    volatile uint32_t ODR;    
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
}GPIO_TypeDef;

// GPIO 组基地址
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x00000800UL)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x00000C00UL)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x00001000UL)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x00001400UL)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x00001800UL)
#define GPIOF_BASE            (APB2PERIPH_BASE + 0x00001C00UL)
#define GPIOG_BASE            (APB2PERIPH_BASE + 0x00002000UL)

// GPIO 引脚号
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0  */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1  */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2  */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3  */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4  */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5  */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6  */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7  */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8  */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9  */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins */

// GPIO组 寄存器结构体对象
#define GPIOA               ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *)GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *)GPIOG_BASE)
/****************** GPIO END ********************/
/****************** GPIO END ********************/

/****************** RCC Start ********************/
/****************** RCC Start ********************/
// RCC 寄存器
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

// RCC 基地址
#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)

// RCC 寄存器结构体对象
#define RCC                 ((RCC_TypeDef *)RCC_BASE)

/****************** RCC END ********************/
/****************** RCC END ********************/

/****************** FLASH Start ********************/
/****************** FLASH Start ********************/
typedef struct
{
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
} FLASH_TypeDef;

#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000UL) /*!< Flash registers base address */
#define FLASH               ((FLASH_TypeDef *)FLASH_R_BASE)
/****************** FLASH END ********************/
/****************** FLASH END ********************/

/****************** UART Start ********************/
/****************** UART Start ********************/
typedef struct
{
  volatile uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  volatile uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  volatile uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  volatile uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  volatile uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  volatile uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  volatile uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800UL)

#define USART1              ((USART_TypeDef *)USART1_BASE)
/****************** UART END ********************/
/****************** UART END ********************/

#endif