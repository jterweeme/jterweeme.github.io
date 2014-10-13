var bmpSpaceship;
var hoogte;

function init()
{
    var item = document.createTextNode("Hallo wereld");
    document.body.appendChild(item);
    var afbeelding = document.createElement("img");
    afbeelding.setAttribute("src", "links.gif");
    afbeelding.setAttribute("alt", "Extraterrestrial");
    afbeelding.style.position = "absolute";
    afbeelding.style.top = "100px";
    afbeelding.style.left = "100px";
    bmpSpaceship = document.createElement("img");
    bmpSpaceship.setAttribute("src", "spaceship.gif");
    bmpSpaceship.setAttribute("alt", "Spaceship");
    bmpSpaceship.style.position = "absolute";
    bmpSpaceship.style.top = "100px";
    bmpSpaceship.style.left = "301px";
    document.body.appendChild(afbeelding);
    document.body.appendChild(bmpSpaceship);

    window.hoogte = 20;
    iinterval = setInterval('land()', 50);
}

function land()
{
    window.hoogte++;
    var foobar;
    if (window.hoogte > 200)
    {
        //foobar = window.hoogte - 200;
        //foobar = 200 - foobar;
        //alert(foobar);
        bmpSpaceship.style.top = (200 - (window.hoogte - 200)) + "px";
    }
    else
        bmpSpaceship.style.top = hoogte + "px";
    if (window.hoogte > 380)
    {
        clearInterval(iinterval);
        bmpSpaceship.style.visibility = "hidden";
    }
    return;
}

document.addEventListener("DOMContentLoaded", init, false);


