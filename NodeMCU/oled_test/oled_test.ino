#include <oled.h>

// Used LCD Assistant to generate this starting splash screen for the buffer
// 128x64 = 8192 bits = 1024 bytes
static unsigned char buffer [1024] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xE0, 0xE0, 0x18, 0x18, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x18,
0x18, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0xFE, 0xFE, 0x06, 0x06, 0x06, 0x06, 0x1E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x06,
0x06, 0x06, 0x06, 0x06, 0x06, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x1E, 0x06, 0x06, 0x06, 0x06, 0xFE,
0xFE, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x7F, 0x7F, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x87,
0x87, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x0C, 0x0C, 0x0C,
0x0C, 0x00, 0x00, 0xFC, 0xFC, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0x00, 0x00, 0xC0,
0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x30, 0x30, 0x48, 0x86, 0x86, 0x48, 0x30, 0x30, 0x00, 0xC0,
0xC0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0xCC, 0xCC, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,
0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
0xF8, 0x18, 0x18, 0x19, 0x19, 0x1E, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x60, 0x60, 0x60, 0x60, 0x60,
0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x1E, 0x19,
0x19, 0x18, 0x18, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x0C,
0x0C, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x3F,
0x3F, 0x0C, 0x0C, 0x03, 0x03, 0x00, 0x0C, 0x0C, 0x12, 0x61, 0x61, 0x12, 0x0C, 0x0C, 0x00, 0x03,
0x03, 0x0C, 0x0C, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F,
0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x0C, 0x0C, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0x60, 0x60, 0x7F,
0x7F, 0x60, 0x60, 0x60, 0x60, 0xE0, 0xE0, 0xF8, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0xF8, 0xE0, 0xE0, 0x60,
0x60, 0x60, 0x60, 0x7F, 0x7F, 0x60, 0x60, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x60, 0x60, 0x60,
0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0xFF, 0x7F, 0x7F, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0xE1,
0xE1, 0x61, 0x61, 0xE1, 0xE1, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x7F, 0x7F, 0xFF, 0xFF, 0x60,
0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7F,
0x7F, 0x00, 0x00, 0x7F, 0x7F, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7F, 0x7F, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned int frame;

void setup()   {                
	// Init the OLED controller
	OLED_init();

	// Customary delay
	delay(2000);
}

void loop() {
	// Collect the current time. This is to ensure maximum frame time will be 40ms
	frame=millis();

	// Set the GDDRAM to (Row0, Col0), ie: top-left and establish range as the whole screen - 128x64
	Wire.beginTransmission(OLED_I2C_ADDRESS);
	Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);

	// column 0 to 127
	Wire.write(OLED_CMD_SET_COLUMN_RANGE);
	Wire.write(0x00);		
	Wire.write(0x7F);

	// page 0 to 7
	Wire.write(OLED_CMD_SET_PAGE_RANGE);
	Wire.write(0x00);
	Wire.write(0x07);
	Wire.endTransmission();

	for(uint16_t i=0;i<1024;i++){
		Wire.beginTransmission(OLED_I2C_ADDRESS);
		Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
	    for (uint8_t x=0; x<16; x++) {	
			Wire.write(buffer[i]);	
			i++;
	    }
	    i--;
	    Wire.endTransmission();   
	}

	// Frame Guard. Ensure that the frame lasted at least 40ms
	while((millis()-frame)<40){};
}

void OLED_init() {
	// Init the I2C interface (pins D1 and D2 on NodeMCU board) in Master Mode.
	Wire.begin();

	// Set the I2C to HS mode - 400KHz!
	// TWBR = (CPU_CLK / I2C_CLK) -16 /2
	// TWBR = ((16,000,000 / 400,000) - 16) / 2 = 12
	// TWBR=12;

	// keywords:
	// SEG = COL = segment = column byte data on a page
	// Page = 8 pixel tall row. Has 128 SEGs and 8 COMs
	// COM = row

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