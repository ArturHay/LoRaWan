// Inclure les bibliothèques nécessaires pour le capteur DHT22 et le capteur MICS
#include "CDHT22.h"
#include "CMICS.h"

// Utiliser l'espace de noms standard
using namespace std;

// Fonction pour convertir un tableau d'octets (buff) en chaîne de caractères représentant les valeurs hexadécimales
void get_hex_string(char *buff, int buff_len, char *ret)
{
    // Déclaration des indices des tableaux buff et ret
    int j; // index buff
    int i; // index string
    char nibble;
    const char hex_map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    // Parcourir le tableau buff et convertir chaque octet en deux caractères hexadécimaux
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
    // Supprimer les caractères supplémentaires dans la chaîne ret, si nécessaire
    if(strlen(ret) != buff_len*2) {
        for(int h=buff_len*2; h<strlen(ret); h++) {
            ret[h] = '\0';
        }
    }
    return;
}

// Fonction pour créer une trame de données au format "string" à partir des valeurs de CO2, température et humidité
string createTrame(float co2, float temperature, float humidite) {
    std::stringstream toString;

    // Convertir les valeurs en chaînes de caractères et les stocker dans des variables
    toString << std::fixed << std::setprecision(0) << co2;
    std::string strCo2 = toString.str();
    toString.str("");

    toString << std::fixed << std::setprecision(2) << temperature;
    std::string strTemp = toString.str();
    toString.str("");

    toString << std::fixed << std::setprecision(2) << humidite;
    std::string strHum = toString.str();
    toString.str("");

    // Créer la trame finale en concaténant les chaînes de caractères avec des séparateurs ";"
    string strTrame = "";
    strTrame = std::to_string((int)std::time(nullptr)) + ";" + strCo2 + ";" + strTemp + ";" + strHum;

    return strTrame;
}

// Fonction pour configurer la connexion au réseau LoRaWAN (ChirpStack) 
void config()
{
    // Ouvrir le port série et initialiser la configuration
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
    // Instancier les objets pour les capteurs de température et d'humidité et le capteur de CO2
    DHT22 *ctempHum = new DHT22();
    MICS *cco2 = new MICS();
    // Initialiser la valeur de CO2 à 0
    float co2 = 0.0f;
    // Vérifier les broches GPIO pour le capteur de température et d'humidité
    ctempHum->checkgpio();
    // Mesurer l'humidité avec le capteur de température et d'humidité
    ctempHum->Humidity();
    // Mesurer la température avec le capteur de température et d'humidité
    ctempHum->Temperature();
    // Ouvrir le bus pour le capteur de CO2
    cco2->openBus();
    // Mesurer la valeur de CO2 avec le capteur de CO2
    cco2->getValues(co2);
    printf("\nco2: %.0lf ppm\n",co2);
    // Récupérer la température en Celsius
    float Celsius = ctempHum->getCelsius();
    // Récupérer l'humidité relative
    float Humidity = ctempHum->getHumidity();
    printf("\nCelsius: %6.2f *C\n\nHumidity: %6.2f %\n\n",Celsius,Humidity);
    // Fermer le bus pour le capteur de CO2
    cco2->closeBus();

    // Ouvrir une connexion série
    int serial;
    // Déclarer une variable pour stocker la trame
    string trameStr;
    serial = serialOpen("/dev/ttyAMA0",57600);
    // Configurer la connexion au réseau LoRaWAN
    config();

    // Créer la trame de données à partir des valeurs des capteurs
    trameStr = createTrame(co2, Celsius, Humidity);
    // Convertir la trame de données en une chaîne de caractères C
    char donnees[trameStr.length()+1];
    strcpy(donnees, trameStr.c_str());

    printf("donnees: %s\n", donnees);
    // Déclarer une variable pour stocker la trame en hexadécimal
    char payload[strlen(donnees)*2];

    // Déclarer un pointeur de char pour stocker la trame
    char *trame = new char();
    // Convertir la trame de données en une chaîne de caractères hexadécimaux
    get_hex_string(donnees,strlen(donnees),payload);

    // Construire la trame finale en ajoutant les informations nécessaires pour envoyer la trame via LoRaWAN
    strcpy(trame, "mac tx cnf 4 ");
    strcat(trame, payload);
    strcat(trame, "\r\n");

    printf("\n------Trame:------\n\n%s\n",trame);

    // Envoyer la trame via la connexion série
    serialPuts(serial, trame);
    // Attendre 3,5 secondes pour recevoir la réponse du serveur
    delay(3500);
    // Fermer la connexion série
    close(serial);

    return 0;
}
