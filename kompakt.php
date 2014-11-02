<!DOCTYPE html>
<?php
echo file_get_contents("achtung.html");
?>
<html>
<head>
<title>Onzin</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
<link rel="stylesheet" type="text/css" href="omelet.css"/>
</head>
<body>

<?php
require_once("omelet.php");
printf(file_get_contents("mainmenu.xml"));
printf("<figure class=\"diagram\">\n%s\n</figure>", new Omelet("kompakt.uxf"));
?>
</body>
</html>


