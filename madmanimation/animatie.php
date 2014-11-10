<!DOCTYPE html>
<?php
echo file_get_contents("../achtung.html");
?>
<html>
<head>
<title>Madmanimation</title>
<link rel="stylesheet" type="text/css" href="../common.css"/>
<link rel="stylesheet" type="text/css" href="common.css"/>
<link rel="stylesheet" type="text/css" href="animatie.css"/>
<link rel="stylesheet" type="text/css" href="scene01.css"/>
<link rel="stylesheet" type="text/css" href="scene02.css"/>
<link rel="stylesheet" type="text/css" href="scene03.css"/>
<link rel="stylesheet" type="text/css" href="scene04.css"/>
<link rel="stylesheet" type="text/css" href="scene05.css"/>
<link rel="stylesheet" type="text/css" href="scene06.css"/>
<link rel="stylesheet" type="text/css" href="scene07.css"/>
<link rel="stylesheet" type="text/css" href="scene08.css"/>
<link rel="stylesheet" type="text/css" href="scene09.css"/>
<link rel="stylesheet" type="text/css" href="scene10.css"/>
<link rel="stylesheet" type="text/css" href="scene11.css"/>
<link rel="stylesheet" type="text/css" href="scene12.css"/>
<link rel="stylesheet" type="text/css" href="scene13.css"/>
<link rel="stylesheet" type="text/css" href="scene14.css"/>
<link rel="stylesheet" type="text/css" href="scene15.css"/>
<link rel="stylesheet" type="text/css" href="scene16.css"/>
</head>
<body>
<?php
require_once('../mainmenu.php');
$mm = new MainMenu();
$mm->setDepth(1);
echo $mm;
?>
<h1 id="header"><a href="index.html">Watch</a> Madmanimation</h1>
<ol id="canvas">
	<li class="scene01">
		<p>
			Don Draper pauses in the doorway of his Manhattan
			office. Everything except fan blades are still. Don's
			desk is littered with bottles and glasses.
		</p>
		<div class="absolute">
			<img src="01-bg.jpg" alt="background"/>
			<div id="fan-01" class="absolute">
				<img src="01-bg-fan.png" alt=""/>
				<img id="fan-blade" src="01-fan-blade.png" alt=""/>
			</div>
		</div>
		<img class="man" src="01-man.png" alt="man"/>
	</li>
	<li id="scene02">
		<p>
			Don walks slowly into the room, his shoes
			reflected in the highly polished floor.
		</p>
        <h1><em>Jeffrey</em> Zeldman</h1>
		<img src="02-bg.jpg" alt="onzin"/>
		<img id="r-shadow" src="02-r-shadow.png" alt=""/>
		<img id="l-shadow" src="02-l-shadow.png" alt=""/>
		<div id="r-leg-02" class="absolute">
			<img id="r-foot-02" src="02-r-foot.png" alt=""/>
			<img id="r-shin-02" src="02-r-leg.png" alt=""/>
		</div>
		<div id="l-leg-02" class="absolute">
			<img src="02-l-foot-tip.png" alt=""/>
			<img src="02-l-foot.png" alt=""/>
			<img id="l-shin-02" src="02-l-leg.png" alt=""/>
		</div>
	</li>
	<li id="scene03" class="scene">
		<p>
			As Don puts down his briefcase, we begin to get a sense
			that everything may not be as it seems in his world.
		</p>
        <h1><em>Sarah</em> Parmenter</h1>
        <img class="background" src="03-bg.jpg" alt="onzin"/>
        <img id="legs-03" class="legs" src="03-legs.png" alt=""/>
        <img id="multi-slats-03" src="03-slats-multi.png" alt=""/>
        <img id="single-slats-03" src="03-slats-single.png" alt=""/>
        <img id="shadow03" class="shadow" src="03-suitcase-shadow.png" alt=""/>
        <img id="suitcase03" class="suitcase" src="03-suitcase.png" alt=""/>
        <img id="arm03" class="arm" src="03-arm.png" alt=""/>
	</li>
	<li id="scene04">
		<p>Magazines begin falling to the floor.</p>
        <h1><em>Jason</em> Santa Maria</h1>
		<img src="04-bg.jpg" alt=""/>
        <img id="ad-a" src="04-ad-a.jpg" alt="" title="Hardboiled Web Design"/>
        <img id="ad-b" src="04-ad-b.jpg" alt=""/>
        <img id="ad-c" src="04-ad-c.jpg" alt=""/>
	</li>
	<li id="scene05">
        <p>
            As he stands motionless, the room falls apart and Don realises
            that none of it’s real and that it’s impossible to
            tell where fiction stops and life begins.
        </p>
		<img class="background" src="05-bg.jpg" alt="onzin"/>
        <div class="fan">
            <img id="bg-fan-05" src="01-bg-fan.png" alt=""/>
            <img id="fan-blade-05" src="01-fan-blade.png" alt=""/>
        </div>
        <img id="shade-a-05" src="05-shade-a.png" alt=""/>
        <img id="shade-b-05" src="05-shade-b.png" alt=""/>
        <img id="shade-c-05" src="05-shade-c.png" alt=""/>
        <img src="05-blinds-a.png" alt=""/>
        <img src="05-blinds-b.png" alt=""/>
        <img class="books" src="05-books.png" alt=""/>
        <img class="lines" src="05-lines.png" alt=""/>
        <img src="05-desk-shadow.png" alt=""/>
        <img src="05-desk-top.png" alt=""/>
        <img class="deskside" src="05-desk-side.png" alt=""/>
        <img class="deskfront" src="05-desk-front.png" alt=""/>
        <img class="desklegs" src="05-desk-legs.png" alt=""/>
        <img class="lamp" src="05-lamp.png" alt=""/>
        <div class="chair5a">
            <img src="05-chair-shadow-a.png" alt=""/>
            <img src="05-chair-slice-a-1.png" alt=""/>
            <img src="05-chair-slice-a-2.png" alt=""/>
            <img src="05-chair-slice-a-3.png" alt=""/>
        </div>
        <div id="chair5b">
            <img src="05-chair-shadow-b.png" alt=""/>
            <img src="05-chair-slice-b-1.png" alt=""/>
            <img src="05-chair-slice-b-2.png" alt=""/>
            <img src="05-chair-slice-b-3.png" alt=""/>
        </div>
        <img class="lights5a" alt="" src="05-lights-a.png"/>
        <img class="lights5b" alt="" src="05-lights-b.png"/>
        <img class="lights5c" alt="" src="05-lights-c.png"/>
        <img class="lights5d" alt="" src="05-lights-d.png"/>
        <img class="man" alt="man" src="05-man.png"/>
        <img class="suitcase" alt="suitcase" src="05-suitcase.png"/>
        <img class="ads5a" alt="ads5a" src="04-ad-a.jpg"/>
        <img class="ads5b" alt="ads5b" src="04-ad-b.jpg"/>
        <img class="ads5c" alt="ads5c" src="04-ad-c.jpg"/>
	</li>
	<li id="scene06">
        <p>
            The room melts away and Don starts
            fall, his arms are outstretched.
        </p>
		<img src="06-bg.jpg" alt="onzin"/>
        <img id="man-06" src="06-man.png" alt=""/>
	</li>
	<li class="scene07">
        <p>
            As Don tumbles down, photographs from magazine
            advertisements merge with his own reflection.
        </p>
        <h1><em>Luke</em> Wroblewski</h1>
		<img src="07-bg.jpg" alt="background"/>
        <img class="building" src="07-control-full.jpg" alt="building"/>
        <img class="man" src="07-man.png" alt="man"/>
	</li>
	<li id="scene08">
        <p>
            Looking out from a building across the street, the
            helplessness of Don’s situation becomes clearer
            as he falls further and faster.
        </p>
        <h1 id="title-08-a"><span>Eric</span> Meyer</h1>
        <h1 id="title-08-b"><span>Jeremy</span> Keith</h1>
		<img id="bg-08" src="08-bg.jpg" alt="background"/>
        <img id="man-08" src="08-man.png" alt="man"/>
	</li>
    <li class="scene09">
        <p>
            Don accepts his fate and relaxes as
            he falls. Behind him, photographs from magazine
            advertisements continue to reflect in the windows.
        </p>
        <h1 id="title-09-a">
            <em>And</em>
            <br/>
            <span>Aaron</span> Walter
        </h1>
        <h1 id="title-09-b">
            <small>Casting by</small>
            <br/>
            <em>Alexa</em> Andrzejewski <small>&amp;</small>
            <br/>
            <em>Jeffrey</em> Veen
        </h1>
        <img class="background" src="09-bg.jpg" alt="background"/>
        <img class="man" src="09-man.png" alt="man"/>
    </li>
    <li class="scene10">
        <p>
            Don is in free-fall. The images behind him merge
            into one continuous stream. Images of alcohol.
        </p>
        <h1 id="title10a"><em>Editor</em><br/><span>Anthony</span> Calzadilla</h1>
        <h1 id="title10b"><em>Production Designer</em><br/><span>Andy</span> Clarke</h1>
        <img class="background" src="10-bg.jpg" alt=""/>
        <img id="man-10" class="man" src="10-man.png" alt=""/>
    </li>
    <li id="scene11">
        <p>
            Of wedding bands - things that are
            wrong and right in his life.
        </p>
        <h1><em>Opening Night Party</em><br/><span>(mt)</span> Media Temple</h1>
        <img class="background" src="11-bg.jpg" alt=""/>
        <img id="man-11" src="11-man.png" alt=""/>
    </li>
    <li class="scene12">
        <p>Glamourous women.</p>
        <img class="background" src="12-bg.jpg" alt="background"/>
        <img class="man" src="12-man.png" alt="man"/>
    </li>
    <li id="scene13">
        <p>Happy families.</p>
        <h1><em>Produced by</em><br/><span>Anthony</span> Calzadilla</h1>
        <div id="bldg-blk">
            <img src="13a-bg.jpg" alt=""/>
            <img src="13b-bg.jpg" alt=""/>
        </div>
        <img class="man" src="13-man.png" alt="man"/>
    </li>
    <li class="scene14">
        <p>
            As reality fades away, all that's left are images
            from an unreal world, spiralling out of control.
        </p>
        <h1><small>Executive Producer</small><span>Andy</span>Clarke</h1>
        <img class="background" src="14-bg.jpg" alt=""/>
    </li>
    <li class="scene15">
        <p>
            Don accepts his fate, that his work
            and his life are one and the same.
        </p>
        <img class="background" src="15-bg.jpg" alt="background15"/>
        <img class="man" src="15-man.png" alt=""/>
    </li>
    <li class="scene16">
        <p>
            He sits, his arm outstretched across the back
            of a chair. A cigarette between his fingers.
        </p>
        <h1><em>Mad</em><span class="title-men">manimation</span></h1>
        <img src="06-bg.jpg" alt=""/>
        <img id="man16" src="16-man.png" alt="man16"/>
    </li>
</ol>
</body>
</html>
