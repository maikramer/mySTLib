//=================================================================================//
//	Arquivo : main.cpp
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

//=================================================================================//
//	Includes
//=================================================================================//

#include <cmsis_device.h>
#include <stdint.h>
#include <Time.h>
#include <DigitalOut.h>
#include <DigitalIn.h>
#include <SharpRanger.h>
#include <Serial.h>
#include <diag/Trace.h>

#include <ff.h>

//=================================================================================//
//	Variáveis e Objetos Globais
//=================================================================================//

DigitalOut ledVe(GPIOE, GPIO_Pin_0);
DigitalOut ledAm(GPIOE, GPIO_Pin_1);

DigitalIn botao(GPIOB, GPIO_Pin_6);
SharpRanger sensorIR(GPIO_Pin_0);
//Serial		serial;

const char msg[] = "AT+GMR\r\n";

//Fatfs object
static FATFS FatFs;
//File object
static FIL fil;

//=================================================================================//
//	Programa Principal
//=================================================================================//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"

int main(int argc, char** argv) {

	if (f_mount(&FatFs, "", 1) == FR_OK) {
		//Mounted OK, turn on RED LED
		trace_printf("\nDrive Montado\n");

		//Try to open file
		if (f_open(&fil, "1stfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE)
				== FR_OK) {
			//File opened, turn off RED and turn on GREEN led
			trace_printf("\nArquivo Aberto\n");

			//If we put more than 0 characters (everything OK)
			char msgSD[] = "First string in my file\n";
			if (f_puts(msgSD, &fil) > 0) {
				trace_printf("\nDados Gravados\n");
			} else {
				trace_printf("\nFalha ao gravar dados\n");
			}
		} else {
			trace_printf("\nFalha ao abrir arquivo\n");
		}

	} else {
		trace_printf("\nFalha ao montar o Drive\n");
	}
	if (f_sync(&fil) != FR_OK) {
		trace_printf("\nFalha Sincronizando Dados\n");
	}

	while (1) {
		//char msgRcvd[256];
		ledVe = !ledVe;
		for (int i = 0; i < 50; i++) {
			ledAm = !botao;
			time.delay(20);
		}
		//serial.send(msg);
		trace_printf("Valor Abs : %d\n", sensorIR.read_raw());
		time.delay(5);
		trace_printf("Valor Real : %dmV\n", sensorIR.read_mV());
		time.delay(5);
		int valor = sensorIR;
		trace_printf("Valor Porcentagem : %d%%\n", valor);
		time.delay(5);
		trace_printf("Distancia : %dcm\n", sensorIR.readDistance());
		//serial.receive(msgRcvd);
		time.delay(5);
		//trace_printf("Recebido : %s\n\n",msgRcvd);
		//serial.resetRx();
	}
}

#pragma GCC diagnostic pop
