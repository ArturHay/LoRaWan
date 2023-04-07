<?php

function webDisplayData($_data, $_node) 
{
    $date = new DateTime();
    $date->setTimestamp(intval($_data[0]));
    echo '<span class="date"><b>'.$date->format('G:i:s') . " (" .$date->format('d-m-y'). ")</b></span>";
    switch($_node){
        case 'co2':
            /*
            mosquitto_pub -h localhost -r -t "application/3/device/60df67df0fbbeae4/event/up" -m "\"trame\":\"1653651855;460;26.3;45.1\"}}}" 
            mosquitto_sub -h 10.121.41.128 -t "application/3/device/60df67df0fbbeae4/event/up"
            Données de noeud CO2: 'timestamp','voc','co2','temperature','humidite';
            // \"trame\":\"1653651855;460;0.00;0.00\"}}
            400 -> 600; 800->1200; >1200
            */
            if($_data[1] <= 600 ) $color = 'green';
            else if($_data[1] <= 1200) $color= 'orange';
            else $color = 'red';
            echo '<p>Qualité de l\'air: <span style="font-weight: bold; color:'. $color .'">'.$_data[1].' ppm</span></p>';
            
            echo '<p>Temperature de l\'air: <b>' .$_data[2]. ' °C</b></p>';
            echo '<p>Pourcentage d\'humidité: <b>' .$_data[3]. ' %</b></p>';
            break;
        case 'boopy':
            /*
            mosquitto_pub -h localhost -r -t "application/3/device/70b3d57ed0048d3e/event/up" -m "1651950109;1838;31.1;41.2;1152.25;47.3146700;5.0266700;243" 
            mosquitto_sub -h 10.121.41.128 -t "application/3/device/70b3d57ed0048d3e/event/up"
            Données de la boopy: 'timestamp','luminosite','temperature','humidite','pressionAtm','longitude','latitude','altitude';
            // mosquitto_pub -h localhost -r -t "application/3/device/70b3d57ed0048d3e/event/up" -m "\"trame\":\"1655117927;1838;31.1;41.2;1152.25;47.3146700;5.0266700;243\"}}}"
            */
            echo '<p>Luminosité: <b>' .$_data[1]. ' lux</b></p>';
            echo '<p>Temperature: <b>' .$_data[2]. ' °C</b></p>';
            echo '<p>Pourcentage d\'humidité: <b>' .$_data[3]. ' %</b></p>';
            echo '<p>Pression atmospherique: <b>' .$_data[4]. ' hpa</b></p>';
            echo '<p>Longitude: <b>'.$_data[5].'°</b></p>';
            echo '<p>Latitude: <b>'.$_data[6].'°</b></p>';
            echo '<p>Altitude: <b>'.$_data[7].' m</b></p>';
            break;
        default:
            return -1;
    }
}
?>