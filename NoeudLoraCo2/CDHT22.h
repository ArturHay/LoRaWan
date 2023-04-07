#ifndef CDHT22_H
#define CDHT22_h
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <iostream>
#include <string.h>
#include <strings.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>

class DHT22
{
    private:
        float humidity;
        short checksum;
        float celsius;
        float fahrenheit;
        short readData();
        unsigned short m_signal;
        unsigned short m_data[5];
    public:
        DHT22();
        virtual ~DHT22();
        void Humidity();
        void Temperature();
        float getFahrenheit();
        float getCelsius();
        float getHumidity();
        void checkgpio();



};

#endif
