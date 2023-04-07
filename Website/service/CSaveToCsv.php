<?php
class CSV {

    private $nodeName;
    private $column;
    private $csvPath;
    public function __construct($_nodeName, $_column) {
        $this->nodeName = $_nodeName;
        $this->column = $_column;
        $this->csvPath = '/home/pi/databases/'.$this->nodeName.'.csv';
    }
    /*
    $nodeName correspond à $name de la class Node
    $column correspond à un tableau de valeur ($column de la class CTable)
    timestamp, voc, co2, temperature, humidite
    /!\Notre fichier .csv sera au nom du noeud.csv /!\
    */

    /*
    $_data correspond à a un tableau de chaine de caractère
    */
    public function saveData($_data) {
        if(file_exists($this->csvPath)) {
            $file = fopen($this->csvPath, 'a');
            fputcsv($file, $_data, ";");
            fclose($file);
        } else $this->createCsvFile($_data);
    }

    private function createCsvFile($_data) {
        $file = fopen($this->csvPath, 'a');
        fputcsv($file, $this->column, ";");
        fclose($file);
        $this->saveData($_data);
    }
}
?>