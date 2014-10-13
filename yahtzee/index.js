var imgDob;
var spanNummer;
var intervalSet;

function newTurn()
{
    sYahtzeeBonus[0].set = false;
    imgDob.turnsRemaining--;
    imgDob.remaining = 3;
    imgDob.clearMask();
    spanNummer.innerHTML = "x" + imgDob.remaining;
    sTotal[0].innerHTML = calcUpperScore().toString();
    sLowerTotal[0].innerHTML = calcLowerScore().toString();

    if (calcUpperScore() >= 63)
    {
        sBonus[0].innerHTML = "35";
        sBonus[0].score = 35;
    }

    if (upperAllSet())
    {
        if (isNaN(sBonus[0].score))
            sBonus[0].score = 0;
        sUpperTotal[0].innerHTML = (calcUpperScore() + sBonus[0].score).toString();
        sUpperTotal2[0].innerHTML = (calcUpperScore() + sBonus[0].score).toString();
    }
        
    if (imgDob.turnsRemaining == 0)
    {
        var score = calcUpperScore() + sBonus[0].score + calcLowerScore();
        sGrandTotal[0].innerHTML = score.toString();
        var form = document.getElementsByTagName("form");
        var formScore = document.getElementById("grandScore");
        formScore.value = score.toString();
        var naam = prompt("Geef uw naam op!", "");
        var formNaam = document.getElementById("naam");
        formNaam.value = naam;
        form[0].submit();
    }
}

function upperAllSet()
{
    return (sAces[0].set &&
            sTwos[0].set &&
            sThrees[0].set &&
            sFours[0].set &&
            sFives[0].set &&
            sSixes[0].set);
}

function calcUpperScore()
{
    return sAces[0].score +
           sTwos[0].score +
           sThrees[0].score +
           sFours[0].score +
           sFives[0].score +
           sSixes[0].score;
}

function calcLowerScore()
{
    return sThreeOfKind[0].score +
           sFourOfKind[0].score +
           sFullHouse[0].score +
           sSmStraight[0].score +
           sLgStraight[0].score +
           sYahtzee[0].score +
           sChance[0].score +
           sYahtzeeBonus[0].score;
}

function init()
{
    intervalSet = false;
    imgDob = document.getElementsByClassName("dobbel");
    imgDob.remaining = 3;
    imgDob.turnsRemaining = 13;
    for (var i = 0; i < imgDob.length; i++)
    {
        imgDob[i].toggleKeep = function()
        {
            if (this.keep)
                this.keep = false;
            else
                this.keep = true;
        };
    }
    imgDob.clearMask = function()
    {
        for (var i = 0; i < imgDob.length; i++)
        {
            imgDob[i].keep = false;
            imgDob[i].className = "dobbel";
        }
    };

    sAces = document.getElementsByClassName("aces");
    sTwos = document.getElementsByClassName("twos");
    sThrees = document.getElementsByClassName("threes");
    sFours = document.getElementsByClassName("fours");
    sFives = document.getElementsByClassName("fives");
    sSixes = document.getElementsByClassName("sixes");
    sTotal = document.getElementsByClassName("total");
    sBonus = document.getElementsByClassName("bonus");
    sUpperTotal = document.getElementsByClassName("upperTotal");
    sThreeOfKind = document.getElementsByClassName("threeOfKind");
    sFourOfKind = document.getElementsByClassName("fourOfKind");
    sFullHouse = document.getElementsByClassName("fullHouse");
    sSmStraight = document.getElementsByClassName("smstraight");
    sLgStraight = document.getElementsByClassName("lgstraight");
    sYahtzee = document.getElementsByClassName("yahtzee");
    sChance = document.getElementsByClassName("chance");
    sYahtzeeBonus = document.getElementsByClassName("yahtzeeBonus");
    sLowerTotal = document.getElementsByClassName("lowerTotal");
    sUpperTotal2 = document.getElementsByClassName("upperTotal2");
    sGrandTotal = document.getElementsByClassName("grandTotal");


    spanNummer = document.getElementById("spanNummer");

    for (var i = 0; i < 1; i++)
    {
        sAces[i].score = 0;
        sAces[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcUpper(1)
        }, false);

        sAces[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);

        sAces[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcUpper(1);
                this.score = calcUpper(1);
                this.set = true;
                newTurn();
            }
        }, false);

        sTwos[i].score = 0;
        sTwos[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcUpper(2);
        }, false);
        sTwos[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sTwos[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcUpper(2);
                this.score = calcUpper(2);
                this.set = true;
                newTurn();
            }
        }, false);

        sThrees[i].score = 0;
        sThrees[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcUpper(3);
        }, false);
        sThrees[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sThrees[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcUpper(3);
                this.score = calcUpper(3);
                this.set = true;
                newTurn();
            }
        }, false);

        sFours[i].score = 0;
        sFours[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcUpper(4);
        }, false);
        sFours[i].addEventListener("mouseout", function()
        {
            if (!this.set) this.innerHTML = "";
        }, false);

        sFours[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcUpper(4);
                this.score = calcUpper(4);
                this.set = true;
                newTurn();
            }
        }, false);

        sFives[i].score = 0;
        sFives[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcUpper(5);
        }, false);
        sFives[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sFives[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcUpper(5);
                this.score = calcUpper(5);
                this.set = true;
                newTurn();
            }
        }, false);

        sSixes[i].score = 0;
        sSixes[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcUpper(6) }, false);
        sSixes[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sSixes[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcUpper(6);
                this.score = calcUpper(6);
                this.set = true;
                newTurn();
            }
        }, false);

        sThreeOfKind[i].score = 0;
        sThreeOfKind[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcOfKind(3) }, false);
        sThreeOfKind[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "" }, false);

        sThreeOfKind[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcOfKind(3);
                this.score = calcOfKind(3);
                this.set = true;
                newTurn();
            }
        }, false);

        sFourOfKind[i].score = 0;
        sFourOfKind[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcOfKind(4);
        }, false);
        sFourOfKind[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);

        sFourOfKind[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcOfKind(4);
                this.score = calcOfKind(4);
                this.set = true;
                newTurn();
            }
        }, false);

        sFullHouse[i].score = 0;
        sFullHouse[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcFullHouse();
        }, false);
        sFullHouse[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sFullHouse[i].addEventListener("click", function() {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcFullHouse();
                this.score = calcFullHouse();
                this.set = true;
                newTurn();
            }
        }, false);

        sSmStraight[i].score = 0;
        sSmStraight[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcSmStraight();
        }, false);
        sSmStraight[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sSmStraight[i].addEventListener("click", function()
        {
            if (!this.set)
            {
                this.innerHTML = calcSmStraight();
                this.score = calcSmStraight();
                this.set = true;
                newTurn();
            }
        }, false);

        sLgStraight[i].score = 0;
        sLgStraight[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcLgStraight();
        }, false);
        sLgStraight[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        sLgStraight[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcLgStraight();
                this.score = calcLgStraight();
                this.set = true;
                newTurn();
            }
        }, false);

        sYahtzee[i].score = 0;
        sYahtzee[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcYahtzee();
        }, false);
        
        sYahtzee[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        
        sYahtzee[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcYahtzee();
                this.score = calcYahtzee();
                this.set = true;
                newTurn();
            }
        }, false);

        sChance[i].score = 0;
        sChance[i].addEventListener("mouseover", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = calcSum();
        }, false);
        sChance[i].addEventListener("mouseout", function()
        {
            if (!this.set && imgDob.remaining < 3)
                this.innerHTML = "";
        }, false);
        
        sChance[i].addEventListener("click", function()
        {
            if (!this.set && imgDob.remaining < 3)
            {
                this.innerHTML = calcSum();
                this.score = calcSum();
                this.set = true;
                newTurn();
            }
        }, false);

        sYahtzeeBonus[i].score = 0;
        sYahtzeeBonus[i].addEventListener("mouseover", function()
        {
            if (imgDob.remaining < 3 && sYahtzee[0].score == 50)
                this.innerHTML = calcYahtzeeBonus().toString();
        }, false);
        sYahtzeeBonus[i].addEventListener("mouseout", function()
        {
            if (imgDob.remaining < 3)
            {
                if (this.score > 0)
                    this.innerHTML = this.score;
                else
                    this.innerHTML = "";
            }
        }, false);
        sYahtzeeBonus[i].addEventListener("click", function()
        {
            if (imgDob.remaining < 3)
            {
                this.innerHTML = calcYahtzeeBonus();
                this.score = calcYahtzeeBonus();
                this.set = true;
            }
        }, false);
    }
}

function calcUpper(ogen)
{
    var counter = 0;
    for (var i = 0; i < imgDob.length; i++)
        if (imgDob[i].waarde == ogen)
            counter++;
    return counter * ogen;
}

function hold(nr)
{
    if (imgDob.remaining < 3)
        imgDob[nr].toggleKeep();

    if (imgDob[nr].keep)
        imgDob[nr].className += " hold";
    else
        imgDob[nr].className = "dobbel";
}

function beweeg()
{
    if (typeof max == 'undefined')
        max = 0;

    if (++max > 9)
    {
        window.clearInterval(interval);
        intervalSet = false;
        max = 0;
    }

    for (var i = 0; i < 5; i++)
    {
        var randomNum = Math.floor(Math.random() * (6 - 1 + 1)) + 1;
        if (!imgDob[i].keep)
        {
            imgDob[i].src = "http://localhost/yahtzee/Dice-" + randomNum + ".svg";
            imgDob[i].waarde = randomNum;
        }
    }
}

function gooien()
{
    if (intervalSet)
        return;
    if (imgDob.remaining < 1)
        return;
    interval = setInterval('beweeg()', 200);
    intervalSet = true;
    imgDob.remaining--;
    spanNummer.innerHTML = "x" + imgDob.remaining;
    
}

function has(ogen)
{
    for (var i = 0; i < imgDob.length; i++)
        if (imgDob[i].waarde == ogen)
            return true;
    return false;
}

function calcOfKind(n)
{
    for (var ogen = 1; ogen <= 6; ogen++)
    {
        var sequence = 0;
        for (var i = 0; i < imgDob.length; i++)
            if (imgDob[i].waarde == ogen)
                sequence++;

        if (sequence >= n)
            return calcSum();
    }
    return 0;
}

function calcFullHouse()
{
    for (var ogen = 1; ogen <= 6; ogen++)
    {
        var sequence = 0;
        for (var i = 0; i < imgDob.length; i++)
            if (imgDob[i].waarde == ogen)
                sequence++;

            if (sequence == 1 || sequence == 4 || sequence == 5)
                return 0;
    }
    return 25;
}

function calcSmStraight()
{
    if (has(3) && has(4))
    {
        if (has(2))
            if (has(1) || has(5))
                return 30;

        if (has(5))
            if (has(2) || has(6))
                return 30;
    }
    return 0;
}

function calcLgStraight()
{
    if (has(2) && has(3) && has(4) && has(5))
        if (has(1) || has(6))
            return 40;
    return 0;
}

function calcYahtzee()
{
    return isYahtzee() ? 50 : 0;
}

function isYahtzee()
{
    for (var i = 0; i < imgDob.length; i++)
        if (imgDob[i].waarde != imgDob[0].waarde)
            return false;
    return true;
}

function calcSum()
{
    totaal = 0;
    for (var i = 0; i < imgDob.length; i++)
        totaal += imgDob[i].waarde;
    return totaal;
}

function calcYahtzeeBonus()
{
    if (isYahtzee() && !sYahtzeeBonus[0].set)
        return sYahtzeeBonus[0].score + 100;
    return sYahtzeeBonus[0].score;
}

document.addEventListener("DOMContentLoaded", init, false);


