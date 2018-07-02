function start()
{
    ampel = document.getElementById("ampel");
    lampen = ampel.getElementsByTagName("circle");
    lampen[0].style = "fill: red";
    cars = document.getElementsByClassName("car");
    horn = document.getElementById("horn");
    cars[0].onclick = function() { horn.play(); }
}



