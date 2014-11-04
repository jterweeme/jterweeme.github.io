<!DOCTYPE html>
<?php
echo file_get_contents("achtung.html");
?>
<html>
<head>
<title>Welcome</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
</head>
<body>
<?php
require_once('mainmenu.php');
$mm = new MainMenu();
echo $mm;
?>
<h1>Welcome</h1>
<ul>
<li><a href="md5sum.html">Diagram MD5Sum</a></li>
<li><a href="kompakt.html">Diagram Kompakt</a></li>
<li><a href="rm.html">Diagram RM</a></li>
<li><a href="kompakt011.zip">Download Kompakt</a></li>
</ul>
</body>
</html>


