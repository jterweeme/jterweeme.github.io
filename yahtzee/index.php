<!DOCTYPE html>
<?php
echo file_get_contents('../achtung.html');
?>
<html>
<head>
    <title>Yahtzee</title>
    <link rel="stylesheet" type="text/css" href="../common.css"/>
    <link rel="stylesheet" type="text/css" href="index.css"/>
    <script src="index.js"></script>
</head>
<body>
<?php
require_once('../mainmenu.php');
$mm = new MainMenu();
echo $mm;
?>

<table class="score">
    <caption>
        <img src="yahtzee.svg" alt="logo"/>
        <dl>
            <dt>NAME</dt>
            <dd></dd>
        </dl>
    </caption>
    <tr>
        <th>UPPER SECTION</th>
        <th>HOW TO SCORE</th>
        <th>GAME<br/>#1</th>
        <th>GAME<br/>#2</th>
        <th>GAME<br/>#3</th>
        <th>GAME<br/>#4</th>
        <th>GAME<br/>#5</th>
        <th>GAME<br/>#6</th>
    </tr>
    <tr>
        <td>
            <span>Aces</span>
            <img src="Dice-1.svg" alt="dice1"/>
            <span> = 1</span>
        </td>
        <td class="toelichting">Count and Add<br />Only Aces</td>
        <td id="aces1" class="aces"></td>
        <td id="aces2" class="aces"></td>
        <td id="aces3" class="aces"></td>
        <td id="aces4" class="aces"></td>
        <td id="aces5" class="aces"></td>
        <td id="aces6" class="aces"></td>
    </tr>
    <tr>
        <td>
            <span>Twos</span>
            <img src="Dice-2.svg" alt="dice2"/>
            <span> = 2</span>
        </td>
        <td class="toelichting">Count and Add<br />Only Twos</td>
        <td id="twos1" class="twos"></td>
        <td class="twos"></td>
        <td class="twos"></td>
        <td class="twos"></td>
        <td class="twos"></td>
        <td class="twos"></td>
    </tr>
    <tr>
        <td>
            <span>Threes</span>
            <img src="Dice-3.svg" alt="dice3"/>
            <span> = 3</span>
        </td>
        <td class="toelichting">Count and Add<br />Only Threes</td>
        <td id="threes1" class="threes"></td>
        <td class="threes"></td>
        <td class="threes"></td>
        <td class="threes"></td>
        <td class="threes"></td>
        <td class="threes"></td>
    </tr>
    <tr>
        <td>
            <span>Fours</span>
            <img src="Dice-4.svg" alt="dice4"/>
            <span> = 4</span>
        </td>
        <td class="toelichting">Count and Add<br />Only Fours</td>
        <td id="fours1" class="fours"></td>
        <td class="fours"></td>
        <td class="fours"></td>
        <td class="fours"></td>
        <td class="fours"></td>
        <td class="fours"></td>
    </tr>
    <tr>
        <td>
            <span>Fives</span>
            <img src="Dice-5.svg" alt="dice5"/>
            <span> = 5</span>
        </td>
        <td class="toelichting">Count and Add<br />Only Fives</td>
        <td id="fives1" class="fives"></td>
        <td class="fives"></td>
        <td class="fives"></td>
        <td class="fives"></td>
        <td class="fives"></td>
        <td class="fives"></td>
    </tr>
    <tr>
        <td>
            <span>Sixes</span>
            <img src="Dice-6.svg" alt="dice6"/>
            <span> = 6</span>
        </td>
        <td class="toelichting">Count and Add<br />Only Sixes</td>
        <td id="sixes1" class="sixes"></td>
        <td class="sixes"></td>
        <td class="sixes"></td>
        <td class="sixes"></td>
        <td class="sixes"></td>
        <td class="sixes"></td>
    </tr>
    <tr>
        <td>TOTAL SCORE</td>
        <td class="toelichting">&#8594;</td>
        <td class="total"></td>
        <td class="total"></td>
        <td class="total"></td>
        <td class="total"></td>
        <td class="total"></td>
        <td class="total"></td>
    </tr>
    <tr>
        <td>
            <dl>
                <dt>BONUS</dt>
                <dd>If total score<br/>is 63 or over</dd>
            </dl>
        </td>
        <td class="toelichting">SCORE 35</td>
        <td class="bonus"></td>
        <td class="bonus"></td>
        <td class="bonus"></td>
        <td class="bonus"></td>
        <td class="bonus"></td>
        <td class="bonus"></td>
    </tr>
    <tr>
        <td><dl><dt>TOTAL</dt><dd>Of Upper<br/>Section</dd></dl></td>
        <td class="toelichting">&#8594;</td>
        <td class="upperTotal"></td>
        <td class="upperTotal"></td>
        <td class="upperTotal"></td>
        <td class="upperTotal"></td>
        <td class="upperTotal"></td>
        <td class="upperTotal"></td>
    </tr>
    <tr>
        <td class="noborder">LOWER SECTION</td>
    </tr>
    <tr>
        <td>3 of a kind</td>
        <td class="toelichting">Add Total<br />Of All Dice</td>
        <td id="threeOfKind1" class="threeOfKind"></td>
        <td id="threeOfKind2" class="threeOfKind"></td>
        <td class="threeOfKind"></td>
        <td class="threeOfKind"></td>
        <td class="threeOfKind"></td>
        <td class="threeOfKind"></td>
    </tr>
    <tr>
        <td>4 of a kind</td>
        <td class="toelichting">Add Total<br />Of All Dice</td>
        <td id="fourOfKind1" class="fourOfKind"></td>
        <td class="fourOfKind"></td>
        <td class="fourOfKind"></td>
        <td class="fourOfKind"></td>
        <td class="fourOfKind"></td>
        <td class="fourOfKind"></td>
    </tr>
    <tr>
        <td>Full house</td>
        <td class="toelichting">SCORE 25</td>
        <td class="fullHouse"></td>
        <td class="fullHouse"></td>
        <td class="fullHouse"></td>
        <td class="fullHouse"></td>
        <td class="fullHouse"></td>
        <td class="fullHouse"></td>
    </tr>
    <tr>
        <td>Sm. Straight</td>
        <td class="toelichting">SCORE 30</td>
        <td class="smstraight"></td>
        <td class="smstraight"></td>
        <td class="smstraight"></td>
        <td class="smstraight"></td>
        <td class="smstraight"></td>
        <td class="smstraight"></td>
    </tr>
    <tr>
        <td>Lg. Straight</td>
        <td class="toelichting">SCORE 40</td>
        <td class="lgstraight"></td>
        <td class="lgstraight"></td>
        <td class="lgstraight"></td>
        <td class="lgstraight"></td>
        <td class="lgstraight"></td>
        <td class="lgstraight"></td>
    </tr>
    <tr>
        <td>YAHTZEE</td>
        <td class="toelichting">SCORE 50</td>
        <td id="yahtzee1" class="yahtzee"></td>
        <td class="yahtzee"></td>
        <td class="yahtzee"></td>
        <td class="yahtzee"></td>
        <td class="yahtzee"></td>
        <td class="yahtzee"></td>
    </tr>
    <tr>
        <td>Chance</td>
        <td class="toelichting">Score Total<br />Of All 5 Dice</td>
        <td id="chance1" class="chance"></td>
        <td class="chance"></td>
        <td class="chance"></td>
        <td class="chance"></td>
        <td class="chance"></td>
        <td class="chance"></td>
    </tr>
    <tr>
        <td>YAHTZEE BONUS</td>
        <td></td>
        <td class="yahtzeeBonus"></td>
        <td class="yahtzeeBonus"></td>
        <td class="yahtzeeBonus"></td>
        <td class="yahtzeeBonus"></td>
        <td class="yahtzeeBonus"></td>
        <td class="yahtzeeBonus"></td>
    </tr>
    <tr>
        <td><dl><dt>TOTAL</dt><dd>Of Lower<br/>Section</dd></dl></td>
        <td class="toelichting">&#8594;</td>
        <td class="lowerTotal"></td>
        <td class="lowerTotal"></td>
        <td class="lowerTotal"></td>
        <td class="lowerTotal"></td>
        <td class="lowerTotal"></td>
        <td class="lowerTotal"></td>
    </tr>
    <tr>
        <td><dl><dt>TOTAL</dt><dd>Of Upper<br/>Section</dd></dl></td>
        <td class="toelichting">&#8594;</td>
        <td class="upperTotal2"></td>
        <td class="upperTotal2"></td>
        <td class="upperTotal2"></td>
        <td class="upperTotal2"></td>
        <td class="upperTotal2"></td>
        <td class="upperTotal2"></td>
    </tr>
    <tr>
        <td>GRAND TOTAL</td>
        <td class="toelichting">&#8594;</td>
        <td class="grandTotal"></td>
        <td class="grandTotal"></td>
        <td class="grandTotal"></td>
        <td class="grandTotal"></td>
        <td class="grandTotal"></td>
        <td class="grandTotal"></td>
    </tr>
</table>
<nav class="sidebar">
    <a href="index.html">New Game</a>
    <a href="hiscore.php">High Score</a>
    <table class="menu">
        <tr>
            <th colspan="5">
                <img id="btnDice" src="dice.jpeg" alt="dobbelstenen" onclick="gooien()"/>
                <span id="spanNummer">x3</span>
            </th>
        </tr>
        <tr>
            <td><img src="Dice-1.svg" alt="dice1" class="dobbel" onclick="hold(0)"/></td>
            <td><img src="Dice-2.svg" alt="dice2" class="dobbel" onclick="hold(1)"/></td>
            <td><img src="Dice-3.svg" alt="dice3" class="dobbel" onclick="hold(2)"/></td>
            <td><img src="Dice-4.svg" alt="dice4" class="dobbel" onclick="hold(3)"/></td>
            <td><img src="Dice-5.svg" alt="dice5" class="dobbel" onclick="hold(4)"/></td>
        </tr>
    </table>
</nav>
<form action="hiscore.php" method="post">
    <input type="number" name="grandScore" id="grandScore"/>
    <input type="text" name="naam" id="naam"/>
</form>
</body>
</html>


