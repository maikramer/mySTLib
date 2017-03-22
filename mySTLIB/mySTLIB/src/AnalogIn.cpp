//=================================================================================//
//	Arquivo : AnalogIn.cpp
//	Projeto : STM32LIB
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descrição:
//=================================================================================//
//	This file is part of IntTeste
//	IntTeste is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=================================================================================//

//=================================================================================//
//	Includes
//=================================================================================//

#include <AnalogIn.h>
#include <diag/Trace.h>

//=================================================================================//
//	Definição dos Métodos
//=================================================================================//

uint8_t AnalogIn::nAnalogIn;
uint16_t AnalogIn::_value[6];

AnalogIn::AnalogIn(uint16_t pin) :
		_pin(pin), _port(GPIOC) {

	if (_pin == GPIO_Pin_0) {
		_channel = ADC_Channel_10;
	} else if (_pin == GPIO_Pin_1) {
		_channel = ADC_Channel_11;
	} else if (_pin == GPIO_Pin_2) {
		_channel = ADC_Channel_12;
	} else if (_pin == GPIO_Pin_3) {
		_channel = ADC_Channel_13;
	} else if (_pin == GPIO_Pin_4) {
		_channel = ADC_Channel_14;
	} else if (_pin == GPIO_Pin_5) {
		_channel = ADC_Channel_15;
	} else {
		_channel = 0;
	}
	_valueIndex = nAnalogIn;
	nAnalogIn++;

	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable ADCx, DMA and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* DMA2 Stream0 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)_value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = nAnalogIn;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC1 Channel7 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = _pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(_port, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = nAnalogIn;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, _channel, 1, ADC_SampleTime_112Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv (ADC1);
}

AnalogIn::~AnalogIn() {
#ifdef DEBUG
	trace_puts(" AnalogIn Deletado");
#endif
}

uint16_t AnalogIn::read_mV() {
	/*
	 * 	O ADC Retorna 0 para 0V e 4096(12 Bits) para VCC
	 * 	Assumindo VCC = 3300mV(3,3V) temos :
	 * 	valor * (3300/4096) = valor * 0.8056
	 */
	uint16_t ret = (uint16_t) (_value[_valueIndex] * 0.8056f);
	return ret;
}

uint8_t AnalogIn::read() {
	/*
	 * O ADC Retorna um valor de 0 a 4096(12 Bits) logo temos :
	 * 	(valor / 4096) * 100 ou valor * 0,0244
	 */
	uint8_t ret = (uint8_t) (_value[_valueIndex] * 0.0244f);
	return ret;
}
