<!DOCTYPE html>
<html>
<head>
<title>Onzin</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
<link rel="stylesheet" type="text/css" href="omelet.css"/>
</head>
<body>

<?php
printf(file_get_contents("mainmenu.xml"));
printf("<figure class=\"diagram\">");
$uxf = simplexml_load_file('kompakt.uxf');

foreach ($uxf->element as $a)
{
    if ($a->type == "com.umlet.element.Class")
    {
        $b = $a->coordinates;
        printf("<ul style=\"left: %upx; top: %upx; width: %upx; height: %upx;\">\n",
                $b->x, $b->y, $b->w, $b->h);

        $c = explode("--", $a->panel_attributes);

        foreach ($c as $d)
            printf("<li>%s</li>", $d);

        printf("</ul>\n");
    }

    if ($a->type == "com.umlet.element.Relation")
    {
        $b = $a->coordinates;
        printf("<figure style=\"left: %upx; top: %upx; width: %upx; height: %upx;\">\n",
                $b->x, $b->y, $b->w, $b->h);
        
        printf("<hr/>\n</figure>\n");
    }
}

?>
</figure>
</body>
</html>


