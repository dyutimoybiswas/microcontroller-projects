#ifndef SPI_CONTROL_H
#define SPI_CONTROL_H

#define R3				(1U << 3)			
#define PD0				(1U << 0)			// clock
#define PD1				(1U << 1)			// CS
#define PD2				(1U << 2)			// Rx (MOSI)
#define PD3				(1U << 3)			// Tx (MISO)
#define RINT			(1U << 2)			// receive interrupt
#define SSE				(1U << 1)			// SSI enabled
#define BUSY			(1U << 4)			// busy bit
#define RNE				(1U << 2)			// receive FIFO not empty
#define TFN				(1U << 1)			// transmit FIFO not full

void spi_receive(void);
void spi_setup(void);
void spi_write(void);

static unsigned char data;
static const char msg_t = 'R';

#endif
