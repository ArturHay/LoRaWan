#ifndef CMICS_H_INCLUDED
#define CMICS_H_INCLUDED
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include <iostream>


class MICS{

private:
    enum Command : uint8_t {
    SetPPMCO2     = 0b000001000,
    GetStatus     = 0b000001100,
    GetRevision   = 0b000001101,
    GetR0         = 0b000010000};
    int i2c_fd;
public:
    MICS();
    ~MICS();
    bool openBus();
    bool closeBus();
    bool getValues(float &co2);
private:
    bool writeData(const uint8_t *data, int size);
    bool readData(uint8_t *data, int size);
    uint8_t getCRC(const uint8_t *buffer, const uint8_t size);
    bool writeCommand(const Command cmd, const uint32_t data );
    bool readResult(uint8_t data[7]) ;



};


#endif
