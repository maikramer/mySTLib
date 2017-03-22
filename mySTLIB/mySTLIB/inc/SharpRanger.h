//=================================================================================//
//	Arquivo : SharpRanger.h
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
#ifndef SHARPRANGER_H_
#define SHARPRANGER_H_

//=================================================================================//
//	Includes
//=================================================================================//

#include <AnalogIn.h>

//=================================================================================//
//	Definição das Funções e Métodos
//=================================================================================//

#define FILTERING 3

class SharpRanger : public AnalogIn {
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
	SharpRanger(uint16_t pin);

	uint32_t readDistance();
	/*
	 * @brief  Destructor não faz nada no momento
	 * @retval None
	 */

	virtual ~SharpRanger();

private:

	uint32_t hist[FILTERING] = {0};
	uint32_t medium;
	uint32_t iter;

	void calcMd();

};



#endif /* SHARPRANGER_H_ */
