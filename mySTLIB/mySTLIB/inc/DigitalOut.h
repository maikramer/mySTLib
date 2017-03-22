//=================================================================================//
//	Arquivo : DigitalOut.h
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
//
//=================================================================================//

#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

//=================================================================================//
//	Includes
//=================================================================================//

#include <cmsis_device.h>
#include <stdint.h>

//=================================================================================//
//	Definição do Objeto
//=================================================================================//

class DigitalOut {
public:

	// O construtor recebe a porta e o pino para criar a saída digital
	// port deve ser entre GPIOA e GPIOF
	// pin deve ser entre GPIO_Pin_0 e GPIO_Pin_15
	DigitalOut(GPIO_TypeDef* port, uint16_t pin);

	// O destrutor no momento não faz nada
	virtual ~DigitalOut();

	// Escreve um valor na saída
	// 1 Seta
	// 0 Reset
	void write(int value) {
		GPIO_WriteBit(_port, _pin, (BitAction)value);
	}

	// Lê o status do pino
	// 1 Set
	// 0 Reset
	int read() {
		return GPIO_ReadOutputDataBit(_port, _pin);
	}

	// Sobrecarga de operador '='
	// Pode definir a saida usando ex: minhaSaida = 1
	DigitalOut& operator=(int value) {
		write(value);
		return *this;
	}

	// Sobrecarga de operador '='
	// Pode receber o estado de outra saida ex: minhaSaidaDestino = minhaSaidaOrigem
	DigitalOut& operator=(DigitalOut& rhs) {
		write(rhs.read());
		return *this;
	}

	operator int() {
		return read();
	}

private:
	GPIO_TypeDef* _port;
	uint16_t _pin;
	uint16_t _init;
	uint32_t _clk;
};

#endif /* DIGITALOUT_H_ */
