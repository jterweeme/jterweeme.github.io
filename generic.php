<?php
class CArray
{
    protected $array;
    public function __construct() { $this->array = array(); }
    public function push($s) { array_push($this->array, $s); }
    public function size() { return count($this->array); }
    public function element($i) { return $this->array[$i]; }
    
    public function __toString()
    {
        $string = "";

        if ($this->size() > 0)
            foreach ($this->array as $element)
                $string .= "$element\n";
        
        return (String)$string;
    }
}
?>


