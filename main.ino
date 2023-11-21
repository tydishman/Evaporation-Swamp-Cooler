
/*
* @author Tyler Dishman
* @author Matthew Gaskell
*/

// https://www.electronicshub.org/wp-content/uploads/2021/01/Arduino-Mega-Pinout.jpg
// https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf


enum State {
    DISABLED,
    IDLE,
    ERROR,
    RUNNING
};
enum Color {
    Red,
    Green,
    Blue,
    Yellow
};

//UART Definitions 
#define RDA 0x80
#define TBE 0x20
//UART Variables
volatile unsigned char *myUCSR0A = (unsigned char*)0x00C0;
volatile unsigned char *myUSCR0B = (unsigned char*)0x00C1;
volatile unsigned char *myUSCR0C = (unsigned char*)0x00C2;
volatile unsigned int *myUBBR0 = (unsigned int*)0x00C4;
volatile unsigned char *myUDR0 = (unsigned char*)0x00C6;

// LED pins
volatile unsigned byte* PORTA = (unsigned byte*) 0x02;
volatile unsigned byte* DDRA = (unsigned byte*) 0x01;
volatile unsigned byte* PINA = (unsigned byte*) 0x00;
// PA0:3 will be used for the LEDs
// Yellow, Red, Green. Blue

// Status register
volatile unsigned byte *mySREG = (unsigned byte*) 0x3f;

State currentState = DISABLED;


void setup(){
    *DDRA = 0b00001111; // sets those pins as outputs
}
void loop(){

    // use a switch-case block to do state management
    switch (currentState)
    {
    case DISABLED:
        break;
    case IDLE:
        break;
    case ERROR:
        break;
    case RUNNING:
        break;
    
    default:
        break;
    }
    driveLED(currentState);
    // ALL STATES OTHER THAN DISABLED
    if(currentState != DISABLED){
        /*
        – Humidity and temperature should be continuously monitored and reported on the LCD screen. Updates should occur once per minute.
        – System should respond to changes in vent position control
        – Stop button should turn fan motor off (if on) and system should go to DISABLED state
        */
    }

    // ALL STATES
    /*
    * The realtime clock must be used to report (via the Serial port) the time of each state transition, and any changes to the stepper motor position for the vent.
    */
}

// Helper functions
Color driveLED(State currState){
    switch (currState)
    {
    case DISABLED:
        // Yellow LED on
        *PORTA &= 0b11110001; // turn other colors off
        *PORTA |= 0b11110001; // set yellow LED
        break;
    case IDLE:
        // Green LED on
        *PORTA &= 0b11110100;
        *PORTA |= 0b11110100; // set green LED
        break;
    case ERROR:
        // Red LED on
        *PORTA &= 0b11110010;
        *PORTA |= 0b11110010; // set red LED
        break;
    case RUNNING:
        // Blue LED on
        *PORTA &= 0b11111000;
        *PORTA |= 0b11111000; // set blue LED
        break;
    }
}

// external interrupt for a state change on INT5 (D3)
ISR(INT5_vect){ 
    byte statusReg = *mySREG;

    // do interrupt stuff

    *mySREG = statusReg;
}

//Functions for the UART
void U0Init(int U0baud){
    unsigned long FCPU = 16000000;
    unsigned long tbaud;
    tbaud = (FCPU / 16 / U0baud - 1);
    *myUCSR0A = 0x20;
    *myUSCR0B = 0x18;
    *myUSCR0C = 0x06;
    *myUBBR0 = tbaud;
}

unsigned char kbhit(){
    return *myUCSR0A & RDA;
}

unsigned char getChar(){
    return *myUDR0;
}

void putChar(unsigned char U0data){
    while((*myUCSR0A & TBE) == 0);
    *myUDR0 = U0data;
}