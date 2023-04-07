<?php

function createSQLRequest($_table, $_data, $_column) {
    $insert = "INSERT INTO ".$_table."(";
    for($i = 0; $i < count($_column); $i++) {
        if(count($_column) - 1 > $i) $insert .= $_column[$i].", ";
        else $insert .= $_column[$i];
    }
    $insert .= ") VALUES(";
    for($i = 0; $i < count($_data); $i++) {
        if(count($_data) - 1 > $i) $insert .= "'".$_data[$i]."',";
        else $insert .= "'".$_data[$i]."'";
    }
    $insert .= ");";
    return $insert;
}


// $_data = tableau de valeur a entrer
// column = tableau du nom de valeur ex: array('timestamp','co2','temperature','humidite')
// $_table = nom de la table
function sendDataToDatabase($_table, $_data, $_column){
    if(count($_column) != count($_data)) return -1;
    $request = createSQLRequest($_table, $_data, $_column);
    global $bdd;
    $bdd->exec($request);
}


// Connexion à la base de donnée
$bdd = new SQLite3('/home/pi/databases/plateformeiot.db');

?>