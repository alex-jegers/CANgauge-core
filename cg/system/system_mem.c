/**********     INCLUDES        **********/
#include "system_mem.h"
#include "drivers/drivers.h"
#include <string.h>
#include <stdlib.h>

/**********		DEFINES		**********/

/**********		EXTERNAL VARIABLE DEFINITIONS		**********/

/**********		STATIC VARIABLES		**********/
SYS_MEM_REGION_EXTERN_RAM static uint8_t system_mem_ram_file_system[SECTOR_SIZE_RAM * NUM_SECTORS_RAM];
static FATFS* prv_fs_ram;
static FATFS* prv_fs_eeprom;           // Filesystem object
/**********		STATIC FUNCTION DECLRATIONS		**********/
SYS_MEM_REGION_RAM_EXE static void prv_memcpy(void* dest, void* src, size_t n_bytes);

/**********		STATIC FUNCTION DEFINITIONS		**********/
SYS_MEM_REGION_RAM_EXE static void prv_memcpy(void* dest, void* src, size_t n_bytes)
{
	for (uint32_t i = 0; i < n_bytes; i++)
	{
		*((uint8_t*)dest + i) = *((uint8_t*)src + i);
	}
}

/**********		GLOBAL FUNCTION DEFINITIONS		**********/
FRESULT sys_mem_create_eeprom_fs()
{
	if (prv_fs_eeprom != NULL)
	{
		return -1;
	}
	FRESULT res;
	const MKFS_PARM params_eeprom =
	{
			.fmt = FM_FAT,
			.n_fat = 1,
			.align = 0,
			.n_root = 16,
			.au_size = 0
	};

	SYS_MEM_REGION_EXTERN_RAM static uint8_t work_eeprom[4096];
	memset(work_eeprom, 0, 4096);
	res = f_mkfs("0:", &params_eeprom, &work_eeprom, 4096);
	prv_fs_eeprom = (FATFS*)malloc(sizeof( FATFS ));
	if (prv_fs_eeprom == NULL)
	{
		return FR_DISK_ERR;
	}
	res = f_mount(prv_fs_eeprom, "0:", 1);
	f_setlabel("CANgauge");
	return res;
}

FRESULT sys_mem_create_ram_fs(uint32_t size_bytes)
{
	FRESULT res;        // API result code
	const MKFS_PARM params =
	{
			.fmt = FM_FAT,
			.n_fat = 1,
			.align = 0,
			.n_root = 0,
			.au_size = 0
	};

	/* Set up the working memory and make the file system in RAM. */
	SYS_MEM_REGION_EXTERN_RAM static uint8_t work_ram[4096];
	memset(work_ram, 0, 4096);
	res = f_mkfs("1:", &params, &work_ram, 4096);
	prv_fs_ram = (FATFS*)malloc(sizeof( FATFS ));
	if (prv_fs_ram == NULL)
	{
		return FR_DISK_ERR;
	}
	res = f_mount(prv_fs_ram, "1:", 1);
	f_setlabel("1:CANgauge");
	return res;
}

FRESULT sys_mem_init_ram_fs()
{
	if (prv_fs_ram == NULL)
	{
		prv_fs_ram = (FATFS*)malloc(sizeof( FATFS ));
	}
	FRESULT res = f_mount(prv_fs_ram, "1:", 1);
	if (res == FR_OK)
	{
		f_setlabel("1:CANgauge");
		return res;
	}
	else
	{
		free(prv_fs_ram);
		return res;
	}
}

FRESULT sys_mem_init_eeprom_fs()
{
	if (prv_fs_eeprom == NULL)
	{
		prv_fs_eeprom = (FATFS*)malloc(sizeof( FATFS ));
	}
	FRESULT res = f_mount(prv_fs_eeprom, "0:", 1);
	if (res == FR_OK)
	{
		f_setlabel("CANgauge");
		return res;
	}
	else
	{
		free(prv_fs_eeprom);
		return res;
	}
}

SYS_MEM_REGION_RAM_EXE void sys_mem_flash_write_sector(uint8_t sector, void* src)
{
    /* Bank start address. */
    uint32_t sector_saddr = (sector * 0x20000) + 0x08000000;


	if (sector < 8)
	{
		flash_b1_unlock();
		flash_b1_erase(sector);
		while (flash_b1_qw_status()) {}
		flash_b1_enable_write();
		for (uint32_t i = 0; i < 0x20000; i+= 0x20)
		{
			prv_memcpy((uint8_t*)sector_saddr + i, (uint8_t*)src + i, 0x20);
			while (flash_b1_qw_status()) {}
		}
		flash_b1_disable_write();
		flash_b1_lock();
	}
	else
	{
		flash_b2_unlock();
		flash_b2_erase(sector - 8);
		while (flash_b2_qw_status()) {}
		flash_b2_enable_write();
		for (uint32_t i = 0; i < 0x20000; i+= 0x20)
		{
			prv_memcpy((uint8_t*)sector_saddr + i, (uint8_t*)src + i, 0x20);
			while (flash_b2_qw_status()) {}
		}
		flash_b2_disable_write();
		flash_b2_lock();
	}
}

uint8_t* sys_mem_get_ram_fs_ptr()
{
	return system_mem_ram_file_system;
}

