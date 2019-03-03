<!DOCTYPE html>
  
<?php
echo file_get_contents("achtung.html");
?>

<html>
<head>
<title>Spectrum FPGA</title>
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

code em {
    font-style: normal;
    color: red;
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
<h1>TI 74xx series</h1>
<h2>7400</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity x7400 is
    port (a1, b1, a2, b2, a3, b3, a4, b4: in std_logic; y1, y2, y3, y4: out std_logic);
end entity;

architecture behavior of x7400 is
begin
    y1 &lt;= not (a1 and b1);
    y2 &lt;= not (a2 and b2);
    y3 &lt;= not (a3 and b3);
    y4 &lt;= not (a4 and b4);
end architecture;
</code>

<h2>7402</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti7402 is
    port (a1, b1, a2, b2, a3, b3, a4, b4: in std_logic; y1, y2, y3, y4: out std_logic);
end entity;

architecture behavior of ti7402 is
begin
    y1 &lt;= not (a1 or b1);
    y2 &lt;= not (a2 or b2);
    y3 &lt;= not (a3 or b3);
    y4 &lt;= not (a4 or b4);
end architecture;
</code>

<img src="ti7402.svg" alt="ti7402"/>

<h2>7483</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ti7483 is
    port(
        c0: in std_logic;
        c4: out std_logic;
        a, b: in unsigned(3 downto 0);
        s: out unsigned(3 downto 0));
end entity;

architecture behavior of ti7483 is
signal tmp: unsigned(5 downto 0);
begin
    tmp &lt;= ('0' &amp; a &amp; '1') + ('0' &amp; b &amp; c0);
    s &lt;= tmp(4 downto 1);
    c4 &lt;= tmp(5);
end architecture;
</code>
<img src="ti7483.svg" alt="ti7483"/>

<h2>74133</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74133 is
    port (a: in std_logic_vector(12 downto 0); y: out std_logic);
end entity;

architecture behavior of ti74133 is
begin
    y &lt;= not (a(0) and a(1) and a(2) and a(3) and a(4) and a(5) and a(6) and a(7) and
            a(8) and a(9) and a(10) and a(11) and a(12));
end architecture;
</code>
<img src="ti74133.svg" alt="ti74133"/>

<h2>74139</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74139 is
    port(
        en0_n, en1_n: in std_logic;
        sel0, sel1: in std_logic_vector(1 downto 0);
        y0, y1: out std_logic_vector(3 downto 0));
end entity ti74139;

architecture behavior of ti74139 is
signal tmp0, tmp1: std_logic_vector(3 downto 0);
begin
    y0 &lt;= "1111" when en0_n='1' else tmp0;
    y1 &lt;= "1111" when en1_n='1' else tmp1;

    process (sel0)
    begin
        case sel0 is
        when "00" =&gt; tmp0 &lt;= "1110";
        when "01" =&gt; tmp0 &lt;= "1101";
        when "10" =&gt; tmp0 &lt;= "1011";
        when "11" =&gt; tmp0 &lt;= "0111";
          end case;
    end process;

    process (sel1)
    begin
        case sel1 is
        when "00" =&gt; tmp1 &lt;= "1110";
        when "01" =&gt; tmp1 &lt;= "1101";
        when "10" =&gt; tmp1 &lt;= "1011";
        when "11" =&gt; tmp1 &lt;= "0111";
        end case;
    end process;
end architecture behavior;
</code>

</main>
</body>
</html>


