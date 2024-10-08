# AVR microcontroller implementation of the Liquid Crystal I2C interface
This repository provides an AVR implementation for interfacing with standard character LCD displays (16x2 or 20x4) using the I2C communication protocol. The code is optimized for AVR microcontrollers, offering a lightweight and efficient way to control LCDs without relying on external libraries like the Arduino framework. By using I2C, you can significantly reduce the number of pins required for communication, simplifying wiring and freeing up valuable microcontroller resources.

### Repository Structure

The repository is structured as follows:

- **`main.c`**: This file serves as an example application, demonstrating how to use the LCD I2C library to initialize the LCD, send commands, and display text.

### Code Overview

#### TWI (Two-Wire Interface / I2C) Initialization (`TWI_init()`)

```c
void TWI_init(void) {
    TWSR = 0x00;  // Set prescaler to 1
    TWBR = 0x48;  // Set bit rate for 100kHz I2C clock
    TWCR = (1 << TWEN);  // Enable TWI
}
```
- `TWSR`: The TWI Status Register is configured to set the prescaler for the I2C clock.
- `TWBR`: The TWI Bit Rate Register determines the I2C clock frequency based on the microcontroller's system clock.
- `TWCR`: The TWI Control Register enables the Two-Wire Interface.

#### LCD Initialization (`lcd_init()`)

```c
void lcd_init(void) {
    _delay_ms(50);  // Wait for LCD to power up
    lcd_command(0x02);  // Initialize LCD in 4-bit mode
    lcd_command(0x28);  // Function set: 4-bit mode, 2 lines, 5x8 dots
    lcd_command(0x0C);  // Display on, cursor off
    lcd_command(0x06);  // Entry mode set: Increment cursor
    lcd_command(0x01);  // Clear display
    _delay_ms(2);
}
```

- This function initializes the LCD module by sending a series of commands. 
- It configures the LCD for 4-bit mode, sets the number of lines and font size, turns on the display, hides the cursor, and clears any existing characters.

#### Sending Data and Commands

The `lcd_send()` function handles the transmission of both data and commands to the LCD:

```c
void lcd_send(uint8_t value, uint8_t mode) {
    TWI_start();  // Start I2C communication
    TWI_write(LCD_ADDR << 1);  // Write LCD address

    // Send upper nibble
    uint8_t data = (value & 0xF0) | mode | 0x08;
    TWI_write(data);
    TWI_write(data | 0x04); // Pulse enable bit
    TWI_write(data & ~0x04);

    // Send lower nibble
    data = ((value << 4) & 0xF0) | mode | 0x08;
    TWI_write(data);
    TWI_write(data | 0x04); // Pulse enable bit
    TWI_write(data & ~0x04);

    TWI_stop();  // Stop I2C communication
}
```
- `value`: The data or command to be sent.
- `mode`: Specifies whether the `value` is data (1) or a command (0).

#### Key Functions

- `lcd_command(uint8_t command)`: Sends a command to the LCD module.
- `lcd_data(uint8_t data)`: Sends a character to be displayed on the LCD.
- `lcd_set_cursor(uint8_t row, uint8_t col)`: Sets the cursor position on the LCD.
- `lcd_print(char* str)`: Prints a null-terminated string to the LCD.

#### Connections (ATMega328P Example):

   | I2C LCD | ATMega328P |
   |---------|-------------|
   | VCC     | 5V         |
   | GND     | GND        |
   | SCL     | SCL or A5   |
   | SDA     | SDA or A4   |

### Common LCD Operations

Here are examples of common LCD operations using the provided implementation:

**1. Clear the Display**

```c
lcd_command(0x01); // Send the clear display command (0x01)
```

**2. Set Cursor Position**

To set the cursor to a specific row and column:

```c
lcd_set_cursor(0, 2);  // Set cursor to row 0, column 2
lcd_set_cursor(1, 5);  // Set cursor to row 1, column 5
```

**3. Print a String**

```c
lcd_print("Hello World!"); // Print "Hello World!" at the current cursor position
```

## Contributing

Contributions are welcome! 

- **Bug Fixes:** If you find any bugs or issues, feel free to create an issue or submit a pull request.
- **Feature Enhancements:** If you have ideas for new features or improvements, don't hesitate to share them.

## Further Information

For more detailed information on programming the Arduino Uno with Atmel Studio 7 and implementing the LCD I2C interface, please refer to these articles:

- Programming the Arduino Uno Using Atmel Studio 7: [https://medium.com/@janukaeshan/programming-the-arduino-uno-using-atmel-studio-7-99b0915dec1b](https://medium.com/@janukaeshan/programming-the-arduino-uno-using-atmel-studio-7-99b0915dec1b)
- AVR Microcontroller Implementation of Liquid Crystal I2C Interface: A Comprehensive Guide: [https://medium.com/@janukaeshan/avr-microcontroller-implementation-of-liquid-crystal-i2c-interface-a-comprehensive-guide-8bc4c0ecc857](https://medium.com/@janukaeshan/avr-microcontroller-implementation-of-liquid-crystal-i2c-interface-a-comprehensive-guide-8bc4c0ecc857)

# License

This project is licensed under the [MIT License](LICENSE).



