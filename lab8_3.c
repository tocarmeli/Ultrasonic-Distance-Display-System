#include <avr/io.h>
#include <util/delay.h>
#define PERSISTENCE 1
#define COUNTTIME 1000
#define TRIG PC5
#define ECHO PC4
#define RANGE_PER_CLOCK 1.098


void timer0_init(){
    TCCR0A = 0;
    TCCR0B = 5;
    TCNT0 = 0;
}

int main(void){
    // 7 Segment declarations and Ultrasonic declarations
    unsigned char ledDigits[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};
    unsigned char DIG1, DIG2, DIG3, DIG4;
    unsigned char rising_edge_clocks, falling_edge_clocks, echo_width_clocks;
    float target_range;
    DDRC = 1<<TRIG;         // TRIG is output pin;
    PORTC &= ~(1<<TRIG);    // Set the TRIG pin low
    timer0_init();
    DDRD = 0xff;
    DDRB = 0xff;
    PORTB = 0xff;


    while(1){
        TCNT0 = 0;          // Load counter with 0
        PORTC |= 1<<TRIG;   // These three lines of code
        _delay_us(10);      // Put a 10 usec pulse on the
        PORTC &= ~(1<<TRIG);// TRIG pin.

        // Wait till the ECHO pulse goes high
        while ((PINC & (1<<ECHO)) ==0);
        rising_edge_clocks = TCNT0; // Note the time
        // Now wait till the ECHO pulse goes low
        while (!(PINC & (1<<ECHO))==0);
        falling_edge_clocks = TCNT0;
        
        if (falling_edge_clocks > rising_edge_clocks){
            // Compute target range and send it to the serial monitor
            echo_width_clocks = falling_edge_clocks - rising_edge_clocks;
            target_range = echo_width_clocks * RANGE_PER_CLOCK;
        }
       // _delay_ms(500);  // Delay then go again
        
        int range = (int) target_range; // Converting range to int to display easier
        int j;
        // Retinal Persistence in order to display 4 different digits on the 7 segment display
        for (j = 0; j < COUNTTIME / PERSISTENCE / 4; j++){
            // Displays the "ones" place digit (furthest to the right)
            DIG4 = range % 10;
            PORTD = ledDigits[DIG4];
            PORTB = ~ (1<<1);
            _delay_ms(PERSISTENCE);

            // Displays the "tens" place digit (second furthest to the right)
            DIG3 = (range / 10) % 10;
            PORTD = ledDigits[DIG3];
            PORTB = ~(1<<2);
            _delay_ms(PERSISTENCE);

            // Displays the "hundreds" place digit (second closest to the left)
            DIG2 = (range / 100) % 10;
            PORTD = ledDigits[DIG2];
            PORTB = ~(1<<3);
            _delay_ms(PERSISTENCE);

            // Displays the "thousands" place digit (closest digit to the left)
            DIG1 = (range / 1000);
            PORTD = ledDigits[DIG1];
            PORTB = ~(1<<4);
            _delay_ms(PERSISTENCE);
        }
        
    }
}