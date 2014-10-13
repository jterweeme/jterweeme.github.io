<!DOCTYPE html>
<html>
<head>
<title>Welcome</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
</head>
<body>
<?php
echo file_get_contents("mainmenu.xml");
?>
<h1>Welcome</h1>
<ul>
<li><a href="md5sum.xml">Diagram MD5Sum</a></li>
<li><a href="kompakt.xml">Diagram Kompakt</a></li>
</ul>
</body>
</html>


