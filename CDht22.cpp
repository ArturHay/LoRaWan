#include "CDHT22.h"

DHT22::DHT22()
{
    humidity = 0;
    checksum = 0;
    celsius = 0;
	fahrenheit = 0;
    m_signal = 17;
    for(int i = 0; i<5; i++)
    {
        m_data[i] = 0;
    }

}

DHT22::~DHT22()
{
}
short DHT22::readData()
{
    unsigned short val = 0x00;
    unsigned short signal_length = 0;
    unsigned short val_counter = 0;
    unsigned short loop_counter = 0;
	while (1)
	{
		// Count only HIGH signal
		while (digitalRead(m_signal) == HIGH)
		{
			signal_length++;

			// When sending data ends, high signal occur infinite.
			// So we have to end this infinite loop.
			if (signal_length >= 200)
			{
				return -1;
			}

			delayMicroseconds(1);
		}

		// If signal is HIGH
		if (signal_length > 0)
		{
			loop_counter++;	// HIGH signal counting

			// The DHT22 sends a lot of unstable signals.
			// So extended the counting range.
			if (signal_length < 10)
			{
				// Unstable signal
				val <<= 1;		// 0 bit. Just shift left
			}

			else if (signal_length < 30)
			{
				// 26~28us means 0 bit
				val <<= 1;		// 0 bit. Just shift left
			}

			else if (signal_length < 85)
			{
				// 70us means 1 bit
				// Shift left and input 0x01 using OR operator
				val <<= 1;
				val |= 1;
			}

			else
			{
				// Unstable signal
				return -1;
			}

			signal_length = 0;	// Initialize signal length for next signal
			val_counter++;		// Count for 8 bit data
		}

		// The first and second signal is DHT22's start signal.
		// So ignore these data.
		if (loop_counter < 3)
		{
			val = 0x00;
			val_counter = 0;
		}

		// If 8 bit data input complete
		if (val_counter >= 8)
		{
			// 8 bit data input to the data array
			m_data[(loop_counter / 8) - 1] = val;

			val = 0x00;
			val_counter = 0;
		}
	}
}

void DHT22::Temperature()
{

        pinMode(m_signal, OUTPUT);
		// Send out start signal
		digitalWrite(m_signal, LOW);
		delay(20);					// Stay LOW for 5~30 milliseconds
		pinMode(m_signal, INPUT);		// 'INPUT' equals 'HIGH' level. And signal read mode

		readData();		// Read DHT22 signal

		// The sum is maybe over 8 bit like this: '0001 0101 1010'.
		// Remove the '9 bit' data using AND operator.
		checksum = (m_data[0] + m_data[1] + m_data[2] + m_data[3]) & 0xFF;

		// If Check-sum data is correct (NOT 0x00), display humidity and temperature
		if (m_data[4] == checksum && checksum != 0x00)
		{

			celsius = (((m_data[2] & 0x7F)*256) + m_data[3]) / 10.0;
			if (m_data[2] == 0x80)
			{
				celsius *= -1;
			}

			fahrenheit = ((celsius * 9) / 5) + 32;
		}
}


void DHT22::Humidity()
{
		pinMode(m_signal, OUTPUT);

		// Send out start signal
		digitalWrite(m_signal, LOW);
		delay(20);					// Stay LOW for 5~30 milliseconds
		pinMode(m_signal, INPUT);		// 'INPUT' equals 'HIGH' level. And signal read mode

		readData();		// Read DHT22 signal

		// The sum is maybe over 8 bit like this: '0001 0101 1010'.
		// Remove the '9 bit' data using AND operator.
		checksum = (m_data[0] + m_data[1] + m_data[2] + m_data[3]) & 0xFF;

		// If Check-sum data is correct (NOT 0x00), display humidity and temperature
		if (m_data[4] == checksum && checksum != 0x00)
		{
			// * 256 is the same thing '<< 8' (shift).
			humidity = ((m_data[0] * 256) + m_data[1]) / 10.0;
		}
}



void DHT22::checkgpio()
{
	if (wiringPiSetupGpio() == -1)
	{
		printf("[x_x] GPIO Initialization FAILED.\n\n");
	}
	else
    {
        printf("GPIO Initialization SUCCEED.\n\n");
    }
}

float DHT22::getHumidity()
{
    return(humidity);
}

float DHT22::getCelsius()
{
    Humidity();
    return(celsius);
}

float DHT22::getFahrenheit()
{
    return(fahrenheit);
}

