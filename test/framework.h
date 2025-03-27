/* Copyright (C) 2025 Neil Johnson */

/* Define CPU environment */
#define CPUCLK 			( 8000000L )
#define TICKRATE 		( 1000 )
#define TCNT0_INIT 		(0xFF - CPUCLK/256/TICKRATE)
#define TMC8_CK256 		( _BV( CS02 ) )

/* Simulavr pseudo-ports */

/* This port corresponds to the "-W 0x20,-" command line option. */
#define special_output_port (*((volatile char *)0x20))

/* This port corresponds to the "-R 0x22,-" command line option. */
#define special_input_port  (*((volatile char *)0x22))

void debug_putchar(char c) {
   special_output_port = c;
}

/* Send a string out the debug port. */
void debug_puts(const char *str) {
   while(*str)
      debug_putchar(*str);
}


