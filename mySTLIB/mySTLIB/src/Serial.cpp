//=================================================================================//
//	Arquivo : Serial.cpp
//	Projeto : mySTLIB
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão:
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

//==================================================================================//
//	Includes
//==================================================================================//

#include <Serial.h>
#include <diag/Trace.h>

//=================================================================================//
//	Definição dos Métodos
//=================================================================================//

Serial::Serial() {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/* Peripheral Clock Enable -------------------------------------------------*/
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	USARTx_CLK_INIT(USARTx_CLK, ENABLE);

	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(USARTx_DMAx_CLK, ENABLE);

	/* USARTx GPIO configuration -----------------------------------------------*/
	/* Connect USART pins to AF7 */
	GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
	GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
	GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
	GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USARTx configuration ----------------------------------------------------*/
	/* Enable the USART OverSampling by 8 */
	USART_OverSampling8Cmd(USARTx, ENABLE);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* When using Parity the word length must be configured to 9 bits */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);

	/* Configure DMA controller to manage USART TX and RX DMA request ----------*/

	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = serBUFFERSIZE;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USARTx->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Channel = USARTx_TX_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) txBuffer;
	DMA_Init(USARTx_TX_DMA_STREAM, &DMA_InitStructure);
	/* Configure RX DMA */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Channel = USARTx_RX_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) rxBuffer;
	DMA_Init(USARTx_RX_DMA_STREAM, &DMA_InitStructure);

	/* Enable USART */
	USART_Cmd(USARTx, ENABLE);

	/* Enable DMA USART RX Stream */
	DMA_Cmd(USARTx_RX_DMA_STREAM, ENABLE);

	/* Enable USART DMA RX Requests */
	USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);

}

void Serial::send(const char *data) {
	strcpy(txBuffer, data);

	/* Enable DMA USART TX Stream */
	DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);

	/* Enable USART DMA TX Requsts */
	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

	/* Waiting the end of Data transfer */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
		;
	while (DMA_GetFlagStatus(USARTx_TX_DMA_STREAM, USARTx_TX_DMA_FLAG_TCIF)
			== RESET)
		;

	/* Clear DMA Transfer Complete Flags */
	DMA_ClearFlag(USARTx_TX_DMA_STREAM, USARTx_TX_DMA_FLAG_TCIF);
	/* Clear USART Transfer Complete Flags */
	USART_ClearFlag(USARTx, USART_FLAG_TC);

}

void Serial::receive(char * data) {
	strcpy(data, rxBuffer);
}

void Serial::resetRx() {
	DMA_Cmd(USARTx_RX_DMA_STREAM, DISABLE);
	DMA_SetCurrDataCounter(USARTx_RX_DMA_STREAM, serBUFFERSIZE);
	DMA_Cmd(USARTx_RX_DMA_STREAM, ENABLE);
}

Serial::~Serial() {
#ifdef DEBUG
	trace_puts(" Serial Deletado");
#endif
}
