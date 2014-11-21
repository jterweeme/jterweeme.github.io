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
<img src="stand.jpg" alt="stand"/>
<img src="rainbow.jpg" alt="Rainbow of exciting flavors!"/>
<footer><small>Copyright © 2001-2002 Penny Juice All Rights Reserved.</small></footer>
</main>
</body>
</html>




