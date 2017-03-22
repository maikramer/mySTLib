//=================================================================================//
//	Arquivo : AnalogIn.h
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

#ifndef ANALOGIN_H_
#define ANALOGIN_H_

//=================================================================================//
//	Includes
//=================================================================================//

#include <cmsis_device.h>
#include <stdint.h>

//=================================================================================//
//	Definição do Objeto
//=================================================================================//

class AnalogIn {
public:
	/*
	 * @brief  	Cria um Objeto Entrada Analógica
	 * @param  	pin: Pino em a ser utilizado
	 * 				Pode ser um valor entre GPIO_Pin_0 e GPIO_Pin_5
	 *
	 * @note  	No momento apenas os pinos do GPIOC PC0 até PC5 são suportados
	 *
	 * @retval 	None
	 */
	AnalogIn(uint16_t pin);

	/*
	 * @brief  Destructor não faz nada no momento
	 * @retval None
	 */
	virtual ~AnalogIn();

	/*
	 * @brief  	Lê e converte o valor da saída em mV
	 *
	 * @note	Assume VCC 3,3V
	 *
	 * @retval 	Valor em miliVolts
	 */
	uint16_t read_mV();

	/*
	 * @brief  	Lê e converte o valor da saída em Percentagem
	 *
	 * @retval 	Valor em Percentual de VCC
	 */
	uint8_t read();

	/*
	 * @brief  	Lê o valor da saída
	 *
	 * @retval 	Valor puro, sem conversão 0 a 4096
	 */
	uint16_t read_raw() {
		return _value[_valueIndex];
	}

	/*
	 * @brief  	Lê e converte o valor da saída em Percentagem
	 *
	 * @retval 	Valor em Percentual de VCC
	 */
	operator uint8_t() {
		return read();
	}

private:
	uint16_t _pin;
	static uint16_t _value[6];
	uint8_t _valueIndex;
	uint8_t _channel;
	GPIO_TypeDef* _port;
	static uint8_t nAnalogIn;
};

#endif /* ANALOGIN_H_ */
