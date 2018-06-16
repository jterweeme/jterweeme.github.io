<!DOCTYPE html>

<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>Project Euler</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<style>
code, kbd, samp {
    white-space: pre;
}

var {
    font-family: monospace;
    font-style: normal;
}

samp {
    color: gray;
}

code em {
    font-style: normal;
    color: red;
}
</style>
</head>
<body>
<?php
require_once("mainmenu.php");
$mm = new MainMenu();
echo $mm;
echo file_get_contents("euler.xml");
?>
</body>
</html>


