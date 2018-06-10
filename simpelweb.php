<!DOCTYPE html>

<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>Meest simpele webserver</title>
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
<h1>HTTP Server</h1>
<code>
import http.server, socketserver

handler = http.server.SimpleHTTPRequestHandler
httpd = socketserver.TCPServer(("", 8080), handler)
httpd.serve_forever()
</code>


</main>
</body>
</html>



