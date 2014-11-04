<?php
class MainMenu
{
    public function __tostring()
    {
        $path = stat('mainmenu.xml') ? "mainmenu.xml" : "../mainmenu.xml";
        return file_get_contents($path);
    }
}
?>


