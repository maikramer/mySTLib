//=================================================================================//
//	Arquivo : Time.cpp
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

#include <Time.h>
#include <diag/Trace.h>

//=================================================================================//
//	Definição dos Métodos
//=================================================================================//

Time::Time() : _ms(0) {
	/* Configura SysTick para 1ms de interrupção  */
	SysTick_Config(SystemCoreClock / 1000);

	/* Configura prioridade do SysTick para Máxima */
	NVIC_SetPriority(SysTick_IRQn, 0x0);
}

void Time::delay(uint32_t ms) {
	uint32_t start;

	start = _ms;
	while ((_ms - start) < ms)
		;
}

Time time;

extern "C" void SysTick_Handler(void) {
	time._ms++;
}

Time::~Time() {
#ifdef DEBUG
	trace_puts(" Time Deletado");
#endif
}
