<!DOCTYPE html>

<html>
<head>
<title>Project Euler</title>
<link rel="stylesheet" type="text/css" href="../common.css"/>
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

section {
    border-bottom: 1px solid black;
}

#clipboard {
    /*visibility: hidden;*/
}
</style>
<script>
function mycopy2(x) {
    var section = document.getElementById(x);
    var copyText = section.getElementsByTagName("code")[0];
    var clipboard = document.getElementById("clipboard");
    clipboard.innerHTML = copyText.innerHTML;
    clipboard.select();
    document.execCommand("copy");
}
</script>
</head>
<body>
<?php
require_once("../mainmenu.php");
$mm = new MainMenu();
echo $mm;
echo file_get_contents("euler.xml");
?>
<textarea id="clipboard"></textarea>
</body>
</html>



