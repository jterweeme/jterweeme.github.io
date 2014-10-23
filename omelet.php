<?php

class Omelet
{
    private $buf;
    
    public function __construct($file)
    {
        $uxf = simplexml_load_file($file);

        foreach ($uxf->element as $a)
        {
            if ($a->type == "com.umlet.element.Class")
            {
                $b = $a->coordinates;

                $this->buf .= sprintf(
                    "<ul style=\"left: %upx; top: %upx; width: %upx; height: %upx;\">\n",
                    $b->x, $b->y, $b->w, $b->h);
        
                $c = explode("--", $a->panel_attributes);
        
                foreach ($c as $d)
                {
                    if (preg_match("/\**\*/", $d))
                        $this->buf .= sprintf("<li><b>%s</b></li>", $d);
                    else
                        $this->buf .= sprintf("<li>%s</li>", $d);
                }
        
                $this->buf .= sprintf("</ul>\n");
            }
        
            if ($a->type == "com.umlet.element.Relation")
            {
                $b = $a->coordinates;

                $this->buf .= sprintf(
                    "<figure style=\"left: %upx; top: %upx; width: %upx; height: %upx;\">\n",
                    $b->x, $b->y, $b->w, $b->h);
                
                $this->buf .= sprintf("<hr/>\n</figure>\n");
            }
        }
    }

    public function __tostring()
    {
        return $this->buf;
    }
}

?>


