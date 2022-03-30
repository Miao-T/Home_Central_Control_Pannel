////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_ADC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE ADC FIRMWARE FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _HAL_ADC_C_

// Files includes  -------------------------------------------------------------
#include "hal_adc.h"
#include "hal_rcc.h"
#include "common.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Deinitializes the ADCn peripheral registers to their default
///   		reset values.
/// @param 	ADCn: where n can be 1, 2  to select the ADC peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_DeInit(ADC_TypeDef* ADCn)
{
#if !defined(__MM0T1)
    switch (*(u32*)&ADCn) {
        case ADC1_BASE: exRCC_APB2PeriphReset(RCC_APB2ENR_ADC1); break;
#if defined(__MM3N1)
        case ADC2_BASE: exRCC_APB2PeriphReset(RCC_APB2ENR_ADC2); break;
#endif
        default: break;
    }
#else
    exRCC_APB1PeriphReset(RCC_APB1ENR_ADC1);
#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the ADCn peripheral according to the specified parameters
///			in the pInitStruct, Please use this function if you want to be
///			compatible with older versions of the library.
/// @param 	ADCn: where n can be 1, 2  to select the ADC peripheral.
/// @param 	pInitStruct: pointer to an ADC_InitTypeDef structure that contains
///			the configuration information for the specified ADC peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_Init(ADC_TypeDef* ADCn, ADC_InitTypeDef* pInitStruct)
{
    ADCn->CFGR &= ~(ADC_CFGR_PRE | ADC_CFGR_RSLTCTL);
    ADCn->CFGR |= (u32)(pInitStruct->ADC_PRESCARE) | pInitStruct->ADC_Resolution;

    ADCn->CR &= ~(ADC_CR_ALIGN | ADC_CR_MODE | ADC_CR_TRGSEL);
    ADCn->CR |= ((u32)pInitStruct->ADC_DataAlign) | pInitStruct->ADC_ExternalTrigConv | ((u32)pInitStruct->ADC_Mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Fills each pInitStruct member with its default value.
/// @param 	pInitStruct : pointer to an ADC_InitTypeDef structure which will be
///			initialized.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_StructInit(ADC_InitTypeDef* pInitStruct)
{
    pInitStruct->ADC_Resolution         = ADC_Resolution_12b;
    pInitStruct->ADC_PRESCARE           = ADC_PCLK2_PRESCARE_2;
    pInitStruct->ADC_Mode               = ADC_CR_IMM;		//ADC_Mode_Single;
    pInitStruct->ADC_ContinuousConvMode = DISABLE;  // useless
    pInitStruct->ADC_ExternalTrigConv   = ADC1_ExternalTrigConv_T1_CC1;
    pInitStruct->ADC_DataAlign          = ADC_DataAlign_Right;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified ADC peripheral.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	state: new state of the ADCn peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_Cmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->CFGR |= ADC_CFGR_ADEN) : (ADCn->CFGR &= ~ADC_CFGR_ADEN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified ADC DMA request.
/// @param 	ADCn: where n can be 1 or 2 to select the ADC peripheral.
/// @param 	state: New state of the selected ADC DMA transfer.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if !defined(__MM0T1)
void ADC_DMACmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->CR |= ADC_CR_DMAEN) : (ADCn->CR &= ~ADC_CR_DMAEN);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified ADC interrupts.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	ADC_IT: specifies the ADC interrupt sources to be enabled or disabled.
/// @param 	state: New state of the specified ADC interrupts.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ITConfig(ADC_TypeDef* ADCn, ADCFLAG_TypeDef ADC_IT, FunctionalState state)
{
    if (ADC_IT == ADC_IT_EOC)
        (state) ? (ADCn->CR |= ADC_CR_ADIE) : (ADCn->CR &= ~ADC_CR_ADIE);
    else
        (state) ? (ADCn->CR |= ADC_CR_ADWIE) : (ADCn->CR &= ~ADC_CR_ADWIE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the selected ADC software start conversion .
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	state: New state of the selected ADC software start conversion.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->CR |= ADC_CR_ADST) : (ADCn->CR &= ~ADC_CR_ADST);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the selected ADC Software start conversion Status.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @retval  The new state of ADC software start conversion (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCn)
{
    return (((ADCn->CR & ADC_CR_ADST) != (u32)RESET) ? SET : RESET);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the selected ADC channel and configure its sample time. Please
///			use this function if you want to be compatible with older versions
///			of the library.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	channel: the ADC channel to configure.
/// @param 	sampleTime: the ADC Channel n Sample time to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_RegularChannelConfig(ADC_TypeDef* ADCn, u32 channel, u8 rank, u16 sampleTime)
{
#if defined(__MM3U1)
    u32 tempchan;
    sampleTime = (u16)sampleTime & 0xF;
    tempchan = channel;
    if(tempchan > 8) {
        tempchan = tempchan & 0xF;
        tempchan = tempchan - 8;
        ADCn->SMPR2 &= ~(0xF << tempchan);
        ADCn->SMPR2 |= (sampleTime << tempchan);
    }
    else {
        ADCn->SMPR1 &= ~(0xF << tempchan);
        ADCn->SMPR1 |= (sampleTime << tempchan);
    }
#else
    ADCn->CFGR &= ~ADC_CFGR_SAMCTL;
    ADCn->CFGR |= sampleTime;
#if !defined(__MM0T1)
	ADCn->CHSR &= ~(1 << channel);
	ADCn->CHSR |=  (1 << channel);
#endif

#if defined(__MM3N1)
    if (channel & ADC_CHSR_CHTV)
        ADC_TempSensorVrefintCmd(ENABLE);
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)  || defined(__MM0S1)
    if (channel & ADC_CHSR_CHT)
        ADC_TempSensorVrefintCmd(ENABLE);
    else if (channel & ADC_CHSR_CHV)
        ADC_TempSensorVrefintCmd(ENABLE);
#endif
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the ADCn conversion through external trigger.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	state: New state of the selected ADC external trigger.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->CR |= ADC_CR_TRGEN) : (ADCn->CR &= ~ADC_CR_TRGEN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the last ADCn conversion result data for regular channel.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @retval The data conversion value.
////////////////////////////////////////////////////////////////////////////////
u32 ADC_GetConversionValue(ADC_TypeDef* ADCn)
{
    return ADCn->DR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief 	Returns the last ADC conversion result data in dual mode.
/// @param  None
/// @retval The Data conversion value.
////////////////////////////////////////////////////////////////////////////////
u32 ADC_GetDualModeConversionValue()
{
    return (*(u32*)ADC1_BASE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the ADCn external trigger for injected channels conversion.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	ADC_ExternalTrigInjecConv: Configuring the external trigger source
///			for the ADC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCn, EXTERTRIG_TypeDef ADC_ExternalTrigInjecConv)
{
    ADCn->CR &= ~ADC_CR_TRGSEL;
    ADCn->CR |=  ADC_ExternalTrigInjecConv;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the ADCn injected channels conversion through
///			external trigger
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	state: New state of the selected ADC external trigger start of
///			external trigger conversion.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->CR |= ADC_CR_TRGEN) : (ADCn->CR &= ~ADC_CR_TRGEN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the analog watchdog.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	state: New state of the selected ADC analog watchdog.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->CFGR |= ADC_CFGR_ADWEN) : (ADCn->CFGR &= ~ADC_CFGR_ADWEN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the high and low thresholds of the analog watchdog.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	HighThreshold: the ADC analog watchdog High threshold value.
///   		This parameter must be a 12bit value.
/// @param 	LowThreshold: the ADC analog watchdog Low threshold value.
///   		This parameter must be a 12bit value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCn, u16 HighThreshold, u16 LowThreshold)
{
    u32 tempThreshold;
    tempThreshold = HighThreshold;
    ADCn->CMPR    = (tempThreshold << 16) | LowThreshold;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the analog watchdog guarded single channel
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	channel: the ADC channel to configure for the analog watchdog.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCn, ADCCHANNEL_TypeDef channel)
{
    ADCn->CR &= ~ADC_CR_CMPCH;
    ADCn->CR |= (channel << ADC_CR_CMPCH_Pos);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the temperature sensor and Vrefint channel.
/// @param 	state: New state of the temperature sensor.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_TempSensorVrefintCmd(FunctionalState state)
{
#if defined(__MM3N1)
    (state) ? (ADC1->CFGR |=   ADC_CFGR_TVEN,  ADC2->CFGR |=  ADC_CFGR_TVEN)
            : (ADC1->CFGR &=  ~ADC_CFGR_TVEN,  ADC2->CFGR &= ~ADC_CFGR_TVEN);
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
    (state) ? (ADC1->CFGR |=  (ADC_CFGR_TEN | ADC_CFGR_VEN))
		    : (ADC1->CFGR &= ~(ADC_CFGR_TEN | ADC_CFGR_VEN));
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the temperature sensor and Vrefint channel.
/// @param 	chs: temperature sensor bit & Vrefint bit.
/// @param 	state: New state of the temperature sensor.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_TempSensorCmd(FunctionalState state)
{
    ADC_TempSensorVrefintCmd(state);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the Vrefint channel.
/// @param  state: New state of the Vrefint channel.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_VrefintCmd(FunctionalState state)
{
    ADC_TempSensorVrefintCmd(state);
}
void exADC_TempSensorVrefintCmd(u32 chs, FunctionalState state)
{
#if defined(__MM3N1)
	if (chs & ADC_CHSR_CHTV) {
		(state) ? (ADC1->CFGR |=   ADC_CFGR_TVEN,  ADC2->CFGR |=  ADC_CFGR_TVEN)
				: (ADC1->CFGR &=  ~ADC_CFGR_TVEN,  ADC2->CFGR &= ~ADC_CFGR_TVEN);
	}
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
	if (chs & ADC_CHSR_CHT) {
		(state) ? (ADC1->CFGR |=  ADC_CFGR_TEN)
				: (ADC1->CFGR &= ~ADC_CFGR_TEN);
	}
	else if (chs & ADC_CHSR_CHV) {
		(state) ? (ADC1->CFGR |=  ADC_CFGR_VEN)
				: (ADC1->CFGR &= ~ADC_CFGR_VEN);
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified ADC flag is set or not.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	ADC_FLAG: specifies the flag to check.
/// @retval  The New state of ADC_FLAG (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCn, ADCFLAG_TypeDef ADC_FLAG)
{
    return (ADC_FLAG == ADC_IT_EOC) ? ((ADCn->SR & ADC_SR_ADIF) ? SET : RESET) : ((ADCn->SR & ADC_SR_ADWIF) ? SET : RESET);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the ADCn's pending flags.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	ADC_FLAG: specifies the flag to clear.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ClearFlag(ADC_TypeDef* ADCn, ADCFLAG_TypeDef ADC_FLAG)
{
    (ADC_FLAG == ADC_IT_EOC) ? (ADCn->SR |= ADC_SR_ADIF) : (ADCn->SR |= ADC_SR_ADWIF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified ADCn's interrupt has occurred or not.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	ADC_IT: specifies the ADC interrupt source to check.
/// @retval  The new state of ADC_IT (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCn, ADCFLAG_TypeDef ADC_IT)
{
    return (ADC_IT == ADC_IT_EOC) ? ((ADCn->SR & ADC_SR_ADIF) ? SET : RESET) : ((ADCn->SR & ADC_SR_ADWIF) ? SET : RESET);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the ADCn's interrupt pending bits.
/// @param 	ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param 	ADC_IT: specifies the ADC interrupt pending bit to clear.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ClearITPendingBit(ADC_TypeDef* ADCn, ADCFLAG_TypeDef ADC_IT)
{
    (ADC_IT == ADC_IT_EOC) ? (ADCn->SR |= ADC_SR_ADIF) : (ADCn->SR |= ADC_SR_ADWIF);
}

#if defined(__MM0T1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the adc any channels conversion rank and channel.
/// @param  ADCn: select the ADC peripheral.
/// @param  rank: rank can be 0x0~0xf for the convert sequence.
/// @param  adc_channel: Configuring the target channel to be converted.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ANY_CH_Config(ADC_TypeDef* ADCn, u8 rank, ADCCHANNEL_TypeDef adc_channel)
{
    rank = rank & 0xF;
    if(rank < 8) {
        ADCn->CHANY0 &= ~(0x0F << (4 * rank));
        ADCn->CHANY0 |= (adc_channel << (4 * rank));
    }
    else {
        ADCn->CHANY1 &= ~(0x0F << (4 * (rank - 8)));
        ADCn->CHANY1 |= (adc_channel << (4 * (rank - 8)));
    }

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the adc any channels conversion Max rank number
/// @param  ADCn: select the ADC peripheral.
/// @param  num: Configuring the max rank number for the ADC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ANY_NUM_Config(ADC_TypeDef* ADCn, u8 num)
{
    if(num > 15) num = 15;                                                      //15 ? 16 need to be confirmed
    ADCn->ANY_CFG = num;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the ANY channel converter.
/// @param  state: enable or disable the ANY channel converter mode.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ANY_Cmd(ADC_TypeDef* ADCn, FunctionalState state)
{
    (state) ? (ADCn->ANY_CR |= ADC_ANY_CR_CHANY_MDEN) : (ADCn->ANY_CR &= ~ADC_ANY_CR_CHANY_MDEN);
}
#endif

#if defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the selected ADC channel and configure its sample time. Please
///         use this function if you want to be compatible with older versions
///         of the library.
/// @param  ADCn:  select the ADC peripheral.
/// @param  event: the ADC external event to configure.
/// @param  sampleTime: the ADC Channel n Sample time to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_InjectedSequencerConfig(ADC_TypeDef* ADCn, u32 event, u32 sampleTime)
{
    ADCn->ANY_CR &= ~(ADC_ANY_CR_JCEN | ADC_ANY_CR_CHANY_MDEN | ADC_ANY_CR_JTRGSEL_7 \
                    | ADC_ANY_CR_JTRGSHIFT_7 | ADC_ANY_CR_JTRGEN);
    ADCn->ANY_CR |= (ADC_ANY_CR_JCEN | ADC_ANY_CR_CHANY_MDEN | sampleTime | event | ADC_ANY_CR_JTRGEN);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Injection channel length configuration.
/// @param  adc:  select the ADC peripheral.
/// @param  Length: Injection channel length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCn, uint8_t Length)
{
    ADCn->JSQR &= ~(0x03 << ADC_JSQR_JL_Pos);
    ADCn->JSQR |= Length << ADC_JSQR_JL_Pos;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Injection channel  configuration.
/// @param  adc  :   select the ADC peripheral.
/// @param  jqsn :   Injection channel.
/// @param  channel: The sampling channel.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_InjectedSequencerChannelConfig(ADC_TypeDef* ADCn, uint8_t jqsn, uint8_t channel)
{
    ADCn->JSQR &= ~(0x1F << jqsn * 5);
    ADCn->JSQR |= (channel << jqsn * 5);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Injection channel  configuration.
/// @param  adc  :   select the ADC peripheral.
/// @param  jqsn :   Injection channel.
/// @param  channel: The sampling channel.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u32 ADC_InjectedSequencerGetConversionValue(ADC_TypeDef* ADCn, uint8_t jqsn)
{
    u32 value = 0;
    switch(jqsn) {
        case 0 :
            value = (u32)ADCn->JDR0;
            value -= ADCn->JOFR0;
            break;
        case 1 :
            value = (u32)ADCn->JDR1;
            value -= ADCn->JOFR1;
            break;
        case 2 :
            value = (u32)ADCn->JDR2;
            value -= ADCn->JOFR2;
            break;
        case 3 :
            value = (u32)ADCn->JDR3;
            value -= ADCn->JOFR3;
            break;
        default :
            break;
    }
    return value & 0xFFF;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Injection channel compensation configuration.
/// @param  adc  :   select the ADC peripheral.
/// @param  jqsn :   Injection channel.
/// @param  value: compensation value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_InjectedSequencerSetJQFR(ADC_TypeDef* ADCn, uint8_t jqsn, uint8_t value)
{
    *(u32*)(*(u32*)&ADCn + 0x7C + jqsn * 4) = value;
}
#endif



////////////////////////////////////////////////////////////////////////////////
/// @brief  Get channel convertion result.
/// @param  adc  :   select the ADC peripheral.
/// @param  channel :   Converted channel.
/// @retval The Data conversion value.
////////////////////////////////////////////////////////////////////////////////
u32 ADC_GetChannelConvertedValue(ADC_TypeDef* ADCn, ADCCHANNEL_TypeDef channel)
{
    return (*(u32*) ((u32)ADCn + 0x18 + 4 * ((u32)channel)));
}
/// @}

/// @}

/// @}
