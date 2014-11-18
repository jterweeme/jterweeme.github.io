<?php
class MainMenu
{
    private $depth;
    public function __construct() { $this->depth = 0; }
    public function setDepth($depth) { $this->depth = $depth; }

    public function __tostring()
    {
        $path = file_exists('mainmenu.xml') ? "mainmenu.xml" : "../mainmenu.xml";
        $xml = simplexml_load_file($path);
        $ret = "<nav class=\"mainmenu\">\n";

        foreach ($xml->a as $link)
        {
            if (file_exists("achtung.html") ||
                $link->attributes() == "https://github.com/jterweeme")
            {
                $ret .= sprintf("<a href=\"%s\">%s</a>\n", $link->attributes(), $link);
            }
            else
            {
                $ret .= sprintf("<a href=\"../%s\">%s</a>\n", $link->attributes(), $link);
            }
        }

        $ret .= "</nav>";
        return (string)$ret;
    }
}
?>


