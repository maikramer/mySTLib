#include <diskio.h>		/* FatFs lower layer API */

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
#if DETECT_SD == 1
	if (SD_Detect() != SD_PRESENT) {
		return STA_NOINIT;
	}
#endif
	if (pdrv > 0)
		return STA_NOINIT;
	else
		return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
	if (pdrv > 0 || (SD_Init() != SD_OK)) {
		return STA_NOINIT;
	}

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, /* Physical drive nmuber to identify the drive */
BYTE *buff, /* Data buffer to store read data */
DWORD sector, /* Start sector in LBA */
UINT count /* Number of sectors to read */
) {
	SD_Error Status = SD_OK;

	if (pdrv > 0) {
		return RES_ERROR;
	}

#if DETECT_SD == 1
	if (SD_Detect() != SD_PRESENT) {
		return RES_NOTRDY;
	}
#endif
	Status = SD_ReadMultiBlocks(buff, sector << 9, BLOCK_SIZE, count);

	if (Status == SD_OK) {
		SDTransferState State;

		Status = SD_WaitReadOperation();

		while ((State = SD_GetStatus()) == SD_TRANSFER_BUSY)
			;

		if ((State == SD_TRANSFER_ERROR) || (Status != SD_OK)) {
			return RES_ERROR;
		} else {
			return RES_OK;
		}
	} else {
		return RES_ERROR;
	}
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv, /* Physical drive nmuber to identify the drive */
const BYTE *buff, /* Data to be written */
DWORD sector, /* Start sector in LBA */
UINT count /* Number of sectors to write */
) {
	SD_Error Status = SD_OK;

	if (pdrv > 0) {
		return RES_ERROR;
	}

#if DETECT_SD == 1
	if (SD_Detect() != SD_PRESENT) {
		return RES_NOTRDY;
	}
#endif

	/*if ((DWORD) buff & 3) {
		DRESULT res = RES_OK;
		DWORD scratch[BLOCK_SIZE / 4];

		while (count--) {
			memcpy(scratch, buff, BLOCK_SIZE);
			res = disk_write(pdrv, (void *) scratch, sector++, 1);

			if (res != RES_OK) {
				break;
			}

			buff += BLOCK_SIZE;
		}

		return (res);
	}*/

	Status = SD_WriteMultiBlocks((uint8_t *) buff, sector << 9, BLOCK_SIZE,
			count); // 4GB Compliant

	if (Status == SD_OK) {
		SDTransferState State;

		Status = SD_WaitWriteOperation(); // Check if the Transfer is finished

		while ((State = SD_GetStatus()) == SD_TRANSFER_BUSY)
			; // BUSY, OK (DONE), ERROR (FAIL)

		if ((State == SD_TRANSFER_ERROR) || (Status != SD_OK)) {
			return RES_ERROR;
		} else {
			return RES_OK;
		}
	} else {
		return RES_ERROR;
	}
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
BYTE cmd, /* Control code */
void *buff /* Buffer to send/receive control data */
) {
	if (pdrv > 0) {
		return RES_ERROR;
	}
	switch (cmd) {
	case GET_SECTOR_SIZE:     // Get R/W sector size (WORD)
		*(WORD *) buff = 512;
		break;
	case GET_BLOCK_SIZE:      // Get erase block size in unit of sector (DWORD)
		*(DWORD *) buff = 32;
		break;
	case CTRL_SYNC:
		break;
	}

	return RES_OK;
}

