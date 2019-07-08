var body, tbody, imgs, tds;

function clearBgColor()
{
    for (var row = 0; row <= 2; row++)
        for (var col = 0; col <= 2; col++)
            tbody.rows[row].cells[col].bgColor = "";
}

function getEmpty()
{
    for (var row = 0; row <= 2; row++)
        for (var col = 0; col <= 2; col++)
            if (tbody.rows[row].cells[col].childElementCount == 0)
                return [row, col];
}

function getEmpty2()
{
    for (var i = 0; i < 9; i++)
        if (tds[i].childElementCount == 0)
            return i;
}

function allowDrop(ev)
{
    clearBgColor();
    ev.currentTarget.bgColor = "red";

    if (ev.currentTarget.childElementCount > 0)
        return;

    ev.preventDefault();
}

function drag(ev)
{
    ev.dataTransfer.setData("text", ev.target.id);
}

function stop(ev)
{
    clearBgColor();
}

function drop(ev)
{
    ev.preventDefault();
    var data = ev.dataTransfer.getData("text");
    ev.target.appendChild(document.getElementById(data));
    lock();
}

function lock()
{
    var empty2 = getEmpty2();

    for (var i = 0; i < 8; i++)
        imgs[i].draggable = false;

    switch (empty2)
    {
    case 0:
        tds[1].children[0].draggable = true;
        tds[3].children[0].draggable = true;
        break;
    case 1:
        tds[0].children[0].draggable = true;
        tds[2].children[0].draggable = true;
        tds[4].children[0].draggable = true;
        break;
    case 2:
        tds[1].children[0].draggable = true;
        tds[5].children[0].draggable = true;
        break;
    case 3:
        tds[0].children[0].draggable = true;
        tds[4].children[0].draggable = true;
        tds[6].children[0].draggable = true;
        break;
    case 4:
        tds[1].children[0].draggable = true;
        tds[3].children[0].draggable = true;
        tds[5].children[0].draggable = true;
        tds[7].children[0].draggable = true;
        break;
    case 5:
        tds[2].children[0].draggable = true;
        tds[4].children[0].draggable = true;
        tds[8].children[0].draggable = true;
        break;
    case 6:
        tds[3].children[0].draggable = true;
        tds[7].children[0].draggable = true;
        break;
    case 7:
        tds[4].children[0].draggable = true;
        tds[6].children[0].draggable = true;
        tds[8].children[0].draggable = true;
        break;
    case 8:
        tds[5].children[0].draggable = true;
        tds[7].children[0].draggable = true;
        break;
    }

}

function init()
{
    body = document.getElementsByTagName('body')[0];
    tbody = document.getElementById("puzzle");
    imgs = tbody.getElementsByTagName("img");
    tds = tbody.getElementsByTagName("td");
    lock();

    for (var i = 0; i < 8; i++)
    {
        imgs[i].addEventListener("dragstart", drag, false);
        imgs[i].addEventListener("dragend", stop, false);
    }
    
    for (var i = 0; i < 9; i++)
    {
        tds[i].addEventListener("drop", drop, false);
        tds[i].addEventListener("dragover", allowDrop, false);
    }
}

document.addEventListener("DOMContentLoaded", init, false);




