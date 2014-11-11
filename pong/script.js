
var p1up = 0;
var p1down = 0;
var p2up = 0;
var p2down = 0;

function init()
{
    score1 = window.document.getElementById("score1");
    score2 = window.document.getElementById("score2");
    ball = window.document.getElementById("ball");
    ball.myTop = 200;
    ball.myLeft = 400;
    ball.speedX = 10;
    ball.speedY = 10;
    paddle1 = window.document.getElementById("paddle1");
    paddle2 = window.document.getElementById("paddle2");
    paddle1.myMarginTop = 100;
    paddle2.myMarginTop = 100;
    paddle1.style.marginTop = paddle1.myMarginTop + "px";
    paddle2.style.marginTop = paddle2.myMarginTop + "px";
    paddle1.score = 0;
    paddle2.score = 0;
    interval = setInterval('beweeg()', 50);
}

function beweeg()
{
    if (p1up && paddle1.myMarginTop >= 10)
    {
        paddle1.myMarginTop -= 10;
        paddle1.style.marginTop = paddle1.myMarginTop + "px";
    }

    if (p1down && paddle1.myMarginTop <= 380)
    {
        paddle1.myMarginTop += 10;
        paddle1.style.marginTop = paddle1.myMarginTop + "px";
    }

    if (p2up && paddle2.myMarginTop >= 10)
    {
        paddle2.myMarginTop -= 10;
        paddle2.style.marginTop = paddle2.myMarginTop + "px";
    }

    if (p2down && paddle2.myMarginTop <= 380)
    {
        paddle2.myMarginTop += 10;
        paddle2.style.marginTop = paddle2.myMarginTop + "px";
    }

    if (ball.myTop > 470)
        ball.speedY *= -1;

    if (ball.myTop < 10)
        ball.speedY *= -1;

    if (ball.myLeft > 870)
    {
        if (ball.myTop >= paddle2.myMarginTop && ball.myTop <= (paddle2.myMarginTop + 100))
        {
            ball.speedX *= -1;
        }
        else
        {
            ball.myTop = 200;
            ball.myLeft = 400;
            paddle1.score++;
            score1.textContent = "Player 1: " + paddle1.score;
        }
    }

    if (ball.myLeft < 10)
    {
        if (ball.myTop >= paddle1.myMarginTop && ball.myTop <= (paddle1.myMarginTop + 100))
        {
            ball.speedX *= -1;
        }
        else
        {
            ball.myTop = 200;
            ball.myLeft = 400;
            paddle2.score++;
            score2.textContent = "Player 2: " + paddle2.score;
        }
    }

    ball.myTop += ball.speedY;
    ball.myLeft += ball.speedX;
    ball.style.top = ball.myTop + "px";
    ball.style.left = ball.myLeft + "px";
}

function onzin(e)
{
    switch (e.keyCode)
    {
        case 87:
            p1up = 1;
            break;
        case 83:
            p1down = 1;
            break;
        case 38:
            p2up = 1;
            break;
        case 40:
            p2down = 1;
            break;
    }
}

function keyup(e)
{
    switch (e.keyCode)
    {
        case 87:
            p1up = 0;
            break;
        case 83:
            p1down = 0;
            break;
        case 38:
            p2up = 0;
            break;
        case 40:
            p2down = 0;
            break;
    }
}

document.onkeydown = onzin;
document.onkeyup = keyup;
document.addEventListener("DOMContentLoaded", init, false);

