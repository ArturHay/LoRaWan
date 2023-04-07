<?php

require('phpMQTT.php');
require('displayData.php');

$boopyMQTT = new Bluerhinos\phpMQTT('localhost', 1883, 'boopyID');

if(!$boopyMQTT->connect(true, NULL, 'bpuser', 'bppswd')) exit(1);
$boopyAllData = $boopyMQTT->subscribeAndWaitForMessage('application/3/device/70b3d57ed0048d3e/event/up', 0);

$boopyTrame = explode('trame":"', $boopyAllData);
$boopyTrame = substr($boopyTrame[1], 0, -4);

$boopyArray = explode(';', $boopyTrame);


webDisplayData($boopyArray, 'boopy'); 
$boopyMQTT->close();

?>