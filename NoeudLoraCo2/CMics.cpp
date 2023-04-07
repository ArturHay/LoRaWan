#include "CMICS.h"

#define cChipAddress 0x70

MICS::MICS()
{
    i2c_fd = 0 ;
}
MICS::~MICS()
{
}
bool MICS::openBus()
{
    const std::string cDevicePath = "/dev/i2c-1";
    i2c_fd = open(cDevicePath.c_str(), O_RDWR);
    if (i2c_fd < 0) {
        return false;
    }
    if (ioctl(i2c_fd, I2C_SLAVE, cChipAddress) < 0) {
        return false;
    }
    return true;
}

bool MICS::closeBus()
{
    close(i2c_fd);
    return true;
}

bool MICS::writeData(const uint8_t *data, int size)
{
    if (write(i2c_fd, data, size) != size) {
        return false;
    }
    return true;
}

bool MICS::readData(uint8_t *data, int size)
{
    if (read(i2c_fd, data, size) != size) {
        return false;
    }
    return true;
}

uint8_t MICS::getCRC(const uint8_t *buffer, const uint8_t size)
{
    uint16_t sum = 0;
    for (uint8_t i=0; i<size; i++) {
        sum += buffer[i];
    }
    uint8_t crc = static_cast<uint8_t>(sum);
    crc += (sum / 0x0100);
    crc = (0xff-crc);
    return crc;
}

bool MICS::writeCommand(const Command cmd, const uint32_t data = 0)
{
    uint8_t buffer[6];
    buffer[0] = static_cast<uint8_t>(cmd);
    buffer[1] = static_cast<uint8_t>(data & 0x000000ffUL);
    buffer[2] = static_cast<uint8_t>((data & 0x0000ff00UL)>>8);
    buffer[3] = static_cast<uint8_t>((data & 0x00ff0000UL)>>16);
    buffer[4] = static_cast<uint8_t>((data & 0xff000000UL)>>24);
    buffer[5] = getCRC(buffer, 5);
    const auto result = writeData(buffer, 6);
    if (!result) {
    }
    return result;
}

bool MICS::readResult(uint8_t data[7])
{
    if (!readData(data, 7)) {
        return false;
    }
    const uint8_t crc = getCRC(data, 6);
    if (data[6] != crc) {
        return false;
    }
    return true;
}

bool MICS::getValues(float &co2)
{

    // Repeat the read three times if necessary.
    for (uint8_t comTry = 0; comTry < 3; ++comTry) {
        Command status = GetStatus;
        if (writeCommand(status,0)) {
            const std::chrono::milliseconds cReadDelay(300);
            std::this_thread::sleep_for(cReadDelay);
            uint8_t data[7];
            if (readResult(data)) {
                co2 = ((data[1] - 13) * (1600 / 229) + 400);
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    return false;
}




