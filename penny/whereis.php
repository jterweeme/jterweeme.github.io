<!DOCTYPE html>
<?php
echo file_get_contents("../achtung.html");
?>
<html class="whereis">
<head>
<title>Penny Juice</title>
<link rel="stylesheet" type="text/css" href="../common.css"/>
<link rel="stylesheet" type="text/css" href="index.css"/>
</head>
<body>
<?php
require_once("../mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>
<main>
<img src="whereis.jpg" alt="Where is"/>
<table>
<tbody>
    <tr>
        <td colspan="6"><img src="finalmap.jpg" alt="Map"/></td>
    </tr>
    <tr>
        <td colspan="6">
Local distributors available in these areas.
(If you do not find a distributor in your area we will gladly ship directly to your door.)
        </td>
    </tr>
    <tr>
        <td colspan="2">Des Moines, IA</td>
        <td colspan="2">Quad Cities</td>
        <td colspan="2">Chicago, IL</td>
    </tr>
    <tr>
        <td colspan="2">Milwaukee, WI</td>
        <td colspan="2">Lincoln, NE</td><td colspan="2">Sioux Falls, SD</td>
    </tr>
    <tr>
        <td colspan="2">Kansas City, MO</td>
        <td colspan="2">St. Louis, MO</td><td colspan="2">Green Bay, WI</td>
    </tr>
    <tr>
        <td colspan="2">Minneapolis, MN</td>
        <td colspan="2">Indianapolis, IN</td><td colspan="2">Louisville, KY</td>
    </tr>
    <tr>
        <td colspan="2">Tallahassee, FL</td>
        <td colspan="2">Jacksonville, FL</td><td colspan="2">Orlando, FL</td>
    </tr>
    <tr>
        <td colspan="2">Miami, FL</td>
        <td colspan="2">Los Angeles, CA</td><td colspan="2">San Diego, CA</td>
    </tr>
    <tr>
        <td colspan="2">San Francisco, CA</td>
        <td colspan="2">Denver, CO</td><td colspan="2">Phoenix, AZ</td>
    </tr>
    <tr>
        <td colspan="2">Tucson, AZ</td>
        <td colspan="2">Flagstaff, AZ</td>
        <td colspan="2">Albuquerque, NM</td>
    </tr>
    <tr>
        <td colspan="2">Memphis, TN</td>
        <td colspan="2">Nashville, TN</td>
        <td colspan="2">Knoxville, TN</td>
    </tr>
    <tr>
        <td colspan="2">Chattanooga, TN</td>
        <td colspan="2">Boston, Ma</td>
        <td colspan="2">Concord, NH</td>
    </tr>

    <tr>
        <td colspan="3"><img src="distavail.gif" alt="Distributorships Available"/></td>
        <td colspan="3">Got Juice?</td>
    </tr>
</tbody>
</table>
<nav>
    <a href="whois.html">Who is Penny Juice?</a>
    <span> / </span>
    <a href="whatis.html">What is Penny Juice?</a>
    <span> / </span>
    <a href="whereis.html">Where is Penny Juice?</a>
</nav>
<nav>
    <span>Got Juice? / E-mail / </span>
    <a href="index.html">HOME</a>
</nav>
</main>
</body>
</html>



