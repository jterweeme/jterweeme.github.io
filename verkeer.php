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

#scene {
    position: relative;
}

#scene img.car1 {
    position: absolute;
    top: 190px;
    left: 390px;
}

figure.verkeer svg#ampel {
    position: absolute;
    top: 50px;
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
<h2>Jasper ter Weeme</h2>
<time>15 Juni 2018</time>
<figure id="scene" class="verkeer">
<img class="weg" src="weg.svg"/>
<svg id="ampel" width="210mm" height="297mm">
    <rect style="stroke:#ff0000;" width="15" height="115" x="77" y="161"/>
    <rect style="stroke:#ff0000;" width="75" height="141" x="50" y="40"/>
    <circle style="fill:gray" cx="85" cy="70" r="18"/>
    <circle style="fill:gray" cx="85" cy="110" r="18"/>
    <circle style="fill:gray" cx="85" cy="150" r="18"/>
</svg>
<img id="car1" class="car1" src="car1.svg"/>
</figure>
<div style="column-count: 2;">
<p>
In deze workshop gaan we een simpel spelletje maken met JavaScript. De HTML code,
artwork en geluid gaan we niet zelf maken, deze zijn al geladen in deze webpagina.
Het voordeel van een spelletje in de webbrowser is dat er geen extra software nodig
is naast een webbrowser. Voor we met het ontwikkelen van het spelletje beginnen, gaan
we eerst de JavaScript interpreter van de webbrowser onder de loep nemen. Mensen
die hier al bekend mee zijn, kunnen dit onderdeel overslaan.
</p>
<p>
Oorspronkelijk werden webpagina's opgemaakt met enkel HTML. HTML geeft slechts
statische pagina's. Om pagina's dynamisch en interactief te maken, is JavaScript
nodig. Met F12 kunnen in Firefox, Chrome en Edge de developer tools worden
geopend. In al deze browsers zijn de developer tools opgedeeld in meerdere
tabbladen. Open hier het "Console" tabblad. Meestal is dit de tweede. In deze
console kan JavaScript ingevoerd worden. We beginnen heel simpel:
</p>
<code>
1 + 2;
</code>
<p>
Als dit ingevuld wordt zal 3 als antwoord terugkomen.
</p>
<code>
a = 1 + 2;
b = 9;
c = b - a;
</code>
<p>
Ook een belangrijk begrip in programmeren zijn arrays, oftewel lijsten:
</p>
<code>
l = [1,3,10,61,69];
</code>
<p>
Dit geeft een lijst met 5 items. Met "length" kan dit worden aangetoond:
</p>
<code>
l.length;
</code>
<p>
Ook kunnen items worden toegevoegd:
</p>
<code>
l.push(533);
l.push(601);
l.length;
</code>
<p>
"Loops" zijn onmisbaar in elk programma:
</p>
<code>
som = 0;
for (var i = 0; i &lt; l.length; i++)
    som += l[i];
som;
</code>
<p>
Deze code zal de som van alle items in de lijst weergeven, er zal 1278 als
antwoord uitkomen.
</p>
<p>
Sommige code zal meerdere malen gebruikt worden of voor het overzicht gegroepeerd
worden. Hiervoor gebruiken we functies. Hierbij een functie die aangeeft of
een getal priem is:
</p>
<code>
function isPrime(n) {
    if (n &lt;= 1) return false;
    for (var i = 2; i &lt; n; i++)
        if (n % i == 0) return false;
    return true;
}

isPrime(1);
isPrime(7);
</code>
<p>
Nu gaan we een functie maken die door een array heen loopt en aangeeft
welke nummers priem zijn:
</p>
<code>
function arePrimes(l) {
    for (var i = 0; i &lt; l.length; i++)
        if (isPrime(l[i])) console.log(l[i]);
}
</code>
<p>
Er zijn nog veel meer constructies uit te leggen, maar dat is niet waar
dit artikel voor geschreven is. We gaan beginnen met het spel zelf.
</p>

<p>
Om gebruik te maken van de content in de webpagina en deze interactief
te maken voor gebruik in ons spel maken we gebruik van het zogenaamde
Document Object Model (DOM). We hebben eerst verwijzingen nodig naar objecten
in onze scene.
</p>
<code>
scene = document.getElementById("scene");
ampel = document.getElementById("ampel");
lights = ampel.getElementsByTagName("circle");
car1 = document.getElementById("car1");
</code>
<p>
Nu hebben we verwijzingen naar enkele objecten. Om aan te tonen wat we
hiermee kunnen, gaan we het bovenste licht van het stoplicht rood maken:
</p>
<code>
lights[0].style = "fill:red";
</code>
<p>
Op dezelfde manier kunnen we het onderste licht groen maken:
</p>
<code>
lights[2].style = "fill:green";
</code>
<p>
Nu branden er twee lichten op hetzelfde moment. Dit gebeurd niet in
het echt in Nederland. Dus zullen we ook het bovenste licht weer op
grijs moeten zetten:
</p>
<code>
lights[0].style = "fill:gray";
</code>
<p>
Het is uiteraard niet handig om dit alles met de hand te doen, daarom
schrijven we voor elk licht een functie:
</p>
<code>
function red() {
    lights[0].style = "fill:red";
    lights[1].style = "fill:gray";
    lights[2].style = "fill:gray";
}

function orange() {
    lights[0].style = "fill:gray";
    lights[1].style = "fill:orange";
    lights[2].style = "fill:gray";
}

function green() {
    lights[0].style = "fill:gray";
    lights[1].style = "fill:gray";
    lights[2].style = "fill:green";
}
</code>
<p>
Probeer ze maar uit:
</p>
<code>
orange();
red();
green();
red();
</code>
<p>
Nu maken we een functie die ze in juiste volgorde schakelt, eerst
definieren we een variabele die bijhoudt wat de huidige stand van
het stoplicht is:
</p>
<code>
light = 0;

function toggleLight() {
    switch (light) {
    case 0:
        green();
        light = 2;
        break;
    case 1:
        red();
        light = 0;
        break;
    case 2:
        orange();
        light = 1;
        break;
    }
}
</code>
<p>
Tot nu toe hebben we enkel interactie gehad met de scene door
code uit te voeren in de console. Een speler van het spel zal dit
niet doen. Het is uiteraard ook mogelijk om acties te verbinden
met het klikken op objecten. We zullen het stoplicht instellen dat
deze zal schakelen wanneer erop geklikt wordt, oftewel het onclick
event zal de toggleLight() functie aanroepen:
</p>
<code>
ampel.onclick = function() { toggleLight(); }
</code>
<p>
Een echt stoplicht gebruikt een timer om na het verstrijken van een
bepaalde tijd vanzelf te schakelen. Dit zullen wij ook implementeren:
</p>
<code>
ampelTimer = setInterval(toggleLight, 2000);
</code>
<p>
ampelTimer verwijst nu naar een interval timer object. We geven toggleLight en
2000 mee, wat betekent dat onze toggleLight functie elke 2000 milliseconden
wordt aangeroepen.
</p>
<p>
Nu gaan we over tot de auto's.
</p>
<code>
car1.onclick = function() { car1.style = "left: 100px;"; }
</code>
</div>
</main>
</body>
</html>


