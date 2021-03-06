<!DOCTYPE html>

<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>Ringtone player</title>
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
<h1>Ringtone speler</h1>
<p>
In deze tutorial gaan we een ringtoonspeler maken met de taal Python op
de Raspberry Pi.
</p>
<p>
We zullen een Raspberry Pi 3 gebruiken met een buzzer aangesloten op
de GPIO header. De ringtoonspeler zal het Nokia RTTTL formaat gebruiken.
</p>
<p>
Voor we de buzzer gaan aansluiten gaan we eerst de GPIO poorten verkennen. GPIO
staat voor General Purpose Input Output. Hiermee wordt bedoeld dat deze aansluitingen
voor van allerlei verschillende doeleinden gebruikt kan worden, in tegenstelling tot
een aansluiting die specifiek bedoeld is voor beelscherm of toetsenbord.
Om te kijken of de GPIO naar behoren werkt zullen we eerst beginnen met
een led. De <i>anode</i>, oftewel de plus-pool, sluiten we aan op pin GPIO18. Dit
is ook de pin die we later zullen gebruiken voor de buzzer. De <i>cathode</i>, oftewel
de min-pool kan op iedere ground-pin worden aangesloten.
</p>
<figure>
<img src="pi3_gpio.png" alt="pinmap" width="400px"/>
</figure>

<p>
Voer op de commandline het volgende commando uit:
</p>

<kbd>
gpio readall
</kbd>

<p>
Dit geeft de status van alle GPIO poorten. De output ziet er ongeveer zo uit:
</p>

<samp>
 +-----+-----+---------+------+---+-Model B1-+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 0 | ALT0 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+-Model B1-+---+------+---------+-----+-----+
</samp>

<p>
De verschillende nummeringen zullen waarschijnlijk voor verwarring zorgen. Wij zijn
geinterresseerd in pin BCM 18. BCM staat voor Broadcom, de fabrikant van de
processor. Zoals u ziet staat de mode op "IN". Om de led aan en uit te zetten
moet deze eerst op output worden gezet.
</p>

<kbd>
gpio -g mode 18 out
</kbd>

<p>
Wanneer u opnieuw <var>readall</var> zal uitvoeren, zal "OUT" als mode worden
gegeven. Nu kunnen we de led aan en uit zetten:
</p>

<kbd>
gpio -g write 18 1
gpio -g toggle 18
</kbd>

<p>
Nu de led werkt, kan de buzzer worden aangesloten. We sluiten de buzzer aan op
pin GPIO18, omdat deze pin hardware PWM ondersteund. PWM staat voor puls-breedte-modulatie.
Deze vorm van modulatie wordt voor vele toepassingen gebruikt, zoals motordrivers,
servo-motoren, het dimmen van leds en buzzers.
</p>

<p>
De PWM kan ook op de command-line worden getest. We zetten eerst pin GPIO18 in
de PWM-toon modus:
</p>

<kbd>
gpio -g mode 18 pwmTone
</kbd>

<p>
Het <var>readall</var> commando nu ALT5 als modus opgeven bij pin BCM 18. Vervolgens kan
een frequentie worden geschreven naar pin BCM 18:
</p>

<kbd>
gpio -g pwmTone 18 440
gpio -g pwmTone 18 800
gpio -g pwmTone 18 1000
gpio -g pwmTone 18 0
</kbd>

<p>
Als alles goed is hoorde u tonen op 440, 800 en 1000Hz respectievelijk. We kunnen er nu
vanuit gaan dat de hardware goed geconfigureerd is en over naar het scripten in Python.
</p>

<h2>Python</h2>
<p>
Voordat we kunnen beginnen moet nog wel een <i>daemon</i> worden gestart als
root om Python ook gebruik te kunnen laten maken van de GPIO functies, met name PWM:
</p>

<kbd>
sudo pigpiod
</kbd>

<p>
We starten nu Python op:
</p>

<kbd>
python3
</kbd>

<samp>
Python 3.5.3 (default, Jan 19 2017, 14:11:04) 
[GCC 6.3.0 20170124] on linux
Type "help", "copyright", "credits" or "license" for more information.
&gt;&gt;&gt; 
</samp>

<p>
We zitten nu in de Python console in de terminal, maar u staat vrij om een IDE te
gebruiken zoals IDLE. Allereerst importeren we functies uit drie modules:
<var>pigpio</var>, <var>pprint</var>
en <var>time</var>. Pigpio importeren we in zijn geheel voor de volledigheid.
Pigpio is nodig om de PWM aan te sturean en de functie <var>sleep</var> uit de
<var>time</var> module hebben
we nodig om tonen een bepaalde duur te geven. <var>pprint</var> is niet strikt
noodzakelijk, maar handig om gegevens netjes uit te printen naar de terminal.
</p>

<code>
import pigpio
from pprint import pprint
from time import sleep
</code>

<p>
De functie <var>pprint</var> kan nuttig gebruikt worden om bijvoorbeeld de
output van <var>locals()</var> op te maken. <var>locals()</var> wordt
gebruikt om de lokale symbolen weer te geven, zoals geladen modules,
gedefinieerde variabelen, functies en klassen:
</p>

<code>
pprint(locals())
</code>

<samp>
{'__builtins__': &lt;module 'builtins' (built-in)&gt;,
 '__doc__': None,
 '__loader__': &lt;class '_frozen_importlib.BuiltinImporter'&gt;,
 '__name__': '__main__',
 '__package__': None,
 '__spec__': None,
 'pigpio': &lt;module 'pigpio' from '/usr/lib/python3/dist-packages/pigpio.py'&gt;,
 'pprint': &lt;function pprint at 0xb67fb150&gt;,
 'sleep': &lt;built-in function sleep&gt;}
</samp>

<p>
Nu zien we dus dat de module <var>pigpio</var> en de functies <var>pprint</var> en
<var>sleep</var> daadwerkelijk geladen zijn.
</p>

<p>
Nu maken we een klasse aan om tonen te genereren met de buzzer. Kopieer de
volgende code naar de Python console.
</p>

<code>
class PWM:
    def __init__(self):
        self.pig = pigpio.pi()
    def play(self, f = 440, duration = 1.0):
        self.pig.hardware_PWM(18, f, 500000)
        sleep(duration)
        self.pig.hardware_PWM(18, 0, 0)
</code>

<p>
Een nieuwe invocatie van <var>locals()</var> zal aangeven dat er nu een klasse
<var>PWM</var> gedefinieerd is. Een klasse is een blauwdruk waarmee een of meerdere
objecten kunnen worden aangemaakt. Onze <var>PWM</var> klasse bestaat uit twee
functies: <var>__init__</var> en <var>play</var>. De <var>__init__</var> functie
is de zogenaamde <i>constructor</i>
die elke klasse heeft om objecten aan te maken. Een object is dus een instantie van
een klasse. We maken een object <var>buzzer</var> aan uit <var>PWM</var>:
</p>

<code>
buzzer = PWM()
</code>

<p>
Als we nogeens <var>pprint(locals())</var> uitvoeren, zien we dat
<var>buzzer</var> een object is uit <var>PWM</var> en wordt de plek in
het geheugen aangegeven. Dit laatste is weinig interressant voor ons.
</p>

<samp>
 'buzzer': &lt;__main__.PWM object at 0xb67cf810&gt;,
</samp>

<p>
Met het <var>buzzer</var> object kunnen we nu tonen afspelen:
</p>

<code>
buzzer.play();
buzzer.play(800);
buzzer.play(1500, 1);
</code>

<p>
Wat nu rest is het schrijven van een klasse die de RTTTL ringtones ontleedt en
afspeelt:
</p>

<code>
class RtttlPlayer:
    def __init__(self, tonegen, tonelen = 280):
        self.tonegen = tonegen
        self.tonelen = tonelen
    def izdigit(self, c):
        if c &gt;= '0' and c &lt;= '9':
            return True
        return False
    noten = [0,262,277,294,311,330,349,370,392,415,440,466,494,523,554, \
        587,622,659,698,740,831,880,932,988,1047,1109,1175,1245, \
        1319,1397,1480,1568,1661,1760,1865,1976,2093,2217,2349,2489, \
        2637,2794,2960,3136,3322,3520,3729,3951,2*2093,2*2217, 2*2349, \
        2*2489,2*2637,2*2794,2*2960,2*3136,2*3322,2*3520,2*3729,2*3951,0]
    def notemap(self, c):
        if c == 'c':
            return 1
        if c == 'd':
            return 3
        if c == 'e':
            return 5
        if c == 'f':
            return 6
        if c == 'g':
            return 8
        if c == 'a':
            return 10
        if c == 'b':
            return 12
        return 0
    def play(self, tune):
        default_dur = 4
        default_oct = 6
        bpm = 160
        length = len(tune)
        i = 0
        while True:
            c = tune[i]
            i += 1
            if c == ':':
                break;
        if tune[i] == 'd':
            i += 2  #skip d=
            num = 0
            while (self.izdigit(tune[i])):
                num = num * 10 + (int(tune[i]) - int('0'))
                i += 1
            if (num &gt; 0):
                default_dur = num
            i += 1 #skip comma
        if tune[i] == 'o':
            i += 2 #skip o=
            num = 0
            while (self.izdigit(tune[i])):
                num = num * 10 + (int(tune[i]) - int('0'))
                i += 1
            if num &gt;= 3 and num &lt;= 7:
                default_oct = num
            i += 1
        if tune[i] == 'b':
            i += 2 #skip b=
            num = 0
            while (self.izdigit(tune[i])):
                num = num * 10 + (int(tune[i]) - int('0'))
                i += 1
            bpm = num
            i += 1  #skip colon
        wholenote = self.tonelen/bpm
        while i &lt; length:
            num = 0
            while (self.izdigit(tune[i])):
                num = num * 10 + (int(tune[i]) - int('0'))
                i += 1
            if num &gt; 0:
                duration = wholenote / num
            else:
                duration = wholenote / default_dur
            note = self.notemap(tune[i])
            i += 1
            if i &lt; length and tune[i] == '#':
                note += 1
                i += 1
            if i &lt; length and tune[i] == '.':
                duration += duration / 2
                i += 1
            if i &lt; length and self.izdigit(tune[i]):
                scale = int(tune[i]) - int('0')
                i += 1
            else:
                scale = default_oct
            if i &lt; length and tune[i] == ',':
                i += 1
            if note:
                freq = self.noten[(scale - 4) * 12 + note]
                self.tonegen.play(freq, duration)
            else:
                self.tonegen.play(0, duration)
</code>

<p>
We maken meteen een <var>player</var> object aan. Onze
<var>RtttlPlayer</var> klasse weet niet van de specifieke hardware die
daadwerkelijk het geluid maakt en zal in dit geval gebruik maken van ons
<var>buzzer</var> object. Later zullen we aantonen dat het ook mogelijk
is om een geheel andere klasse te gebruiken om geluid te maken met
dezelfde <var>RtttlPlayer</var> klasse. Zo kunnen bijvoorbeeld ook de
luidsprekers bij de PC gebruikt worden via het geluidssysteem van Windows
of Linux. Maar eerst geven we ons <var>buzzer</var> object mee aan de
constructor van de <var>RtttlPlayer</var> klasse:
</p>

<code>
p = RtttlPlayer(buzzer)
</code>

<p>
Nu willen we uiteraard geluid horen. Hierbij dus enkele ringtones:
</p>

<code>
godfather = ("Godfather:d=4,o=5,b=160:8g,8c6,8d#6,8d6,8c6,8d#6,8c6,8d6,c6,8g#,8a#,2g,8p,"
    "8g,8c6,8d#6,8d6,8c6,8d#6,8c6,8d6,c6,8g,8f#,2f,8p,8f,8g#,8b,2d6,8p,8f,8g#,"
    "8b,2c6,8p,8c,8d#,8a#,8g#,g,8a#,8g#,8g#,8g,8g,8b4,2c,")
goodbad = ("Good Bad:d=4,o=6,b=40:32b5,32e6,32b5,32e6,"
    "8b.5,16g.5,16a.5,4e5,32b5,32e6,32b5,32e6,8b.5,16g.5,16a.5,4d6,32b5,32e6,"
    "32b5,32e6,8b.5,16g.5,32f#.5,32e.5,4d5,32b5,32e6,32b5,32e6,8b.5,16a.5,4e5,")
wilhelmus = ("Wilhelmus:d=4,o=5,b=90:d,g,g,8a,8b,8c6,8a,b,8a,8b,c6,b,8a,8g,a,2g,p,d,g,"
    "g,8a,8b,8c6,8a,b,8a,8b,c6,b,8a,8g,a,2g,p,8b,8c6,2d6,e6,2d6,c6,b,8a,8b,c6,b,"
    "a,g,2a,p,d,8g,8f#,8g,8a,b,2a,g,f#,d,8e,8f#,g,g,f#,2g")
indy = ("Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,"
    "1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,"
    "8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6")
</code>

<p>
Definieer een of meerdere van deze ringtones en speel ze af met de
<var>RtttlPlayer.play</var> functie:
</p>

<code>
p.play(indy)
</code>

<p>
Als het goed is hoort u nu een ringtone. Gefeliciteerd! We hebben hierbij
het einde behaald van de Ringtonespeler op de RaspberryPi.
</p>

<h2>PyAudio</h2>

<p>
Om aan te tonen dat de <var>RtttlPlayer</var> klasse onafhankelijk werkt
van een specifieke implementatie van de toongenerator, zullen we met nog
een voorbeeld komen welk de <var>PyAudio</var> interface gebruikt om
geluid af te spelen met de geluidskaart.
</p>

<code>
import pyaudio
import numpy as np

class ToneGen:
    def __init__(self, volume = 1, fs = 48000):
        self.vol = volume
        self.fs = fs
        self.p = pyaudio.PyAudio()
    def enable(self):
        self.stream = self.p.open(format = pyaudio.paFloat32, channels = 1, rate = self.fs, output = True)
    def disable(self):
        self.stream.close()
        self.stream.stop_stream()
    def play(self, f = 440.0, duration = 1.0):
        self.enable()
        samples = (np.sin(2 * np.pi * np.arange(self.fs*duration*4) * f/self.fs)).astype(np.float32)
        self.stream.write(self.vol * samples)
        self.disable()
    def __del__(self):
        self.disable()
        self.p.terminate()
</code>


<p>
Net als met de buzzer maken we een object aan:
</p>

<code>
tg = ToneGen()
</code>

<p>
Optioneel kan een volume en samplerate worden opgegeven, maar dit laten we
op de standaardwaarden staan. Kopieer wederom de <var>RtttlPlayer</var>
klasse naar de Python omgeving en maak een object aan:
</p>

<code>
p = RtttlPlayer(tg, 350)
</code>

<p>
Dit keer geven we de waarde 350 mee aan de constructor. Dit zet de
standaard toonduur van 280 naar 350. Dit doen we omdat anders de snelheid
van de ringtones te hoog zal zijn. Dit komt omdat deze <var>ToneGen</var>
klasse minder vertraging veroorzaakt dan de eerdere <var>PWM</var> klasse.
Het opgeven van de waarde 350 compenseert dit. Op dezelfde manier als
eerder in de tutorial kunnen ringtones gedefinieerd worden en afgespeeld.
</p>

<p>
Tot slot nog enkele ringtones:
</p>

<code>
<em>James Bond</em>:d=4,o=6,b=112:16c.5,32d.5,32d.5,16d.5,8d.5,16c.5,16c.5,16c.5,
16c.5,32d#.5,32d#.5,16d#.5,8d#.5,16d.5,16d.5,16d.5,16c.5,32d.5,32d.5,16d.5,
8d.5,16c.5,16c.5,16c.5,16c.5,32d#.5,32d#.5,16d#.5,8d#.5,16d.5,16d.5,16d.5,
16c.5,32d.5,32d.5,16d.5,8d.5,16c.5,16c.5,16c.5,16c.5,32d#.5,32d#.5,16d#.5,
8d#.5,16d.5,16d.5,16c.5,16b.5,2a#5,16g.5,16f.5,2g.5
<em>MissionImp</em>:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,
32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,
8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,
8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d
<em>A-Team</em>:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,
f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#
<em>walklife</em>:d=4,o=6,b=80:8d.6,4d.6,32c.6,32d.6,16f.6,
32d.6,8c6,8a#.5,2a#5,32c.6,32d.6,8f.6,4f.6,32c.6,32d.6,16f.6,32d.6,8c6,8a#.5,
4a#.5,32c.6,32d.6,16f.6,32d.6,32c.6,32a#.5,8f.6,4f.6,32c.6,32d.6,16f.6,32d.6,
8c6,8a#.5,2a#5,32c.6,32d.6,8f.6,4f.6,32c.6,32d.6,16f.6,32d.6,8c6,8a#.5,4a#.5,
32c.6,32d.6,16f.6,32d.6,32c.6,32a#.5,8f6,8f6
<em>AuldLS</em>:d=4,o=6,b=101:g5,c,8c,c,e,d,8c,d,8e,8d,c,8c,
e,g,2a,a,g,8e,e,c,d,8c,d,8e,8d,c,8a5,a5,g5,2c
<em>Good Bad</em>:d=4,o=6,b=40:32b5,32e6,32b5,32e6,
8b.5,16g.5,16a.5,4e5,32b5,32e6,32b5,32e6,8b.5,16g.5,16a.5,4d6,32b5,32e6,
32b5,32e6,8b.5,16g.5,32f#.5,32e.5,4d5,32b5,32e6,32b5,32e6,8b.5,16a.5,4e5,
<em>Wilhelmus</em>:d=4,o=5,b=90:d,g,g,8a,8b,8c6,8a,b,8a,8b,c6,b,8a,8g,a,2g,p,d,g,
g,8a,8b,8c6,8a,b,8a,8b,c6,b,8a,8g,a,2g,p,8b,8c6,2d6,e6,2d6,c6,b,8a,8b,c6,b,
a,g,2a,p,d,8g,8f#,8g,8a,b,2a,g,f#,d,8e,8f#,g,g,f#,2g
<em>Indiana</em>:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,
1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,
8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6
<em>Godfather</em>:d=4,o=5,b=160:8g,8c6,8d#6,8d6,8c6,8d#6,8c6,8d6,c6,8g#,8a#,2g,8p,
8g,8c6,8d#6,8d6,8c6,8d#6,8c6,8d6,c6,8g,8f#,2f,8p,8f,8g#,8b,2d6,8p,8f,8g#,
8b,2c6,8p,8c,8d#,8a#,8g#,g,8a#,8g#,8g#,8g,8g,8b4,2c
<em>Final Countdown</em>:d=16,o=5,b=125:b,a,4b,4e,4p,8p,c6,b,8c6,8b,4a,4p,8p,c6,b,
4c6,4e,4p,8p,a,g,8a,8g,8f#,8a,4g.,f#,g,4a.,g,a,8b,8a,8g,8f#,4e,4c6,2b.,b,c6,b,a,1b
<em>Barbie Girl</em>:d=8,o=5,b=125:g#,e,g#,c#6,4a,4p,f#,d#,f#,b,
4g#,f#,e,4p,e,c#,4f#,4c#,4p,f#,e,4g#,4f#
<em>Back to the Future</em>:d=16,o=5,b=200:4g.,p,4c.,p,2f#.,p,g.,
p,a.,p,8g,p,8e,p,8c,p,4f#,p,g.,p,a.,p,8g.,p,8d.,p,8g.,p,
8d.6,p,4d.6,p,4c#6,p,b.,p,c#.6,p,2d.6
</code>

</main>
</body>
</html>



