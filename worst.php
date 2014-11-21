<!DOCTYPE html>

<?php
echo file_get_contents("achtung.html");
?>
<html class="worst">
<head>
<title>Onzin</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
</head>
<body>

<?php
require_once("mainmenu.php");
$mm = new MainMenu();
echo $mm;

?>
<main>
<h1>World's Worst Website</h1>
<a href="yvettes/index.html">Yvettes Bridal Formal</a>
<a href="accept/index.html">Accept Jesus, Forever Forgiven</a>
</main>
</body>
</html>

