<!DOCTYPE html>
<?php
echo file_get_contents("../achtung.html");
?>
<html class="penny">
<head>
<link rel="stylesheet" type="text/css" href="../common.css"/>
<link rel="stylesheet" type="text/css" href="index.css"/>
<title>fruitTreesFinal</title>
</head>
<body>
<?php
require_once("../mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>
<main>
<img src="stand.jpg" alt="stand" usemap="#menu"/>
<map name="menu">
<area shape="rect" coords="211,323,347,345" href="mailto:pennyjuice@hotmail.com"/>
<area shape="rect" coords="223,302,332,324" href="gotjuice.htm"/>
<area shape="rect" coords="179,283,383,303" href="whereispj.htm"/>
<area shape="rect" coords="180,262,381,283" href="whatispj.htm"/>
<area shape="rect" coords="188,242,369,263" href="whois.html"/>
</map>
<img src="rainbow.jpg" alt="Rainbow of exciting flavors!"/>
<footer><small>Copyright (c) 2001-2002 Penny Juice All Rights Reserved.</small></footer>
</main>
</body>
</html>




