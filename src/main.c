/*
 * main.c
 *
 * Created: 8/16/2024
 * Author : Januka
 */ 

#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_ADDR 0x27

void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
void lcd_send(uint8_t value, uint8_t mode);
void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void lcd_init(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(char* str);

int main(void) {
	TWI_init();   // Initialize TWI (I2C) and LCD
	lcd_init();

	while (1) {
		// Display text on the LCD
		lcd_set_cursor(0, 2);
		lcd_print("Hello World!");

		lcd_set_cursor(1, 2);
		lcd_print("LCD Tutorial");

		_delay_ms(1000);
		// lcd_command(0x01); // Clear display
	}
}

void TWI_init(void) {
	TWSR = 0x00;
	TWBR = 0x48;
	TWCR = (1 << TWEN);  // Enable TWI
}

void TWI_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);  // Send start condition
	while (!(TWCR & (1 << TWINT)));  // Wait for TWINT flag
}

void TWI_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);  // Send stop condition
	while (TWCR & (1 << TWSTO));  // Wait for stop to complete
}

void TWI_write(uint8_t data) {
	TWDR = data;  // Load data into TWDR
	TWCR = (1 << TWINT) | (1 << TWEN);  // Start transmission
	while (!(TWCR & (1 << TWINT)));  // Wait for TWINT flag
}

void lcd_send(uint8_t value, uint8_t mode) {
	TWI_start();  // Start I2C communication
	TWI_write(LCD_ADDR << 1);  // Write LCD address

	uint8_t data = (value & 0xF0) | mode | 0x08;  // Send upper nibble
	TWI_write(data);
	TWI_write(data | 0x04);  // Pulse the enable bit
	TWI_write(data & ~0x04);

	data = ((value << 4) & 0xF0) | mode | 0x08;  // Send lower nibble
	TWI_write(data);
	TWI_write(data | 0x04);  // Pulse the enable bit
	TWI_write(data & ~0x04);

	TWI_stop();  // Stop I2C communication
}

void lcd_command(uint8_t command) {
	lcd_send(command, 0);
	_delay_ms(2);
}

void lcd_data(uint8_t data) {
	lcd_send(data, 1);
	_delay_ms(2);
}

void lcd_init(void) {
	_delay_ms(50);  // Wait for LCD to power up
	lcd_command(0x02);  // Initialize LCD in 4-bit mode
	lcd_command(0x28);  // Function set: 4-bit mode, 2 lines, 5x8 dots
	lcd_command(0x0C);  // Display on, cursor off
	lcd_command(0x06);  // Entry mode set: Increment cursor
	lcd_command(0x01);  // Clear display
	_delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
	uint8_t address = (row == 0) ? 0x80 : 0xC0;
	address += col;
	lcd_command(address);
}

void lcd_print(char* str) {
	while (*str) {
		lcd_data(*str++);
	}
}

