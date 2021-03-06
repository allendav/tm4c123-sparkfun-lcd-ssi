// Example of connecting the TI Tiva TM4C123 to
// the Sparkfun LCD-14073 LCD via SSI / SPI
// Allen Snook
// 2020 February 15

// Connect PA2/SSI0Clk to SCK
// Connect PA3/SSI0Fss to /CS
// Connect PA5/SSI0Tx to SDI

#include "stdint.h"
#include "tm4c123gh6pm.h"

void LCD_Init()
{
	// Activate SSI0
	SYSCTL_RCGCSSI_R += 0x01;

	// Activate Port A
	SYSCTL_RCGCGPIO_R |= 0x01;

	// Wait until ready
	while ( ( SYSCTL_PRGPIO_R & 0x01 ) == 0 )
	{
	}

	// Enable alternate function on PA2, 3, 5
	GPIO_PORTA_AFSEL_R |= 0x2C;

	// Enable digital on PA2, 3, 5
	GPIO_PORTA_DEN_R |= 0x2C;

	// Configure PA2,3,5 as SSI
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFF0F00FF) | 0x00202200;

	// Disable analog functions on Port A
	GPIO_PORTA_AMSEL_R = 0;

	// Disable SSI, master mode while we configure it
	SSI0_CR1_R = 0x0;

	// Divide 16 MHz bus clock down
	SSI0_CPSR_R = 0xFF;

	// SCR = 0, SPH (CPHA) = 0, SPO (CPOL) = 0 Freescale
	SSI0_CR0_R &= ~(0x0000FFF0);

	// DSS = 8 bit data
	SSI0_CR0_R |= 0x07;

	// Enable SSI
	SSI0_CR1_R = 0x02;
}

void LCD_Out( uint16_t code)
{
	while ((SSI0_SR_R & 0x00000002) == 0)
	{
	}

	SSI0_DR_R = code; // "Right justified"
}

void LCD_Backlight()
{
	LCD_Out( 0x7C ); // Enter setting mode
	LCD_Out( 0x2D ); // Clear display

	LCD_Out( 0x7C ); // Enter setting mode
	LCD_Out( 0x9D ); // Red backlight full

	LCD_Out( 0x7C ); // Enter setting mode
	LCD_Out( 0xBB ); // Green backlight full

	LCD_Out( 0x7C ); // Enter setting mode
	LCD_Out( 0xD9 ); // Blue backlight full

	LCD_Out( 0x52 );
	LCD_Out( 0x65 );
	LCD_Out( 0x61 );
	LCD_Out( 0x64 );
	LCD_Out( 0x79 );
}

int main(void)
{
	LCD_Init();
	LCD_Backlight();

	while ( 1 )
	{
	}
}
