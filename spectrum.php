<!DOCTYPE html>
  
<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>Spectrum FPGA</title>
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
</style>
</head>
<body>
<?php
require_once("mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>
<main style="width: 900px;">
<h1>ZX Spectrum</h1>
<p>
In deze tutorial gaan we een ZX Spectrum implementeren op een FPGA
bord. We zullen hiervoor het DE2 115 bord als voorbeeld gebruiken.

De ZX Spectrum is een homecomputer uit 1982. Ontworpen door Sir
Clive Sinclair, en bedoeld om de grote massa te introduceren in de wereld
van computers en programmeren in een tijdperk dat computers nog
voorbehouden waren tot een kleine groep mensen. Dit werd mogelijk gemaakt
voor een prijs van ongeveer 150 pond.
</p>
</main>
