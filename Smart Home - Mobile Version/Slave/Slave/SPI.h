#ifndef SPI_H_
#define SPI_H_

void SPI_Master_init(void);
void SPI_Slave_init(void);
char SPI_Master_transmit(char Data);
char SPI_Slave_receive(char Data);
void SPI_Master_transmit_string(char *ptr);

#endif