<?php
require('CNode.php');
//                      $_server, $_port, $_username, $_password, $_id, $_topic, $_nodeName
$co2Node = new CNode('localhost', 1883, '','','co2Id', 'application/3/device/60df67df0fbbeae4/event/up', 'co2', 
                    array('timestamp', 'co2', 'temperature', 'humidite'));

$boopyNode = new CNode('localhost', 1883, '', '', 'boopyId', 'application/3/device/70b3d57ed0048d3e/event/up', 'boopy', 
                    array('timestamp','luminosite','temperature','humidite',
                    'pressionAtm','longitude','latitude','altitude'));

$waitingTime = 5 * 60; // 10 * 60 pour 10 minutes

$co2Node->goConnect();
$boopyNode->goConnect();

while(true) {
        
    $co2Node->waitMessage();

    $boopyNode->waitMessage(); 
    
    // Attendre 
    sleep($waitingTime);
}
?>