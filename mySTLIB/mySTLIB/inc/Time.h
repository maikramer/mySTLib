//=================================================================================//
//	Arquivo : Time.h
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
#ifndef TIME_H_
#define TIME_H_
//==================================================================================//
//	Includes
//==================================================================================//

#include <cmsis_device.h>

//==================================================================================//
//	Definições
//==================================================================================//

extern "C" void SysTick_Handler(void);

class Time {
public:

	// Decllara o Handler do SysTick como Friend para poder ter acesso
	friend void SysTick_Handler(void);

	// Construtor inicializa o SysTick e o inicia
	Time();

	// Destrutor não faz nada
	virtual ~Time();

	operator int() {
		return _ms;
	}

	void delay(uint32_t ms);
protected:
	volatile uint32_t _ms;
};

extern Time time;

#endif /* MYSTLIB_INC_TIME_H_ */
