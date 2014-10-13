<?php
if (!empty($_POST))
{
    $xml = new DOMDocument();
    $xml->load("hiscore.xml");
    $xml->formatOutput = true;
    $foo = $xml->getElementsByTagName("root");
    foreach ($foo as $bar)
        $root = $bar;
    $entry = $xml->createElement("entry");

    $naam = $xml->createElement("naam");
    $naam->appendChild($xml->createTextNode($_POST['naam']));

    $score = $xml->createElement("score");
    $score->appendChild($xml->createTextNode($_POST['grandScore']));

    $entry->appendChild($naam);
    $entry->appendChild($score);

    $root->appendChild($entry);

    $xml->save("hiscore.xml");
}
echo "<html>
<head>
    <title>High Score</title>
    <link rel=\"stylesheet\" type=\"text/css\" href=\"hiscore.css\"/>
</head>
<body>
<table><caption>Hall of Fame</caption>";
$xml = simplexml_load_file("hiscore.xml");
$entries = $xml->xpath('/root/entry');
function sort_entry($t1, $t2)
{
    return ((int)$t1->score < (int)$t2->score);
}
usort($entries, 'sort_entry');
foreach ($entries as $foo)
    printf("<tr>\n<td>%s</td>\n<td>%s</td>\n</tr>\n", $foo->naam, $foo->score);
echo "</table>
<nav>
<a href=\"index.php\">New Game</a>
</nav>
</body>
</html>";
?>

