#ifndef SONY_H
#define SONY_H



// set direction of pins/ports
int8_t DirectionPortB(uint16_t pin, uint8_t direction);
int8_t DirectionPortC(uint16_t pin, uint8_t direction);
int8_t DirectionPortD(uint16_t pin, uint8_t direction);

void SonyCommand(uint8_t command);


#endif // SONY_H

