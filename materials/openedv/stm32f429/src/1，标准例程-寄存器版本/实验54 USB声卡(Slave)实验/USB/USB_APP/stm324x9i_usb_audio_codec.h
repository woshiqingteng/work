/**
  ******************************************************************************
  * @file    STM324x9i_usb_eval_audio_codec.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-June-2015
  * @brief   This file contains all the functions prototypes for the 
  *          STM324x9i_eval_audio_codec.c driver.
  ******************************************************************************
  *     *******************************************************************  
  *                           IMPORTANT NOTES
  *     *******************************************************************  
  *     This file is extracted and modified from the audio codec driver
  *     "stm324x9i_eval_audio_codec.c" provided by STMicroelectronics for
  *     the STM32F42x family (in folder Utilities\STM32_EVAL\STM324x9I_EVAL).
  *     This modified driver is intended for use ONLY in the scope of the
  *     USB Device Audio Example.
  *        
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324x9I_EVAL_AUDIOCODEC_H
#define __STM324x9I_EVAL_AUDIOCODEC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h" 
 
 


/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))


/*----------------------------------------------------------------------------
                        Audio Codec User defines
 -----------------------------------------------------------------------------*/
/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
                                       
#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1

/* Codec POWER DOWN modes */
#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2
                                       
/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0


uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint32_t EVAL_AUDIO_DeInit(void);
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size);
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd);
uint32_t EVAL_AUDIO_Stop(uint32_t CodecPowerDown_Mode);
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume);
uint32_t EVAL_AUDIO_Mute(uint32_t Command);
void Audio_MAL_IRQHandler(void);
void     Audio_MAL_Play(uint32_t Addr, uint32_t Size);
/* User Callbacks: user has to implement these functions in his code if
  they are needed. -----------------------------------------------------------*/

/* This function is called when the requested data has been completely transferred.
   In Normal mode (when  the define AUDIO_MAL_MODE_NORMAL is enabled) this function
   is called at the end of the whole audio file.
   In circular mode (when  the define AUDIO_MAL_MODE_CIRCULAR is enabled) this 
   function is called at the end of the current buffer transmission. */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size);

/* This function is called when half of the requested buffer has been transferred 
   This callback is useful in Circular mode only (when AUDIO_MAL_MODE_CIRCULAR 
   define is enabled)*/
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void EVAL_AUDIO_Error_CallBack(void* pData);

/* Codec_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...) on the Codec control interface (I2C).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm324x9i_eval_audio_codec.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t Codec_TIMEOUT_UserCallback(void);
 
#endif /* __STM324x9i_EVAL_AUDIOCODEC_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */    

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
