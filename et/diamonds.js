
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
    var bmpSpaceship = document.createElement("img");
    bmpSpaceship.setAttribute("src", "spaceship.gif");
    bmpSpaceship.setAttribute("alt", "Spaceship");
    bmpSpaceship.style.position = "absolute";
    bmpSpaceship.style.top = "200px";
    bmpSpaceship.style.left = "299px";
    document.body.appendChild(afbeelding);
    document.body.appendChild(bmpSpaceship);
    return;
}

document.addEventListener("DOMContentLoaded", init, false);


