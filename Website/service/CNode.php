<?php
require('phpMQTT.php');
require('CSaveToCsv.php');
require('bdd.php');

class CNode extends Bluerhinos\phpMQTT{
    
    protected $username;
    protected $password;
    private $topic;
    private $nodeName;
    private $column;
    private $data;
    private $csv;
    
    public function __construct($_server, $_port, $_username, $_password, 
                                $_id, $_topic, $_nodeName, $_column) {

        parent::__construct($_server, $_port, $_id);
        
        $this->username = $_username;
        $this->password = $_password;
        
        $this->topic = $_topic;
        $this->nodeName = $_nodeName;
        $this->column = $_column;
        
        $this->csv = new CSV($this->nodeName, $this->column);
    }
    
    private function trame($_data) {
        $trame = explode('trame":"', $_data);
        $trame = substr($trame[1], 0, -4);

        return $trame;
    }

    private function useData() {
        // Créer un tableau de donnée
        $arrayData = explode(";", $this->data);
        // Sauvegarder dans le CSV
        $this->csv->saveData($arrayData);
        // Sauvegarder dans la base de donnée
        sendDataToDatabase($this->nodeName, $arrayData, $this->column);
    }
    
    public function goConnect() {
        if(!parent::connect(true, NULL, $this->username, $this->password)) exit(1);
    }

    public function waitMessage() {
        $allData = parent::subscribeAndWaitForMessage($this->topic, 0);
        $this->data = $this->trame($allData);
        $this->useData();
        //parent::close();
    }
}
?>
