<?php
class MainMenu
{
    public function __tostring()
    {
        $path = stat('mainmenu.xml') ? "mainmenu.xml" : "../mainmenu.xml";
        $xml = simplexml_load_file($path);
        $ret = "<nav class=\"mainmenu\">\n";

        foreach ($xml->a as $link)
        {
            if (stat("achtung.html"))
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


