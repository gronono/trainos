#define PORT 0x3F8
// Fonction pour lire un octet depuis un port d'entrée/sortie spécifié
static inline unsigned char inb(unsigned short port)
{
    unsigned char data;
    __asm__ volatile("inb %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

// Fonction pour écrire un octet vers un port d'entrée/sortie spécifié
static inline void outb(unsigned short port, unsigned char data)
{
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}


static int init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
   return 0;
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(PORT,a);
}

int serial_received() {
   return inb(PORT + 5) & 1;
}

char read_serial() {
   while (serial_received() == 0);

   return inb(PORT);
}

// __attribute__((section(".start")))
void start() {
    // Breakpoint Bochs to test that C code works
    __asm__("xchgw %bx, %bx");
    // DONT work in BOCHS
    init_serial();
    write_serial('H');
    write_serial('e');
    write_serial('l');
    write_serial('l');
    write_serial('o');

    __asm__("xchgw %bx, %bx");
    while(1) {
        outb(0x3F8, 'A');
    }
}
