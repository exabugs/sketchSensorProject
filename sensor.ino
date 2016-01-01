extern "C"{
#include <spi.h>
#include <spi_register.h>
}

#include <Ticker.h>

Ticker ticker;
Ticker ticker2;

void setup() {
  Serial.begin(115200);
  Serial.print("\n");

  spi_init(HSPI);

  ticker.attach_ms(1000, timer);
  ticker2.attach_ms(5, Angularity);
}

void loop() {
}

volatile uint32 angularity = 0;
void Angularity() {
  uint32 val = check(3);
  if (val > angularity) {
    angularity = val;
  }
}

void timer() {

  uint32 val0 = check(0);
  uint32 val1 = check(1);
  uint32 val2 = check(2);
  //uint32 val3 = check(3);
  uint32 val3 = angularity;
  angularity = 0;
  uint32 val4 = check(4);

  Serial.print(" Temperature:");
  Serial.print(val0);
  Serial.print(" Illuminance:");
  Serial.print(val1);
  Serial.print(" Humidity:");
  Serial.print(val2);
  Serial.print(" Angularity:");
  Serial.print(val3);
  Serial.print(" Test:");
  Serial.print(val4);
  Serial.print("\n");

}

uint32 check(int channel) {
    // start bit (1 bit) : 1
    // SGL/DIFF bit (1 bit) : SGL:1
    //  select the input channel (3 bit) : CH0:0, CH1:1
    uint8 cmd = (0b11 << 3) | channel;
  // uint32 spi_transaction(
  //            uint8  spi_no,
  //            uint8  cmd_bits,    // 0
  //            uint16 cmd_data,    // 0
  //            uint32 addr_bits,   // 0
  //            uint32 addr_data,   // 0
  //            uint32 dout_bits,   // COMMAND_LENGTH
  //            uint32 dout_data,   // cmd
  //            uint32 din_bits,    // RESPONSE_LENGTH
  //            uint32 dummy_bits   // 0
  //         );

  const uint32 COMMAND_LENGTH = 5;
  const uint32 RESPONSE_LENGTH = 12;

  uint32 retval = spi_transaction(HSPI, 0, 0, 0, 0, COMMAND_LENGTH, cmd, RESPONSE_LENGTH, 0);
  retval = retval & 0x3FF; // mask to 10-bit value
  return retval;
}

