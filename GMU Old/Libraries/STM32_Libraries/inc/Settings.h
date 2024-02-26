#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>

#define FLASH_PAGE_ADR(page) (0x08000000 + (page * 0x800))

class Settings
{

// Types
public:
	enum class Index {
		LcdBrightness = 0u,
		LcdContrast,
		GearCut,
		TractionCtrlLevel,
		PitLimit,
		AutoUpshift,
		AutoUpshiftRPM,
		AntiStall,
		Debug,
		SETTINGS_COUNT
	};

	struct Setting {
		char name[12];
		char shortcut[8];
		uint16_t value = 0;
		uint16_t default_val = 0;
	};

// Methods
public:
	Settings();
	~Settings();

	void initialize(void);

	void readFromFlash(void);
	void saveToFlash(void);

	void setValue(Index idx, uint16_t value) { setting_list[(uint8_t)idx].value = value; }

	uint16_t getValue(Index idx) const { return setting_list[(uint8_t)idx].value; }
	const char * getShortcut(Index idx) const { return setting_list[(uint8_t)idx].shortcut; }

// Types
private:

// Members
private:
	Setting setting_list[(uint8_t)Index::SETTINGS_COUNT];
	static const uint32_t sett_flash_adr = FLASH_PAGE_ADR(63);

// Methods
private:

};

extern Settings theSettings;

#endif /* SETTINGS_H_ */
