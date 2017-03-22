//=================================================================================//
//	Arquivo : DigitalIn.h
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
#ifndef DIGITALIN_H_
#define DIGITALIN_H_

//=================================================================================//
//	Includes
//=================================================================================//

#include <cmsis_device.h>
#include <stdint.h>

//=================================================================================//
//	Definição do Objeto
//=================================================================================//

class DigitalIn {
public:

	// O construtor recebe a porta e o pino para criar a saída digital
	// port deve ser entre GPIOA e GPIOF
	// pin deve ser entre GPIO_Pin_0 e GPIO_Pin_15
	DigitalIn(GPIO_TypeDef* port, uint16_t pin);

	// O destrutor no momento não faz nada
	virtual ~DigitalIn();

	// Lê o status do pino
	// 1 Set
	// 0 Reset
	int read() {
		return GPIO_ReadInputDataBit(_port, _pin);
	}

	operator int() {
		return read();
	}
private:
	GPIO_TypeDef* _port;
	uint16_t _pin;
	uint32_t _clk;
};

#endif /* DIGITALIN_H_ */
