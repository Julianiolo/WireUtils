#include "WireUtils.h"

int WireUtils::read_delay_ms = 10;

uint16_t WireUtils::read16(TwoWire* wire, uint8_t device_addr, uint8_t register_addr, uint8_t* error) {
    wire->beginTransmission(device_addr);
    wire->write(register_addr);
    uint8_t ret = wire->endTransmission(false);
    if(ret != 0) {
        if(error != NULL) *error = ret;
        return 0;
    }
    wire->requestFrom(device_addr, (uint8_t)2); // returns number of bytes available or something? Anyway not an error
    
    uint8_t buf[2] = {0};

    for (uint8_t i = 0; i < 2; i++) {

        for(uint8_t t = 0; t < WireUtils::read_delay_ms; t++) {
            if(wire->available())
                break;
            delay(1);
        }

        if (!wire->available()) {
            if(error != NULL) *error = 6;
            return 0;
        }

        int v = wire->read();
        if(v == -1) {  // some read error occured
            if(error != NULL) *error = 7;
            return 0;
        } 
        buf[i] = (uint8_t)v;
    }

    return ((uint16_t)buf[1] << 8) | buf[0];
}

uint8_t WireUtils::write16(TwoWire* wire, uint8_t device_addr, uint8_t register_addr, uint16_t value, uint16_t mask) {
    uint16_t actual_value;
    if(mask == 0xffff) {
        actual_value = value;
    }else {
        uint8_t error = 1;
        uint16_t current_value = read16(wire, device_addr, register_addr, &error);
        if(error != 0) return error;
        
        actual_value = current_value;
        actual_value &= ~mask;
        actual_value |= value & mask;
    }
    
    wire->beginTransmission(device_addr);

    wire->write(register_addr);
    wire->write((uint8_t)value);
    wire->write((uint8_t)(value >> 8));

    return wire->endTransmission(true);
}

uint8_t WireUtils::write16Confirm(TwoWire* wire, uint8_t device_addr, uint8_t register_addr, uint16_t value, uint16_t mask) {
    uint8_t ret = WireUtils::write16(wire, device_addr, register_addr, value, mask);
    if(ret != 0) return ret;

    uint8_t error = 1;
    uint16_t result = read16(wire, device_addr, register_addr, &error);
    
    if(error != 0)
        return error;
    
    if(result != value)
        return 8;

    return 0;
}