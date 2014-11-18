<!DOCTYPE html>
<?php
echo file_get_contents("../achtung.html");
?>
<html class="accept">
<head>
<title>Accept Jesus</title>
<link rel="stylesheet" type="text/css" href="../common.css"/>
<link rel="stylesheet" type="text/css" href="index.css"/>
</head>
<body>
<?php
require_once("../mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>
<aside>
<img src="warning.gif" alt="warning"/>
<span>
    You are possibly not secure in your own personal faith. Trust
    in God solely through His Son Jesus for the forgiveness of all
    your sins and the free gift of eternal life. Click here.
</span>
</aside>
<figure>
    <img class="titel" src="title.png" alt="accept jesus, forever forgiven"/>
    <figcaption>
        Behold, now is the accepted time; behold, now is the day of salvation. - 2 Cor. 6:2
    </figcaption>
</figure>
<blockquote>
<q>
For the wages of sin is death;
but the gift of God is eternal life
through Jesus Christ our Lord.
</q>
<cite>-Romans 6:23</cite>
</blockquote>
<img class="cat" src="blackcat.gif" alt="cat"/>
<img class="door" src="door.gif" alt="door" width="100"/>
</body>
</html>


