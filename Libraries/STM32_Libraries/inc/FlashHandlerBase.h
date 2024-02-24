#ifndef INC_FLASHHANDLERBASE_H_
#define INC_FLASHHANDLERBASE_H_

#if defined(STM32F303xC)
#include "stm32f3xx.h"
#else
#include "stm32f0xx.h"
#endif

class FlashHandlerBase {
public:
	static constexpr uint32_t getPageAddr(uint8_t page_nr)
	{
		return FLASH_BASE + page_nr * FLASH_PAGE_SIZE;
	}

	void writeUserOB(uint8_t data_idx, uint8_t value)
	{
		__disable_irq();

		uint8_t data0 = (uint8_t)HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA0);
		uint8_t data1 = (uint8_t)HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA1);
		if(data_idx == 0)
			data0 = value;
		else
			data1 = value;

		HAL_FLASH_Unlock();
		HAL_FLASH_OB_Unlock();
		HAL_FLASHEx_OBErase();

		FLASH_OBProgramInitTypeDef ob;
		ob.OptionType = OPTIONBYTE_DATA;
		ob.DATAAddress = OB_DATA_ADDRESS_DATA0;
		ob.DATAData = data0;
		HAL_FLASHEx_OBProgram(&ob);

		ob.DATAAddress = OB_DATA_ADDRESS_DATA1;
		ob.DATAData = data1;
		HAL_FLASHEx_OBProgram(&ob);

		HAL_FLASH_OB_Launch();
	}

	void writePage(uint8_t page_nr, const uint16_t *data, uint16_t size)
	{
		__disable_irq();
		uint32_t page_addr = getPageAddr(page_nr);

		HAL_FLASH_Unlock();

		FLASH_EraseInitTypeDef EraseInitStruct;
		uint32_t sector_error;
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.PageAddress = page_addr;
		EraseInitStruct.NbPages = 1;
		HAL_FLASHEx_Erase(&EraseInitStruct, &sector_error);
		assert_param(sector_error == UINT32_MAX);

		uint32_t flash_adr = page_addr;
		for(uint8_t i = 0; i < size; ++i, flash_adr += 2, ++data) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_adr, *data);
		}

		HAL_FLASH_Lock();
		__enable_irq();
	}

};




#endif /* INC_FLASHHANDLERBASE_H_ */
