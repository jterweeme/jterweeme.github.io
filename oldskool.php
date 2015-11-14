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
<h1>OldSkool</h1>
<h2>Windows 98</h2>
<h3>To much memory</h3>
<p>
Windows 98 will by default not run on systems with more than 1GB of
memory. To remedy this add the following to SYSTEM.INI
</p>
<code>
[386Enh]
MaxPhysPage=9000
</code>
<h3>Real Mode Network</h3>
</main>
</body>
</html>


