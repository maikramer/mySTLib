//=================================================================================//
//	Arquivo : SharpRanger.cpp
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

//=================================================================================//
//	Includes
//=================================================================================//

#include <SharpRanger.h>

//=================================================================================//
//	Definição das Funções e Métodos
//=================================================================================//

SharpRanger::SharpRanger(uint16_t pin) :
		AnalogIn(pin), medium(0), iter(0) {

}

uint32_t SharpRanger::readDistance() {
	uint32_t ret = 0;
	ret = (uint32_t) ((12500.0f / read_mV()) - 0.42f);
	calcMd();
	ret = (ret + medium) / 2;
	hist[iter] = ret;
	if (++iter == FILTERING) {
		iter = 0;
	}
	return ret;
}

void SharpRanger::calcMd() {
	uint32_t tmp = 0;
	for (int i = 0; i < FILTERING; i++) {
		if (hist[i] != 0) {
			tmp += hist[i];
		}
	}
	medium = tmp / FILTERING;
}

SharpRanger::~SharpRanger() {
	;
}
