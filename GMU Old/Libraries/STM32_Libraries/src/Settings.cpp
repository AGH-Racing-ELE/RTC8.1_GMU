#include "Settings.h"
#include <string.h>
#include "stm32f3xx.h"

Settings theSettings;

Settings::Settings()
{

}

Settings::~Settings()
{

}

void Settings::initialize(void)
{
	strcpy(setting_list[(uint8_t)Index::LcdBrightness].name, "LcdBright");
	setting_list[(uint8_t)Index::LcdBrightness].default_val = 1000;

	strcpy(setting_list[(uint8_t)Index::LcdContrast].name, "LcdContrast");
	setting_list[(uint8_t)Index::LcdContrast].default_val = 160;

	strcpy(setting_list[(uint8_t)Index::GearCut].name, "GearCut");
	setting_list[(uint8_t)Index::GearCut].default_val = 1;

	strcpy(setting_list[(uint8_t)Index::TractionCtrlLevel].name, "TcLevel");
	setting_list[(uint8_t)Index::TractionCtrlLevel].default_val = 0;

	strcpy(setting_list[(uint8_t)Index::PitLimit].name, "PitLimit");
	setting_list[(uint8_t)Index::PitLimit].default_val = 0;

	strcpy(setting_list[(uint8_t)Index::AutoUpshift].name, "AutoUpshift");
	setting_list[(uint8_t)Index::AutoUpshift].default_val = 0;

	strcpy(setting_list[(uint8_t)Index::AutoUpshiftRPM].name, "AutoUpRPM");
	setting_list[(uint8_t)Index::AutoUpshiftRPM].default_val = 14500;

	strcpy(setting_list[(uint8_t)Index::AntiStall].name, "AntiStall");
	setting_list[(uint8_t)Index::AntiStall].default_val = 0;

	strcpy(setting_list[(uint8_t)Index::Debug].name, "Debug");
	strcpy(setting_list[(uint8_t)Index::Debug].shortcut, "DBG");

	readFromFlash();
}

void Settings::readFromFlash(void)
{
	uint32_t flash_adr = sett_flash_adr;

	for(uint8_t i = 0; i < (uint8_t)Index::SETTINGS_COUNT; ++i, flash_adr += 2)
	{
		volatile uint16_t *val = (volatile uint16_t *)flash_adr;
		setting_list[i].value = ((*val) == 0xFFFF) ? setting_list[i].default_val : *val;
	}
}

void Settings::saveToFlash(void)
{
	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t sector_error;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = sett_flash_adr;
	EraseInitStruct.NbPages = 1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &sector_error);

	uint32_t flash_adr = sett_flash_adr;
	for(uint8_t i = 0; i < (uint8_t)Index::SETTINGS_COUNT; ++i, flash_adr += 2) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_adr, setting_list[i].value);
	}

	HAL_FLASH_Lock();
}
