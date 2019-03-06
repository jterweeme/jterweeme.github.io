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
$mm->setDepth(0);
echo $mm;
?>
<main>
<h1>Welcome</h1>
<ul>
<li><a href="ringtone.html">Ringtoon speler</a></li>
<li><a href="spelcomputer.pdf">Spelcomputer op breadboard</a></li>
<li><a href="euler/index.html">Project Euler</a></li>
<li><a href="simpelweb.html">Meest simpele webserver</a></li>
<li><a href="md5sum.html">Diagram MD5Sum</a></li>
<li><a href="kompakt.html">Diagram Kompakt</a></li>
<li><a href="rm.html">Diagram RM</a></li>
<li><a href="kompakt011.zip">Download Kompakt</a></li>
</ul>
</main>
</body>
</html>


