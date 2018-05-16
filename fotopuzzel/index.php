<!DOCTYPE html>
<html>
<head>
<title>Fotopuzzel</title>
<link rel="stylesheet" type="text/css" href="../common.css"/>
<link rel="stylesheet" type="text/css" href="index.css"/>
<script type="text/javascript" src="index.js"></script>
</head>
<body onload="REDIPS.drag.init()">
<?php
require_once('../mainmenu.php');
$mm = new MainMenu();
echo $mm;
?>
<div id="drag">
<table>
<tr>
    <td id="0"><div class="drag"><img src="win_2_0.jpg" alt="tux" /></div></td>
    <td id="1"><div class="drag"><img src="win_1_0.jpg" alt="tux" /></div></td>
    <td id="2"><div class="drag"><img src="win_0_2.jpg" alt="tux" /></div></td>
</tr>
<tr>
    <td id="3"><div class="drag"><img src="win_0_1.jpg" alt="tux" /></div></td>
    <td id="4"><div class="drag"><img src="win_0_0.jpg" alt="tux" /></div></td>
    <td id="5"><div class="drag"><img src="win_1_1.jpg" alt="tux" /></div></td>
</tr>
<tr>
    <td id="6"><div class="drag"><img src="win_2_2.jpg" alt="tux" /></div></td>
    <td id="7"><div class="drag"><img src="win_2_1.jpg" alt="tux" /></div></td>
    <td id="8"></td>
</tr>
</table>
</div>

<pre style="color: white;">
[Oplossing 1]
8 7 4 1 2 5 8 7 4 1 2 3 6 5 8 9 6 3
2 5 6

[Oplossing 2]
6 5 8 7 4 5 6 9 8 7 4 1 2 5 8 7 4 1
2 5 6
</pre>

</body>
</html>



