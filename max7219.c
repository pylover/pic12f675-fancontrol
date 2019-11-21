
//
// Use one MAX7219 to control 8-digit seven-segment display
// the display module: http://www.icshop.com.tw/product_info.php/products_id/20686
//
// MAX7219 datasheet: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
// 

// the MAX7219 address map (datasheet table 2)
#define MAX7219_DECODE_REG      (0x09)
#define MAX7219_INTENSITY_REG   (0x0A)
#define MAX7219_SCANLIMIT_REG   (0x0B)
#define MAX7219_SHUTDOWN_REG    (0X0C)
#define MAX7219_DISPLAYTEST_REG (0x0F)
#define MAX7219_DIGIT_REG(pos)  ((pos) + 1)

// shutdown mode (datasheet table 3)
#define MAX7219_OFF             (0x0)
#define MAX7219_ON              (0x1)


#define MAX7219_DATA   GP0
#define MAX7219_LOAD   GP1
#define MAX7219_CLOCK  GP4


#define DP_FLAG       (0b10000000)
#define NUM_OF_DIGITS (8)



// digit pattern for a 7-segment display. datasheet table 5
static const unsigned char digit_pattern[10] = {
  0b01111110,  // 0
  0b00110000,  // 1
  0b01101101,  // 2
  0b01111001,  // 3
  0b00110011,  // 4
  0b01011011,  // 5
  0b01011111,  // 6
  0b01110000,  // 7
  0b01111111,  // 8
  0b01111011,  // 9
//  0b01110111,  // A
//  0b00011111,  // b
//  0b01001110,  // C
//  0b00111101,  // d
//  0b01001111,  // E
//  0b01000111   // F
};


void shiftout (unsigned char data) {
	int i;
	for (i = 0; i < 8; i++) {
        MAX7219_DATA = !!(data & (1 << (7 - i)));
        MAX7219_CLOCK = 1;
        MAX7219_CLOCK = 0;
	}
}


// update the register value of MAX7219
void set_register(unsigned char address, unsigned char value) {
    MAX7219_LOAD = 0;
    shiftout(address);
    shiftout(value);
    MAX7219_LOAD = 1;
}


void max7219_init() {
    MAX7219_LOAD = 0;
    MAX7219_CLOCK = 0;
    MAX7219_DATA = 0;

    // disable test mode. datasheet table 10
    set_register(MAX7219_DISPLAYTEST_REG, MAX7219_OFF);
    // set medium intensity. datasheet table 7
    set_register(MAX7219_INTENSITY_REG, 0x3);
    // turn on display. datasheet table 3
    set_register(MAX7219_SHUTDOWN_REG, MAX7219_ON);
    // drive 8 digits. datasheet table 8
    set_register(MAX7219_SCANLIMIT_REG, 7);
    // no decode mode for all positions. datasheet table 4
    set_register(MAX7219_DECODE_REG, 0b00000000);
}



void display(unsigned int number, unsigned short dp) {
    unsigned short i;
    unsigned int digit_value;
    unsigned char byte_data;

    for (i = 0; i < NUM_OF_DIGITS; i++){
      digit_value = number % 10;
      number /= 10;
      byte_data = digit_pattern[digit_value];
  
      if (dp == i) 
          byte_data |= DP_FLAG;
  
      set_register(MAX7219_DIGIT_REG(i), byte_data);
    }
}

