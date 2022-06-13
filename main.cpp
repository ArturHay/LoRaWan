#include "CDHT22.h"
#include "CMICS.h"


using namespace std;

//conversion hexadécimal en string 
void get_hex_string(char *buff, int buff_len, char *ret)
{
    int j;  //index buff
    int i;  //index string
    char nibble;
    const char hex_map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (i = 0, j = 0; i < buff_len*2; i++)
    {
        nibble = 20; 

        if (i % 2 == 0)
            nibble = (buff[j] >> 4) & 0x0F;
        else
        {
            nibble = buff[j] & 0x0F;
            j++;
        }
        ret[i] = hex_map[nibble];
    }
    if(strlen(ret) != buff_len*2) {
        for(int h=buff_len*2; h<strlen(ret); h++) {
            ret[h] = '\0';
        }
    }
    return;
}

// création des trames en format "string"
string createTrame(float co2, float temperature, float humidite) {
    std::stringstream toString;

    toString << std::fixed << std::setprecision(0) << co2;
    std::string strCo2 = toString.str();
    toString.str("");

    toString << std::fixed << std::setprecision(2) << temperature;
    std::string strTemp = toString.str();
    toString.str("");

    toString << std::fixed << std::setprecision(2) << humidite;
    std::string strHum = toString.str();
    toString.str("");

    string strTrame = "";
    strTrame = std::to_string((int)std::time(nullptr)) + ";" + strCo2 + ";" + strTemp + ";" + strHum;


    return strTrame;
}

// Configuration de la connexion a chirpStack 
void config()
{
    int serial = serialOpen("/dev/ttyAMA0",57600);
    printf("Ouverture port serie... Port=%d\nDebut des configurations...\n", serial);
    serialPuts(serial, "sys reset\r\n");
    delay(2000);
    serialPuts(serial, "mac reset 868\r\n");
    delay(500);
    serialPuts(serial, "mac set devaddr 00e3410f\r\n");
    delay(500);
    serialPuts(serial, "mac set appskey 7a035253b777757ddc9e265008b89d91\r\n");
    delay(500);
    serialPuts(serial, "mac set nwkskey 798a0b096b4c36736fe215a471ebb965\r\n");
    delay(500);
    serialPuts(serial, "mac deveui 60df67df0fbbeae4\r\n");
    delay(500);
    serialPuts(serial, "mac join abp\r\n");
    delay(500);
    printf("Fin des configurations...\nCreation de la trame...\n");
}

int main(void)
{
    DHT22 *ctempHum = new DHT22();
    MICS *cco2 = new MICS();
    float co2 = 0.0f;
    ctempHum->checkgpio();
    ctempHum->Humidity();
    ctempHum->Temperature();
    cco2->openBus();
    cco2->getValues(co2);
    printf("\nco2: %.0lf ppm\n",co2);
    float Celsius = ctempHum->getCelsius();
    float Humidity = ctempHum->getHumidity();
    printf("\nCelsius: %6.2f *C\n\nHumidity: %6.2f %\n\n",Celsius,Humidity);
    cco2->closeBus();

    int serial;
    string trameStr;
    serial = serialOpen("/dev/ttyAMA0",57600);
    config();

    trameStr = createTrame(co2, Celsius, Humidity);
    char donnees[trameStr.length()+1];

    strcpy(donnees, trameStr.c_str());

    printf("donnees: %s\n", donnees);
    char payload[strlen(donnees)*2];

    char *trame = new char();

    get_hex_string(donnees,strlen(donnees),payload);

    strcpy(trame, "mac tx cnf 4 ");
    strcat(trame, payload);
    strcat(trame, "\r\n");

    printf("\n------Trame:------\n\n%s\n",trame);

    serialPuts(serial, trame);
    delay(3500);
    close(serial);

    return 0;
}
