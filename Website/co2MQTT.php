<?php

require('phpMQTT.php');
require('displayData.php');

$co2MQTT = new Bluerhinos\phpMQTT('localhost', '1883', 'displayCo2');

if(!$co2MQTT->connect(true, NULL, 'co2usr', 'co2passwd')) exit(1);

$co2AllData = $co2MQTT->subscribeAndWaitForMessage('application/3/device/60df67df0fbbeae4/event/up', 0);

$co2Trame = explode('trame":"', $co2AllData);

// "trame":"1653651855;460;0.00;0.00"}}
$co2Trame = substr($co2Trame[1], 0, -4);

$co2Array = explode(';', $co2Trame);


webDisplayData($co2Array, 'co2');

$co2MQTT->close();

?>