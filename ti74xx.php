<!DOCTYPE html>
  
<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>TI 74xx series</title>
<link rel="stylesheet" type="text/css" href="common.css"/>
<style>
code, kbd, samp {
    white-space: pre;
}

var {
    font-family: monospace;
    font-style: normal;
}

samp {
    color: gray;
}

code {
    display: block;
    background-color: #eee;
    margin-bottom: 10px;
}

code em {
    font-style: normal;
    color: red;
}

svg, img {
    display: block;
    /*border: 1px dotted red;*/
}

svg text {
    stroke: none;
    fill: black;
}

svg path, svg g {
    stroke: black;
    fill: none;
}

.klas1 {
    stroke: black;
    fill: none;
}

.klas1 text {
    font-size: 8px;
}

#gmux text, #dflop text {
    stroke: none;
    fill: black;
    font-size: 8px;
}
</style>
</head>
<body>
<?php
require_once("mainmenu.php");
$mm = new MainMenu();
echo $mm;
?>
<main style="width: 900px;">
<a href="#ti7400">7400</a>, <a href="#ti7402">7402</a>, <a href="#ti7404">7404</a>,
<a href="#ti7408">7408</a>, <a href="#ti7432">7432</a>,
<a href="#ti7448">7448</a>, <a href="#ti7483">7483</a>, <a href="#ti74133">74133</a>,
<a href="#ti74138">74138</a>, <a href="#ti74139">74139</a>,
<a href="#ti74150">74150</a>, <a href="#ti74151">74151</a>, <a href="#ti74153">74153</a>,
<a href="#ti74157">74157</a>, <a href="#ti74161">74161</a>,
<a href="#ti74174">74174</a>, <a href="#ti74181">74181</a>,
<a href="#ti74240">74240</a>, <a href="#ti74244">74244</a>,
<a href="#ti74273">74273</a>, <a href="#ti74377">74377</a>, <a href="#ti744040">744040</a>
<h1>TI 74xx series</h1>
<h2 id="ti7400">7400</h2>
<p>quad 2-input NAND gate</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti7400 <b>is</b>
    <b>port</b> (a, b: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(3 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> testvhd <b>is</b>
<b>begin</b>
    q &lt;= <b>not</b> (a <b>and</b> b);
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti7400(<b>input</b> [3:0] a, b, <b>output</b> [3:0] q);
<b>assign</b> q = ~(a &amp; b);
<b>endmodule</b>
</code>
<svg width="400" height="400">
<defs>
    <path id="pin" d="M0,0 H40 L45,5 L40,10 H0Z"/>
    <path id="p1" d="M0,0 H50 C62.5,0 75,12.5 75,25 C75,37.5 62.5,50 50,50H0Z"/>
    <g id="g1">
        <use x="0" y="5" href="#pin"/>
        <use x="0" y="35" href="#pin"/>
        <line x1="45" y1="10" x2="95" y2="10"/>
        <line x1="45" y1="40" x2="95" y2="40"/>
        <use x="95" y="0" href="#p1"/>
    </g>
    <g id="g2">
        <use x="0" y="0" href="#g1"/>
        <circle cx="175" cy="25" r="5"/>
        <line x1="180" y1="25" x2="230" y2="25"/>
        <use x="230" y="20" href="#pin"/>
    </g>
</defs>
<text x="5" y="25">a[0]</text>
<text x="5" y="55">b[0]</text>
<use class="klas1" x="45" y="10" href="#g2"/>
<text x="325" y="40">q[0]</text>
<text x="5" y="115">a[1]</text>
<text x="5" y="145">b[1]</text>
<use class="klas1" x="45" y="100" href="#g2"/>
<text x="325" y="130">q[1]</text>
<text x="5" y="205">a[2]</text>
<text x="5" y="235">b[2]</text>
<use class="klas1" x="45" y="190" href="#g2"/>
<text x="325" y="220">q[2]</text>
<text x="5" y="295">a[3]</text>
<text x="5" y="325">b[3]</text>
<use class="klas1" x="45" y="280" href="#g2"/>
<text x="325" y="310">q[3]</text>
</svg>

<h2 id="ti7402">7402</h2>
<p>quad 2-input NOR gate</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti7402 <b>is</b>
    <b>port</b> (a, b: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(3 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti7402 <b>is</b>
<b>begin</b>
    q &lt;= <b>not</b> (a or b);
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti7402(<b>input</b> [3:0] a, b, <b>output</b> [3:0] q);
<b>assign</b> q = ~(a | b);
<b>endmodule</b>
</code>
<svg width="400" height="400">
<defs>
    <path id="p5" d="M0,0 C0,0 50,0 50,25 C50,25 50,50 0,50 C0,50 10,50 10,25 C10,25 10,15 0,0"/>
    <g id="g4">
        <use x="0" y="5" href="#pin"/>
        <use x="0" y="35" href="#pin"/>
        <line x1="45" y1="10" x2="105" y2="10"/>
        <line x1="45" y1="40" x2="105" y2="40"/>
        <use x="100" y="0" href="#p5"/>
        <circle cx="155" cy="25" r="5"/>
        <line x1="160" y1="25" x2="210" y2="25"/>
        <use x="210" y="20" href="#pin"/>
    </g>
</defs>
<text x="5" y="20">a[0]</text>
<text x="5" y="50">b[0]</text>
<use class="klas1" x="45" y="5" href="#g4"/>
<text x="310" y="35">q[0]</text>
<text x="5" y="105">a[1]</text>
<text x="5" y="135">b[1]</text>
<use class="klas1" x="45" y="90" href="#g4"/>
<text x="310" y="120">q[1]</text>
<text x="5" y="190">a[2]</text>
<text x="5" y="220">b[2]</text>
<use class="klas1" x="45" y="175" href="#g4"/>
<text x="310" y="205">q[2]</text>
<text x="5" y="275">a[3]</text>
<text x="5" y="305">b[3]</text>
<use class="klas1" x="45" y="260" href="#g4"/>
<text x="310" y="290">q[3]</text>
</svg>

<h2 id="ti7404">7404</h2>
<p>hex inverter gate</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti7404 <b>is</b>
    <b>port</b> (a: <b>in</b> std_logic_vector(5 <b>downto</b> 0);
          q: <b>out</b> std_logic_vector(5 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti7404 <b>is</b>
<b>begin</b>
    q &lt;= <b>not</b> a;
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti7404(<b>input</b> [5:0] a, <b>output</b> [5:0] q);
<b>assign</b> q = ~a;
<b>endmodule</b>
</code>
<svg width="400" height="200">
<defs>
    <g id="g6">
        <use x="0" y="0" href="#pin"/>
        <line x1="45" y1="5" x2="190" y2="5"/>
        <circle cx="195" cy="5" r="5"/>
        <use x="200" y="0" href="#pin"/>
    </g>
</defs>
<use class="klas1" x="50" y="5" href="#g6"/>
<use class="klas1" x="50" y="30" href="#g6"/>
<use class="klas1" x="50" y="55" href="#g6"/>
<use class="klas1" x="50" y="80" href="#g6"/>
<use class="klas1" x="50" y="105" href="#g6"/>
<use class="klas1" x="50" y="130" href="#g6"/>
</svg>

<h2 id="ti7408">7408</h2>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti7408 <b>is</b>
    <b>port</b> (a, b: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(3 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti7408 <b>is</b>
<b>begin</b>
    q &lt;= a <b>and</b> b;
<b>end architecture;</b>
</code>
<code>
<b>module</b> ti7408(<b>input</b> [3:0] a, b, <b>output</b> [3:0] q);
<b>assign</b> q = a &amp; b;
<b>endmodule</b>
</code>
<svg width="400" height="400">
<defs>
    <g id="g3">
        <use x="0" y="0" href="#g1" fill="none"/>
        <line x1="170" y1="25" x2="230" y2="25"/>
        <use x="230" y="20" href="#pin" fill="none"/>
    </g>
</defs>
<text x="5" y="25">a[0]</text>
<text x="5" y="55">b[0]</text>
<use class="klas1" x="45" y="10" href="#g3"/>
<text x="325" y="40">q[0]</text>
<text x="5" y="115">a[1]</text>
<text x="5" y="145">b[1]</text>
<use class="klas1" x="45" y="100" href="#g3"/>
<text x="325" y="130">q[1]</text>
<text x="5" y="205">a[2]</text>
<text x="5" y="235">b[2]</text>
<use class="klas1" x="45" y="190" href="#g3"/>
<text x="325" y="220">q[2]</text>
<text x="5" y="295">a[3]</text>
<text x="5" y="325">b[3]</text>
<use class="klas1" x="45" y="280" href="#g3"/>
<text x="325" y="310">q[3]</text>
</svg>

<h2 id="ti7432">7432</h2>
<p>quad 2-input OR gate</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti7432 <b>is</b>
    <b>port</b> (a, b: <b>in</b> std_logic; q: <b>out</b> std_logic);
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti7432 <b>is</b>
<b>begin</b>
    q &lt;= a <b>or</b> b;
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti7432(<b>input</b> [3:0] a, b, <b>output</b> [3:0] q);
<b>assign</b> q = a | b;
<b>endmodule</b>
</code>

<svg width="400" height="400">
<defs>
    <g id="g5" fill="none" stroke="black">
        <use x="0" y="5" href="#pin"/>
        <use x="0" y="35" href="#pin"/>
        <line x1="45" y1="10" x2="105" y2="10"/>
        <line x1="45" y1="40" x2="105" y2="40"/>
        <use x="100" y="0" href="#p5"/>
        <line x1="150" y1="25" x2="210" y2="25"/>
        <use x="210" y="20" href="#pin"/>
    </g>
</defs>
<text x="5" y="20">a[0]</text>
<text x="5" y="50">b[0]</text>
<use x="45" y="5" href="#g5"/>
<text x="310" y="35">q[0]</text>
<text x="5" y="105">a[1]</text>
<text x="5" y="135">b[1]</text>
<use x="45" y="90" href="#g5"/>
<text x="310" y="120">q[1]</text>
<text x="5" y="190">a[2]</text>
<text x="5" y="220">b[2]</text>
<use x="45" y="175" href="#g5"/>
<text x="310" y="205">q[2]</text>
<text x="5" y="275">a[3]</text>
<text x="5" y="305">b[3]</text>
<use x="45" y="260" href="#g5"/>
<text x="310" y="290">q[3]</text>
</svg>

<h2 id="ti7448">7448</h2>
<p>BCD to 7-segment decoder/driver</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti7448 <b>is</b>
    <b>port</b>(
        dec: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(6 <b>downto</b> 0));
<b>end entity</b> ti7448;

<b>architecture</b> behavior <b>of</b> ti7448 <b>is</b>
<b>begin</b>
    <b>with</b> dec <b>select</b>
        q &lt;= "0111111" <b>when</b> "0000",
             "0000110" <b>when</b> "0001",
             "1011011" <b>when</b> "0010",
             "1001111" <b>when</b> "0011",
             "1100110" <b>when</b> "0100",
             "1101101" <b>when</b> "0101",
             "1111100" <b>when</b> "0110",
             "0000111" <b>when</b> "0111",
             "1111111" <b>when</b> "1000",
             "1100111" <b>when</b> "1001",
             "0000000" <b>when</b> others;
<b>end architecture</b> behavior;
</code>
<code>
<b>module</b> ti7448(<b>input</b> [3:0] dec, <b>output</b> [6:0] q);
<b>assign</b> q = (dec == 4'b0000) ? 8'b0111111 :
           (dec == 4'b0001) ? 8'b0000110 :
           (dec == 4'b0010) ? 8'b1011011 :
           (dec == 4'b0011) ? 8'b1001111 :
           (dec == 4'b0100) ? 8'b1101101 :
           (dec == 4'b0101) ? 8'b1111100 :
           (dec == 4'b0110) ? 8'b0000111 :
           (dec == 4'b0111) ? 8'b0000111 :
           (dec == 4'b1000) ? 8'b1111111 :
           (dec == 4'b1001) ? 8'b1100111 :
                              8'b0000000;
<b>endmodule</b>
</code>
<svg width="400" height="500">
<defs>
    <path id="mux" d="M0,0 L60,20 L60,40 L0,60Z"/>
    <g id="gmux">
        <use x="0" y="0" href="#mux"/>
        <text x="2" y="23">SEL[3..0]</text>
        <text x="2" y="43">DATA[15..0]</text>
    </g>
</defs>
<text x="5" y="32" font-size="8">dec[3..0]</text>
<g class="klas1">
<use x="45" y="25" href="#pin"/>
<line x1="90" y1="30" x2="150" y2="30"/>
<line x1="130" y1="30" x2="130" y2="450"/>
<text x="75" y="52">0x037c</text>
<line x1="110" y1="50" x2="150" y2="50"/>
<use x="150" y="10" href="#gmux"/>
<line x1="210" y1="40" x2="260" y2="40"/>
<use x="260" y="35" href="#pin"/>
<text x="310" y="43">q[0]</text>

<line x1="130" y1="100" x2="150" y2="100"/>
<text x="75" y="122">0x0371</text>
<line x1="110" y1="120" x2="150" y2="120"/>
<use x="150" y="80" href="#gmux"/>
<line x1="210" y1="110" x2="260" y2="110"/>
<use x="260" y="105" href="#pin"/>
<text x="310" y="113">q[1]</text>

<line x1="130" y1="170" x2="150" y2="170"/>
<text x="75" y="192">0x0145</text>
<line x1="110" y1="190" x2="150" y2="190"/>
<use x="150" y="150" href="#gmux"/>
<line x1="210" y1="180" x2="260" y2="180"/>
<use x="260" y="175" href="#pin"/>
<text x="310" y="183">q[2]</text>

<line x1="130" y1="240" x2="150" y2="240"/>
<text x="75" y="262">0x016d</text>
<line x1="110" y1="260" x2="150" y2="260"/>
<use x="150" y="220" href="#gmux"/>
<line x1="210" y1="250" x2="260" y2="250"/>
<use x="260" y="245" href="#pin"/>
<text x="310" y="253">q[3]</text>

<line x1="130" y1="310" x2="150" y2="310"/>
<text x="75" y="332">0x03fb</text>
<line x1="110" y1="330" x2="150" y2="330"/>
<use x="150" y="290" href="#gmux"/>
<line x1="210" y1="320" x2="260" y2="320"/>
<use x="260" y="315" href="#pin"/>
<text x="310" y="323">q[4]</text>

<line x1="130" y1="380" x2="150" y2="380"/>
<text x="75" y="402">0x039f</text>
<line x1="110" y1="400" x2="150" y2="400"/>
<use x="150" y="360" href="#gmux"/>
<line x1="210" y1="390" x2="260" y2="390"/>
<use x="260" y="385" href="#pin"/>
<text x="310" y="393">q[5]</text>

<line x1="130" y1="450" x2="150" y2="450"/>
<text x="75" y="462">0x03ad</text>
<line x1="110" y1="460" x2="150" y2="460"/>
<use x="150" y="430" href="#gmux"/>
<line x1="210" y1="460" x2="260" y2="460"/>
<use x="260" y="455" href="#pin"/>
<text x="310" y="463">q[6]</text>
</g>
</svg>

<h2 id="ti7483">7483</h2>
<p>4-bit binary full adder</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;
<b>use</b> ieee.numeric_std.<b>all</b>;

<b>entity</b> ti7483 <b>is</b>
    <b>port</b>(
        c0: <b>in</b> std_logic;
        c4: <b>out</b> std_logic;
        a, b: <b>in</b> unsigned(3 <b>downto</b> 0);
        s: <b>out</b> unsigned(3 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti7483 <b>is</b>
<b>signal</b> tmp: unsigned(5 <b>downto</b> 0);
<b>begin</b>
    tmp &lt;= ('0' &amp; a &amp; '1') + ('0' &amp; b &amp; c0);
    s &lt;= tmp(4 <b>downto</b> 1);
    c4 &lt;= tmp(5);
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti7483(<b>input</b> c0, <b>input</b> [3:0] a, b, <b>output</b> [3:0] s, <b>output</b> c4);
<b>wire</b> bogus;
<b>assign</b> {c4, s, bogus} = {1'b0, a, 1'b1} + {1'b0, b, c0};
<b>endmodule</b>
</code>
<svg width="400" height="200">
<defs>
<g id="adder">
<circle cx="40" cy="40" r="40"/>
</g>
</defs>
<use x="0" y="0" href="#adder"/>
</svg>
<img src="ti74xx/ti7483.svg" alt="ti7483" width="300px"/>

<h2 id="ti74133">74133</h2>
<p>single 13-input NAND gate</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74133 <b>is</b>
    <b>port</b> (a: <b>in</b> std_logic_vector(12 <b>downto</b> 0); y: <b>out</b> std_logic);
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti74133 <b>is</b>
<b>begin</b>
    y &lt;= <b>not</b> (a(0) <b>and</b> a(1) <b>and</b> a(2) <b>and</b> a(3) <b>and</b> a(4) <b>and</b> a(5) <b>and</b> a(6) <b>and</b> a(7) <b>and</b>
            a(8) <b>and</b> a(9) <b>and</b> a(10) <b>and</b> a(11) <b>and</b> a(12));
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74133(<b>input</b> [12:0] a, <b>output</b> q);
<b>assign</b> q = ~(a[0] &amp; a[1] &amp; a[2] &amp; a[3] &amp; a[4] &amp; a[5] &amp; a[6] &amp; a[7] &amp; a[8] &amp; a[9] &amp; a[10] &amp; a[11] &amp; a[12]);
<b>endmodule</b>
</code>
<img src="ti74xx/ti74133.svg" alt="ti74133"/>

<h2 id="ti74138">74138</h2>
<p>3-line to 8-line decoder/demultiplexer, inverting outputs</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74138 <b>is</b>
    <b>port</b> (en1, en2a_n, en2b_n: <b>in</b> std_logic;
        sel: <b>in</b> std_logic_vector(2 <b>downto</b> 0);
        y: <b>out</b> std_logic_vector(7 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti74138 <b>is</b>
<b>signal</b> en: std_logic;
<b>signal</b> demux: std_logic_vector(7 <b>downto</b> 0);
<b>begin</b>
    en &lt;= en1 <b>and</b> (<b>not</b> en2a_n) <b>and</b> (<b>not</b> en2b_n);
    y &lt;= demux <b>when</b> en='1' <b>else</b> "11111111";

    <b>with</b> sel <b>select</b>
        demux &lt;= "11111110" <b>when</b> "000",
                 "11111101" <b>when</b> "001",
                 "11111011" <b>when</b> "010",
                 "11110111" <b>when</b> "011",
                 "11101111" <b>when</b> "100",
                 "11011111" <b>when</b> "101",
                 "10111111" <b>when</b> "110",
                 "01111111" <b>when</b> "111";
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74138(<b>input</b> en1, en2a_n, en2b_n, <b>input</b> [2:0] sel, <b>output</b> [7:0] y);
<b>reg</b> [7:0] demux;
<b>assign</b> en = en1 &amp; !en2a_n &amp; !en2b_n;
<b>assign</b> y = en ? demux : 8'b11111111;

<b>always</b> @(sel)
<b>begin</b>
    <b>case</b> (sel)
    3'b000:
        demux = 8'b11111110;
    3'b001:
        demux = 8'b11111101;
    3'b010:
        demux = 8'b11111011;
    3'b011:
        demux = 8'b11110111;
    3'b100:
        demux = 8'b11101111;
    3'b101:
        demux = 8'b11011111;
    3'b110:
        demux = 8'b10111111;
    3'b111:
        demux = 8'b01111111;
    <b>endcase</b>
<b>end</b>
<b>endmodule</b>
</code>
<img src="ti74xx/ti74138.svg" alt="ti74138"/>

<h2 id="ti74139">74139</h2>
<p>dual 2 to 4-line decoder/demultiplexer, inverting outputs</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74139 <b>is</b>
    <b>port</b>(
        en0_n, en1_n: <b>in</b> std_logic;
        sel0, sel1: <b>in</b> std_logic_vector(1 <b>downto</b> 0);
        y0, y1: <b>out</b> std_logic_vector(3 <b>downto</b> 0));
<b>end entity</b> ti74139;

<b>architecture</b> behavior <b>of</b> ti74139 <b>is</b>
<b>signal</b> tmp0, tmp1: std_logic_vector(3 <b>downto</b> 0);
<b>begin</b>
    y0 &lt;= "1111" <b>when</b> en0_n='1' <b>else</b> tmp0;
    y1 &lt;= "1111" <b>when</b> en1_n='1' <b>else</b> tmp1;

    <b>with</b> sel0 <b>select</b>
        tmp0 &lt;= "1110" <b>when</b> "00",
                "1101" <b>when</b> "01",
                "1011" <b>when</b> "10",
                "0111" <b>when</b> "11";

    <b>with</b> sel1 <b>select</b>
        tmp1 &lt;= "1110" <b>when</b> "00",
                "1101" <b>when</b> "01",
                "1011" <b>when</b> "10",
                "0111" <b>when</b> "11";

<b>end architecture</b> behavior;
</code>

<img src="ti74xx/ti74139.svg" alt="ti74139" width="400px"/>

<h2 id="ti74150">74150</h2>
<p>16-line to 1-line data selector/multiplexer</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74150 <b>is</b>
    <b>port</b> (
        g_n: <b>in</b> std_logic;
        e: <b>in</b> std_logic_vector(15 <b>downto</b> 0);
        sel: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        w_n: <b>out</b> std_logic);
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti74150 <b>is</b>
<b>signal</b> tmp: std_logic;
<b>begin</b>
    <b>with</b> sel <b>select</b>
        tmp &lt;= e(0) <b>when</b> "0000",
               e(1) <b>when</b> "0001",
               e(2) <b>when</b> "0010",
               e(3) <b>when</b> "0011",
               e(4) <b>when</b> "0100",
               e(5) <b>when</b> "0101",
               e(6) <b>when</b> "0110",
               e(7) <b>when</b> "0111",
               e(8) <b>when</b> "1000",
               e(9) <b>when</b> "1001",
               e(10) <b>when</b> "1010",
               e(11) <b>when</b> "1011",
               e(12) <b>when</b> "1100",
               e(13) <b>when</b> "1101",
               e(14) <b>when</b> "1110",
               e(15) <b>when</b> "1111";

    w_n &lt;= g_n <b>or not</b> tmp;
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74150(<b>input</b> g_n, <b>input</b> [15:0] e, <b>input</b> [3:0] sel, <b>output</b> w_n);
<b>assign</b> w_n = g_n | ~e[sel];
<b>endmodule</b>
</code>
<img src="ti74xx/ti74150.svg" alt="ti74150" width="400px"/>

<h2 id="ti74151">74151</h2>
<p>8-line to 1-line data selector/multiplexer</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74151 <b>is</b>
    <b>port</b> (g_n: <b>in</b> std_logic;
        e: <b>in</b> std_logic_vector(7 <b>downto</b> 0);
        sel: <b>in</b> std_logic_vector(2 <b>downto</b> 0);
        w_n, y: <b>out</b> std_logic);
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti74151 <b>is</b>
<b>signal</b> tmp, tmp2: std_logic;
<b>begin</b>
    <b>with</b> sel <b>select</b>
        tmp &lt;= e(0) <b>when</b> "000",
               e(1) <b>when</b> "001",
               e(2) <b>when</b> "010",
               e(3) <b>when</b> "011",
               e(4) <b>when</b> "100",
               e(5) <b>when</b> "101",
               e(6) <b>when</b> "110",
               e(7) <b>when</b> "111";

    tmp2 &lt;= g_n <b>or not</b> tmp;
    w_n &lt;= tmp2;
    y &lt;= <b>not</b> tmp2;
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74141(<b>input</b> g_n, <b>input</b> [7:0] e, <b>input</b> [2:0] sel, <b>output</b> w_n, y);
<b>assign</b> tmp = g_n | ~e[sel];
<b>assign</b> w_n = tmp;
<b>assign</b> y = ~tmp;
<b>endmodule</b>
</code>
<img src="ti74xx/ti74151.svg" alt="ti74151" width="400px"/>

<h2 id="ti74153">74153</h2>
<p>dual 4-line to 1-line data selector/multiplexer, non-inverting outputs</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74153 is
    port (sel: in std_logic_vector(1 downto 0);
        g0_n, g1_n: in std_logic;
        c0, c1: in std_logic_vector(3 downto 0);
        y0, y1: out std_logic);
end entity;

architecture behavior of ti74153 is
signal mux0_out, mux1_out: std_logic;
begin
    y0 &lt;= not g0_n and mux0_out;
    y1 &lt;= not g1_n and mux1_out;

    process (sel, c0, c1)
    begin
        case sel is
            when "00" =&gt;
                mux0_out &lt;= c0(0);
                mux1_out &lt;= c1(0);
            when "01" =&gt;
                mux0_out &lt;= c0(1);
                mux1_out &lt;= c1(1);
            when "10" =&gt;
                mux0_out &lt;= c0(2);
                mux1_out &lt;= c1(2);
            when "11" =&gt;
                mux0_out &lt;= c0(3);
                mux1_out &lt;= c1(3);
        end case;
    end process;
end architecture;
</code>
<img src="ti74xx/ti74153.svg" alt="ti74153" width="400px"/>

<h2 id="ti74157">74157</h2>
<p>quad 2-line to 1-line data selector/multiplexer, non-inverting outputs</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74157 is
    port (sel, g_n: in std_logic;
        c0, c1, c2, c3: in std_logic_vector(1 downto 0);
        y0, y1, y2, y3: out std_logic);
end entity;

architecture behavior of ti74157 is
signal mux0, mux1, mux2, mux3: std_logic;
begin
    y0 &lt;= not g_n and mux0;
    y1 &lt;= not g_n and mux1;
    y2 &lt;= not g_n and mux2;
    y3 &lt;= not g_n and mux3;

    process (sel, c0, c1)
    begin
        case sel is
            when '0' =&gt;
                mux0 &lt;= c0(0);
                mux1 &lt;= c1(0);
                mux2 &lt;= c2(0);
                mux3 &lt;= c3(0);
            when '1' =&gt;
                mux0 &lt;= c0(1);
                mux1 &lt;= c1(1);
                mux2 &lt;= c2(1);
                mux3 &lt;= c3(1);
        end case;
    end process;
end architecture;
</code>
<img src="ti74xx/ti74157.svg" alt="ti74157" width="400px"/>

<h2 id="ti74161">74161</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ti74161 is
    port (
        clk, rst_n, load_n, enp, ent: in std_logic;
        a: in std_logic_vector(3 downto 0);
        q: out std_logic_vector(3 downto 0);
        c4: out std_logic
    );
end entity;

architecture behavior of ti74161 is
signal cnt: unsigned(3 downto 0);
begin
    c4 &lt;= '1' when cnt="1111" and ent='1' else '0';
    q &lt;= std_logic_vector(cnt);

    process (clk, rst_n) begin
        if rst_n='0' then
            cnt &lt;= (others =&gt; '0');
        elsif rising_edge(clk) then
            if load_n='1' then
                if ent='1' and enp='1' then
                    cnt &lt;= cnt + 1;
                end if;
            else
                cnt &lt;= unsigned(a);
            end if;
        end if;
    end process;
end architecture behavior;
</code>
<img src="ti74xx/ti74161.svg" alt="ti74161" width="800px"/>

<h2 id="ti74174">74174</h2>
<p>hex D flip-flop, common asynchronous clear</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74174 is
    port (clk, rst_n: in std_logic;
        d: in std_logic_vector(5 downto 0);
        q: out std_logic_vector(5 downto 0));
end entity;

architecture behavior of ti74174 is
begin
    process (clk) is
    begin
        if rst_n='0' then
            q &lt;= (others =&gt; '0');
        elsif rising_edge(clk) then
            q &lt;= d;
        end if;
    end process;
end architecture;
</code>
<code>
<b>module</b> ti74174(<b>input</b> clk, <b>input</b> rst_n, <b>input</b> [5:0] d, <b>output reg</b> [5:0] q);

<b>always</b> @(<b>posedge</b> clk, <b>negedge</b> rst_n)
    <b>if</b> (!rst_n)
        q &lt;= 0;
    <b>else</b>
        q &lt;= d;

<b>endmodule</b>
</code>
<svg width="400" height="200">
<defs>
<g id="dflop">
<rect x="0" y="0" width="50" height="70"/>
<text x="2" y="20">D</text>
<text x="38" y="20">Q</text>
<path d="M0,20 L10,30 L0,40"/>
<text x="2" y="50">EN</text>
<text x="17" y="65">RST</text>
</g>
</defs>
<g class="klas1">
<text x="5" y="28">d[5..0]</text>
<use x="45" y="20" href="#pin"/>
<line x1="90" y1="25" x2="150" y2="25"/>
<text x="15" y="43">clk</text>
<use x="45" y="35" href="#pin"/>
<line x1="90" y1="40" x2="150" y2="40"/>
<text x="5" y="108">rst_n</text>
<use x="45" y="100" href="#pin"/>
<line x1="90" y1="105" x2="175" y2="105"/>
<line x1="175" y1="90" x2="175" y2="105"/>
<circle cx="175" cy="85" r="5"/>
<use x="150" y="10" href="#dflop"/>
<line x1="200" y1="25" x2="250" y2="25"/>
<use x="250" y="20" href="#pin"/>
<text x="300" y="28">q[5..0]</text>
</g>
</svg>

<h2 id="ti74181">74181</h2>
<p>4-bit arithmetic logic unit and function generator</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74181 is
    port(
        sel, a, b: in std_logic_vector(3 downto 0);
        m, c0: in std_logic;
        f: out std_logic_vector(3 downto 0);
        c4, g, p, eq: out std_logic);
end entity ti74181;

architecture behavior of ti74181 is
signal t0, t1, t2, t3, t4, t5, t6, t7: std_logic;
signal tmp_g, tmp_f3, tmp_f2, tmp_f1, tmp_f0, tmp2, tmp3, tmp4, tmp5: std_logic;
signal tmp6, tmp7: std_logic;
begin
    t0 &lt;= not (a(0) or (b(0) and sel(0)) or (sel(1) and not b(0)));
    t2 &lt;= not (a(1) or (b(1) and sel(0)) or (sel(1) and not b(1)));
    t4 &lt;= not (a(2) or (b(2) and sel(0)) or (sel(1) and not b(2)));
    t6 &lt;= not (a(3) or (b(3) and sel(0)) or (sel(1) and not b(3)));
    t1 &lt;= not ((not b(0) and sel(2) and a(0)) or (a(0) and b(0) and sel(3)));
    t3 &lt;= not ((not b(1) and sel(2) and a(1)) or (a(1) and b(1) and sel(3)));
    t5 &lt;= not ((not b(2) and sel(2) and a(2)) or (a(2) and b(2) and sel(3)));
    t7 &lt;= not ((not b(3) and sel(2) and a(3)) or (a(3) and b(3) and sel(3)));
    tmp2 &lt;= t7 and t5 and t3 and t0;
    tmp_g &lt;= t6 or (t7 and t4) or (t7 and t5 and t2) or tmp2;
    g &lt;= not tmp_g;
    c4 &lt;= (c0 and t1 and t3 and t5 and t7) or tmp_g;
    p &lt;= not (t1 and t3 and t5 and t7);
    tmp3 &lt;= m and c0 and t1 and t3 and t5;
    tmp4 &lt;= (m and t4) or (m and t2 and t5) or (m and t0 and t3 and t5) or tmp3;
    tmp_f3 &lt;= not tmp4 xor (not t6 and t7);
    tmp5 &lt;= m and c0 and t1 and t3 and t5;
    tmp6 &lt;= (m and t0 and t3 and t5) or (m and t2 and t5) or (m and t4) or tmp5;
    tmp_f2 &lt;= not tmp6 xor (not t4 and t5);
    tmp7 &lt;= (m and t0) or (m and c0);
    tmp_f1 &lt;= not tmp7 xor (not t2 and t4);
    tmp_f0 &lt;= not (c0 and m) xor (not t0 and t1);
    eq &lt;= tmp_f3 and tmp_f2 and tmp_f1 and tmp_f0;
    f(3) &lt;= tmp_f3;
    f(2) &lt;= tmp_f2;
    f(1) &lt;= tmp_f1;
    f(0) &lt;= tmp_f0;
end architecture behavior;
</code>

<h2 id="ti74240">74240</h2>
<p>octal buffer, inverting outputs</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.all;

<b>entity</b> ti74240 <b>is</b>
    <b>port</b> (ena_n, enb_n: <b>in</b> std_logic;
        a, b: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        qa, qb: <b>out</b> std_logic_vector(3 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti74240 <b>is</b>
<b>begin</b>
    qa &lt;= <b>not</b> a <b>when</b> ena_n='0' <b>else</b> (<b>others</b> =&gt; '0');
    qb &lt;= <b>not</b> b <b>when</b> enb_n='0' <b>else</b> (<b>others</b> =&gt; '0');
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74240(<b>input</b> ena_n, enb_n, <b>input</b> [3:0] a, b, <b>output</b> [3:0] qa, qb);
<b>assign</b> qa = ena_n ? 0 : ~a;
<b>assign</b> qb = enb_n ? 0 : ~b;
<b>endmodule</b>
</code>
<img src="ti74xx/ti74240.svg" alt="ti74240" width="300px"/>

<h2 id="ti74244">74244</h2>
<p>octal buffer, non-inverting outputs</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74244 <b>is</b>
    <b>port</b> (ena_n, enb_n: <b>in</b> std_logic;
        a, b: <b>in</b> std_logic_vector(3 <b>downto</b> 0);
        qa, qb: <b>out</b> std_logic_vector(3 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior of ti74244 is
<b>begin</b>
    qa &lt;= a <b>when</b> ena_n='0' <b>else</b> (<b>others</b> =&gt; '0');
    qb &lt;= b <b>when</b> enb_n='0' <b>else</b> (<b>others</b> =&gt; '0');
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74244(<b>input</b> ena_n, enb_n, <b>input</b> [3:0] a, b, <b>output</b> [3:0] qa, qb);
<b>assign</b> qa = ena_n ? 0 : a;
<b>assign</b> qb = enb_n ? 0 : b;
<b>endmodule</b>
</code>
<img src="ti74xx/ti74244.svg" alt="ti74244" width="300px"/>

<h2 id="ti74273">74273</h2>
<p>8-bit register, asynchronous clear</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74273 <b>is</b>
    <b>port</b> (
        clk, rst_n: <b>in</b> std_logic;
        d: <b>in</b> std_logic_vector(7 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(7 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> ti74273 <b>is</b>
<b>begin</b>
    <b>process</b> (clk,rst_n,d)
    <b>begin</b>
        <b>if</b> rst_n='0' <b>then</b>
            q &lt;= (others =&gt; '0');
        <b>elsif</b> rising_edge(clk) <b>then</b>
            q &lt;= d;
        <b>end if</b>;
    <b>end process</b>;
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74273(<b>input</b> clk, rst_n, <b>input</b> [7:0] d, <b>output reg</b> [7:0] q);

<b>always</b> @(<b>posedge</b> clk, <b>negedge</b> rst_n)
    <b>if</b> (!rst_n)
        q &lt;= 0;
    <b>else</b>
        q &lt;= d;
<b>endmodule</b>
</code>
<svg width="400" height="150">
<g class="klas1">
<text x="5" y="28">d[7..0]</text>
<use x="45" y="20" href="#pin"/>
<line x1="90" y1="25" x2="150" y2="25"/>
<text x="15" y="43">clk</text>
<use x="45" y="35" href="#pin"/>
<line x1="90" y1="40" x2="150" y2="40"/>
<text x="5" y="108">rst_n</text>
<use x="45" y="100" href="#pin"/>
<line x1="90" y1="105" x2="175" y2="105"/>
<line x1="175" y1="90" x2="175" y2="105"/>
<circle cx="175" cy="85" r="5"/>
<use x="150" y="10" href="#dflop"/>
<line x1="200" y1="25" x2="250" y2="25"/>
<use x="250" y="20" href="#pin"/>
<text x="300" y="28">q[7..0]</text>
</g>
</svg>

<h2 id="ti74377">74377</h2>
<p>8-bit register, clock enable</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> ti74377 <b>is</b>
    <b>port</b>(en_n, clk: <b>in</b> std_logic;
        d: <b>in</b> std_logic_vector(7 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(7 <b>downto</b> 0));
<b>end entity</b> ti74377;

<b>architecture</b> behavior <b>of</b> ti74377 <b>is</b>
<b>begin</b>
    <b>process</b> (clk)
    <b>begin</b>
        <b>if</b> rising_edge(clk) <b>and</b> en_n='0' <b>then</b>
            q &lt;= d;
        <b>end if</b>;
    <b>end process</b>;
<b>end architecture</b> behavior;
</code>
<code>
<b>module</b> ti74377(<b>input</b> clk, en_n,
    <b>input</b> [7:0] d, <b>output reg</b> [7:0] q);

<b>always</b> @(<b>posedge</b> clk)
    <b>if</b> (!en_n)
        q &lt;= d;
<b>endmodule</b>
</code>
<svg width="400" height="100">
<g class="klas1">
<text x="5" y="28">d[7..0]</text>
<use x="45" y="20" href="#pin"/>
<line x1="90" y1="25" x2="150" y2="25"/>
<text x="15" y="43">clk</text>
<use x="45" y="35" href="#pin"/>
<line x1="90" y1="40" x2="150" y2="40"/>
<text x="5" y="58">en_n</text>
<use x="45" y="50" href="#pin"/>
<line x1="90" y1="55" x2="140" y2="55"/>
<circle cx="145" cy="55" r="5"/>
<use x="150" y="10" href="#dflop"/>
<line x1="200" y1="25" x2="250" y2="25"/>
<use x="250" y="20" href="#pin"/>
<text x="300" y="28">q[7..0]</text>
</g>
</svg>

<h2 id="ti74557">74557</h2>
<p>8-bit by 8-bit multiplier</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;
<b>use</b> ieee.numeric_std.<b>all</b>;

<b>entity</b> ti74557 <b>is</b>
    <b>port</b> (a, b: <b>in</b> std_logic_vector(7 <b>downto</b> 0);
        q: <b>out</b> std_logic_vector(15 <b>downto</b> 0));
<b>end entity</b>;

<b>architecture</b> behavior of ti74557 is
<b>signal</b> ua, ub: unsigned(7 downto 0);
<b>signal</b> uq: unsigned(15 downto 0);
<b>begin</b>
    ua &lt;= unsigned(a);
    ub &lt;= unsigned(b);
    uq &lt;= ua * ub;
    q &lt;= std_logic_vector(uq);
<b>end architecture</b>;
</code>
<code>
<b>module</b> ti74557(<b>input</b> [7:0] a, b, <b>output</b> [15:0] q);
<b>assign</b> q = a * b;
<b>endmodule</b>
</code>
<img src="ti74xx/ti74557.svg" alt="ti74557" width="300px"/>

<h2 id="ti74595">74595</h2>
<p>8-bit shift registers, output latches, parallel outputs</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74595 is
    port(rck, srck, g_n, srclr_n, ser: in std_logic;
        q: out std_logic_vector(7 downto 0));
end entity;

architecture behavior of ti74595 is
signal xq: std_logic_vector(7 downto 0);
begin
    q &lt;= xq;
    process (srck)
    begin
        if srclr_n='0' then
            xq &lt;= (others =&gt; '0');
        elsif rising_edge(srck) then
            xq(7 downto 1) &lt;= xq(6 downto 0);
            xq(0) &lt;= ser;
        end if;
    end process;
end architecture;
</code>

<h2 id="ti744040">744040</h2>
<p>12-stage binary ripple counter</p>
<code>
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ti744040 is
    port (clk, clr: in std_logic; q: out unsigned(11 downto 0));
end entity;

architecture behavior of ti744040 is
signal tmp: unsigned(11 downto 0);
begin
    q &lt;= tmp;
    process (clk, clr) begin
        if clr='1' then
            tmp &lt;= (others =&gt; '0');
        elsif falling_edge(clk) then
            tmp &lt;= tmp + 1;
        end if;
    end process;
end architecture;
</code>
<code>
<b>module</b> ti744040(<b>input</b> clk_n, rst, <b>output reg</b> [11:0] q);
<b>always</b> @(<b>negedge</b> clk_n, <b>posedge</b> rst)
    <b>if</b> (rst)
        q = 0;
    <b>else</b>
        q = q + 1;
<b>endmodule</b>
</code>
<img src="ti74xx/ti744040.svg" alt="ti744040" width="400px"/>

</main>
</body>
</html>


