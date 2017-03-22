//=================================================================================//
//	Arquivo : DigitalOut.cpp
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

#include <DigitalOut.h>
#include <diag/Trace.h>

//=================================================================================//
//	Definição dos Métodos
//=================================================================================//

DigitalOut::DigitalOut(GPIO_TypeDef* port, uint16_t pin) :
		_port(port), _pin(pin), _init(0) {

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = _pin;

#ifdef STM32F40_41xxx
	if (port == GPIOA)
		_clk = RCC_AHB1Periph_GPIOA;
	else if (port == GPIOB)
		_clk = RCC_AHB1Periph_GPIOB;
	else if (port == GPIOC)
		_clk = RCC_AHB1Periph_GPIOC;
	else if (port == GPIOD)
		_clk = RCC_AHB1Periph_GPIOD;
	else if (port == GPIOE)
		_clk = RCC_AHB1Periph_GPIOE;
	else if (port == GPIOF)
		_clk = RCC_AHB1Periph_GPIOF;
	else
		_clk = 0;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	RCC_AHB1PeriphClockCmd(_clk, ENABLE);
#else
#ifdef STM32F10XX
	if (port == GPIOA)
	_clk = RCC_APB2Periph_GPIOA;
	else if (port == GPIOB)
	_clk = RCC_APB2Periph_GPIOB;
	else if (port == GPIOC)
	_clk = RCC_APB2Periph_GPIOC;
	else if (port == GPIOD)
	_clk = RCC_APB2Periph_GPIOD;
	else if (port == GPIOE)
	_clk = RCC_APB2Periph_GPIOE;
	else if (port == GPIOF)
	_clk = RCC_APB2Periph_GPIOF;
	else
	_clk = 0;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	RCC_APB2PeriphClockCmd(_clk, ENABLE);
#else
#error Incompativel com o Microcontrolador
#endif // STM32F10XX
#endif // STM32F40_41xxx
	GPIO_Init(_port, &GPIO_InitStructure);

}

DigitalOut::~DigitalOut() {
#ifdef DEBUG
	trace_puts(" DigitalOut Deletado");
#endif
}

