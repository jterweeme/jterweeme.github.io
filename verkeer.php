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
    width: 1100px;
    position: relative;
    overflow: hidden;
}

#scene .car {
    position: absolute;
    top: 190px;
    cursor: pointer;
}

#scene #car1 {
    right: 150px;
}

#scene #car2 {
    right: -90px;
}

#scene svg#ampel {
    position: absolute;
    top: -30px;
    left: 520px;
}
</style>
<script src="verkeer.js"></script>
</head>
<body onload="start()">
<?php
require_once("mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>

<audio id="horn">
    <source src="horn.ogg" type="audio/ogg"/>
</audio>
<audio>
<source src="car_accel.ogg" type="audio/ogg"/>
</audio>

<main style="width: 900px;">
<h1>Verkeerspel</h1>
<h2>Jasper ter Weeme</h2>
<time>15 Juni 2018</time>
<figure id="scene" class="verkeer">
<img class="weg" src="weg.svg"/>
<svg id="ampel" height="300px">
    <rect style="stroke:#ff0000;" width="15" height="115" x="77" y="161"/>
    <rect style="stroke:#ff0000;" width="75" height="141" x="50" y="40"/>
    <circle style="fill:gray" cx="85" cy="70" r="18"/>
    <circle style="fill:gray" cx="85" cy="110" r="18"/>
    <circle style="fill:gray" cx="85" cy="150" r="18"/>
</svg>
<svg id="car1" class="car">
    <path
       style="fill:blue;stroke:#ff0000;stroke-width:0.26458332px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1"
       d="M 37.299689,122.38377 C 37.352614,75.496111 104.3604,79.622588 104.3604,79.622588 l 14.32397,-22.070616 c 37.77143,-13.16009 108.11192,-8.915944 133.73917,49.987758 l 9.5006,15.34711 -19.58585,1.46163 c 0,0 -6.86724,-21.3893 -26.60167,-21.48596 -20.38464,-0.0999 -22.41436,26.21486 -22.41436,26.21486 L 90.036421,131.3643 c 0,0 -4.474927,-26.83407 -22.070615,-28.8672 -17.389404,-2.0093 -25.636158,20.55733 -30.666117,19.88667 z"/>
    <path
       id="path1053"
       style="opacity:1;fill:#ff0000;fill-opacity:1;stroke:#ff0000;stroke-width:0.06174354;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 77.771507,127.06085 a 13.022461,13.022461 0 0 0 -13.022461,-13.02246 13.022461,13.022461 0 0 0 -13.022461,13.02246 13.022461,13.022461 0 0 0 13.022461,13.02246 13.022461,13.022461 0 0 0 13.022461,-13.02246 z m 6.639271,0.35705 A 19.368229,19.368229 0 0 1 65.042549,146.78613 19.368229,19.368229 0 0 1 45.67432,127.4179 19.368229,19.368229 0 0 1 65.042549,108.04967 19.368229,19.368229 0 0 1 84.410778,127.4179 Z" />
    <path
       id="path1053-3"
       style="opacity:1;fill:#ff0000;fill-opacity:1;stroke:#ff0000;stroke-width:0.06174354;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 230.8035,123.44666 a 13.022461,13.022461 0 0 0 -13.02246,-13.02246 13.022461,13.022461 0 0 0 -13.02246,13.02246 13.022461,13.022461 0 0 0 13.02246,13.02246 13.022461,13.022461 0 0 0 13.02246,-13.02246 z m 6.63928,0.35705 a 19.368229,19.368229 0 0 1 -19.36823,19.36823 19.368229,19.368229 0 0 1 -19.36823,-19.36823 19.368229,19.368229 0 0 1 19.36823,-19.36823 19.368229,19.368229 0 0 1 19.36823,19.36823 z" />
    <path
       style="fill:gray;stroke:#ff0000;stroke-width:0.26499999;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 110.45394,82.988826 c -1.54545,-1.952485 4.6193,-13.158085 9.47515,-16.125199 11.23655,-6.865968 37.90783,-5.980774 39.22179,-4.718766 1.10211,1.058537 2.6981,18.432104 -0.18379,21.023441 -1.14393,1.0286 -46.36175,2.538549 -48.51315,-0.179476 z"
       id="path817"/>
    <path
       style="fill:gray;stroke:#ff0000;stroke-width:0.26499999;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 166.08805,83.392741 c -3.17714,-1.309459 -2.68688,-19.958505 -1.13688,-21.600749 2.46656,-2.613353 45.28124,1.984786 42.78809,18.706867 -0.75879,5.089376 -38.821,4.06035 -41.65121,2.893882 z"
       id="path819"/>
</svg>
<svg id="car2" class="car">
    <path
       style="fill:red;stroke:#ff0000;stroke-width:0.26458332px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1"
       d="M 37.299689,122.38377 C 37.352614,75.496111 104.3604,79.622588 104.3604,79.622588 l 14.32397,-22.070616 c 37.77143,-13.16009 108.11192,-8.915944 133.73917,49.987758 l 9.5006,15.34711 -19.58585,1.46163 c 0,0 -6.86724,-21.3893 -26.60167,-21.48596 -20.38464,-0.0999 -22.41436,26.21486 -22.41436,26.21486 L 90.036421,131.3643 c 0,0 -4.474927,-26.83407 -22.070615,-28.8672 -17.389404,-2.0093 -25.636158,20.55733 -30.666117,19.88667 z"/>
    <path
       id="path1053"
       style="opacity:1;fill:#111144;fill-opacity:1;stroke:#ff0000;stroke-width:0.06174354;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 77.771507,127.06085 a 13.022461,13.022461 0 0 0 -13.022461,-13.02246 13.022461,13.022461 0 0 0 -13.022461,13.02246 13.022461,13.022461 0 0 0 13.022461,13.02246 13.022461,13.022461 0 0 0 13.022461,-13.02246 z m 6.639271,0.35705 A 19.368229,19.368229 0 0 1 65.042549,146.78613 19.368229,19.368229 0 0 1 45.67432,127.4179 19.368229,19.368229 0 0 1 65.042549,108.04967 19.368229,19.368229 0 0 1 84.410778,127.4179 Z" />
    <path
       id="path1053-3"
       style="opacity:1;fill:#111144;fill-opacity:1;stroke:#ff0000;stroke-width:0.06174354;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 230.8035,123.44666 a 13.022461,13.022461 0 0 0 -13.02246,-13.02246 13.022461,13.022461 0 0 0 -13.02246,13.02246 13.022461,13.022461 0 0 0 13.02246,13.02246 13.022461,13.022461 0 0 0 13.02246,-13.02246 z m 6.63928,0.35705 a 19.368229,19.368229 0 0 1 -19.36823,19.36823 19.368229,19.368229 0 0 1 -19.36823,-19.36823 19.368229,19.368229 0 0 1 19.36823,-19.36823 19.368229,19.368229 0 0 1 19.36823,19.36823 z" />
    <path
       style="fill:gray;stroke:#ff0000;stroke-width:0.26499999;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 110.45394,82.988826 c -1.54545,-1.952485 4.6193,-13.158085 9.47515,-16.125199 11.23655,-6.865968 37.90783,-5.980774 39.22179,-4.718766 1.10211,1.058537 2.6981,18.432104 -0.18379,21.023441 -1.14393,1.0286 -46.36175,2.538549 -48.51315,-0.179476 z"
       id="path817"/>
    <path
       style="fill:gray;stroke:#ff0000;stroke-width:0.26499999;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
       d="m 166.08805,83.392741 c -3.17714,-1.309459 -2.68688,-19.958505 -1.13688,-21.600749 2.46656,-2.613353 45.28124,1.984786 42.78809,18.706867 -0.75879,5.089376 -38.821,4.06035 -41.65121,2.893882 z"
       id="path819"/>
</svg>
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
Een for-loop bestaat uit drie delen, gescheiden door punt-komma's. Het eerste
deel wordt voorafgaand aan de loop uitgevoerd. Hier worden meestal variabelen
gedefinieerd. In het tweede deel wordt een vergelijking uitgevoerd, alleen
wanneer deze waar is, wordt de code onder de for-loop uitgevoerd. Het derde
deel van de for-loop wordt elke keer uitgevoerd nadat de computer een keer
door de code van de loop is heengelopen. Meestal worden er in dit deel
variabelen opgehoogd of juist verlaagd, die gebruikt worden in de vergelijking
uit het tweede deel. Vaak volgen programmeurs dus een bepaald stramien bij het
invullen van een for-loop, maar dit is niet verplicht.
</p>
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
In deze functie wordt ook een "if" statement gebruikt. Bij dit statement wordt
een vergelijking berekend en wanneer deze waar is wordt de code achter
het if-statement uitgevoerd.
</p>
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
<p>
Als je nu op de voorste auto klikt, wordt deze verplaatst naar links.
</p>
<code>
function myMove() {
  var elem = car1;  
  var pos = 150;
  var id = setInterval(frame, 5);
  function frame() {
    if (pos == 550) {
      clearInterval(id);
    } else {
      pos++; 
      elem.style.right = pos + 'px'; 
    }
  }
}
</code>
</div>
</main>
</body>
</html>



