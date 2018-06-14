<!DOCTYPE html>

<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>Verkeerspel</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
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

figure.verkeer {
    position: absolute;
}

figure.verkeer img.car1 {
    position: absolute;
    top: 190px;
    left: 390px;
}
</style>
</head>
<body>
<?php
require_once("mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>

<main style="width: 900px;">
<h1>Verkeerspel</h1>
<figure class="verkeer">
<img class="weg" src="weg.svg"/>
<img class="car1" src="car1.svg"/>
</figure>
</main>
</body>
</html>



