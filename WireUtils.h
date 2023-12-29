#ifndef __WIREUTILS_H__
#define __WIREUTILS_H__


#include <Arduino.h>
#include <Wire.h>


namespace WireUtils {
    extern int read_delay_ms; // default 10

    /*
        Reads the value at register_addr

        waits for WireUtils::read_delay_ms milliseconds before reading each byte

        Returns same errors as Wire endTransmission function + no answer + wire->read error
        https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
        endTransmission() returns:
        0: success.
        1: data too long to fit in transmit buffer.
        2: received NACK on transmit of address.
        3: received NACK on transmit of data.
        4: other error.
        5: timeout
        6: no answer
        7: wire->read error
    */
    uint16_t read16(TwoWire* wire, uint8_t device_addr, uint8_t register_addr, uint8_t* error = NULL);

    /*
        Returns same errors as read16 (only errors of endTransmission() if mask unused)
        https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
        endTransmission() returns:
        0: success.
        1: data too long to fit in transmit buffer.
        2: received NACK on transmit of address.
        3: received NACK on transmit of data.
        4: other error.
        5: timeout
        6: no answer
        7: wire->read error
    */
    uint8_t write16(TwoWire* wire, uint8_t device_addr, uint8_t register_addr, uint16_t value, uint16_t mask = 0xffff);
    /*
        Reads back the written value to confirm it was written properly. (Returns 6: unexpected value read)

        Returns same errors as read16 (only errors of endTransmission() if mask unused)
        https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
        endTransmission() returns:
        0: success.
        1: data too long to fit in transmit buffer.
        2: received NACK on transmit of address.
        3: received NACK on transmit of data.
        4: other error.
        5: timeout
        6: no answer
        7: wire->read error
        8: unexpected value read
    */
    uint8_t write16Confirm(TwoWire* wire, uint8_t device_addr, uint8_t register_addr, uint16_t value, uint16_t mask = 0xffff);
}

#endif