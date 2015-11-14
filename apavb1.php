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
printf(file_get_contents("mainmenu.xml"));
$proc = new XSLTProcessor();
$xsl = new DOMDocument();
$xsl->load('apa.xsl');
$xml = new DOMDocument();
$xml->load('apavb1.xml');
$proc->importStylesheet($xsl);
echo $proc->transformToXml($xml);
?>

</body>
</html>


