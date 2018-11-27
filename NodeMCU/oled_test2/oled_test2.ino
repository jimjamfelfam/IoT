#include <oled.h>

// Box pattern
uint8_t pattern1[] = {
	0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00,
	0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00
	};

// Cross weave pattern
uint8_t pattern2[] = {
	0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81,
	0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81
	};

uint16_t pat = 0;

void setup()   {                
	// Init the OLED controller
	OLED_init();
}

void loop() {
	// I2C
	Wire.beginTransmission(OLED_I2C_ADDRESS);
	Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
	Wire.write(OLED_CMD_SET_COLUMN_RANGE);
	Wire.write(0x00);
	Wire.write(0x7F);
	Wire.write(OLED_CMD_SET_PAGE_RANGE);
	Wire.write(0);
	Wire.write(0x07);
	Wire.endTransmission();

	for(uint16_t i=0;i<1024;i++){
		Wire.beginTransmission(OLED_I2C_ADDRESS);
		Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
	    for (uint8_t x=0; x<16; x++) {	
			switch (pat){
				case 0:
					Wire.write(0xC1);
					break;
				case 1:
					Wire.write(0x81);
					break;
				case 2:
					Wire.write(0x02);
					break;
				case 3:
					Wire.write(pattern1[x]);
					break;
				case 4:
					Wire.write(pattern2[x]);
					break;
			}
			i++;
	    }
	    i--;
	    Wire.endTransmission();   
	}
	pat++;
	if (pat==5) pat=0;
    delay(1000);
}

void OLED_init() {
	// Init the I2C interface (pins A4 and A5 on the Arduino Uno board) in Master Mode.
	Wire.begin();

	// Begin the I2C comm with SSD1306's address (SLA+Write)
	Wire.beginTransmission(OLED_I2C_ADDRESS);

	// Tell the SSD1306 that a command stream is incoming
	Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);

	// Follow instructions on pg.64 of the dataSheet for software configuration of the SSD1306
	// Turn the Display OFF
	Wire.write(OLED_CMD_DISPLAY_OFF);

	// Set mux ration tp select max number of rows - 64
	Wire.write(OLED_CMD_SET_MUX_RATIO);
	Wire.write(0x3F);

	// Set the display offset to 0
	Wire.write(OLED_CMD_SET_DISPLAY_OFFSET);
	Wire.write(0x00);

	// Display start line to 0
	Wire.write(OLED_CMD_SET_DISPLAY_START_LINE);

	// Mirror the x-axis. In case you set it up such that the pins are north.
	// Wire.write(0xA0); - in case pins are south - default
	Wire.write(OLED_CMD_SET_SEGMENT_REMAP);
		
	// Mirror the y-axis. In case you set it up such that the pins are north.
	// Wire.write(0xC0); - in case pins are south - default
	Wire.write(OLED_CMD_SET_COM_SCAN_MODE);

	// Default - alternate COM pin map
	Wire.write(OLED_CMD_SET_COM_PIN_MAP);
	Wire.write(0x12);

	// set contrast
	Wire.write(OLED_CMD_SET_CONTRAST);
	Wire.write(0x7F);

	// Set display to enable rendering from GDDRAM (Graphic Display Data RAM)
	Wire.write(OLED_CMD_DISPLAY_RAM);

	// Normal mode!
	Wire.write(OLED_CMD_DISPLAY_NORMAL);

	// Default oscillator clock
	Wire.write(OLED_CMD_SET_DISPLAY_CLK_DIV);
	Wire.write(0x80);

	// Enable the charge pump
	Wire.write(OLED_CMD_SET_CHARGE_PUMP);
	Wire.write(0x14);

	// Set precharge cycles to high cap type
	Wire.write(OLED_CMD_SET_PRECHARGE);
	Wire.write(0x22);

	// Set the V_COMH deselect volatage to max
	Wire.write(OLED_CMD_SET_VCOMH_DESELCT);
	Wire.write(0x30);

	// Horizonatal addressing mode - same as the KS108 GLCD
	Wire.write(OLED_CMD_SET_MEMORY_ADDR_MODE);
	Wire.write(0x00);

	// Turn the Display ON
	Wire.write(OLED_CMD_DISPLAY_ON);

	// End the I2C comm with the SSD1306
	Wire.endTransmission();
}