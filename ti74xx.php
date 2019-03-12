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
<a href="#ti7400">7400</a>, <a href="#ti7402">7402</a>, <a href="#ti7404">7404</a>,
<a href="#ti7408">7408</a>, <a href="#ti7432">7432</a>,
<a href="#ti7448">7448</a>, <a href="#ti7483">7483</a>, <a href="#ti74133">74133</a>,
<a href="#ti74138">74138</a>, <a href="#ti74139">74139</a>,
<a href="#ti74150">74150</a>, <a href="#ti74151">74151</a>, <a href="#ti74153">74153</a>,
<a href="#ti74157">74157</a>, <a href="#ti74161">74161</a>,
<a href="#ti74174">74174</a>, <a href="#ti74181">74181</a>,
<a href="#ti74240">74240</a>, <a href="#ti72244">74244</a>,
<a href="#ti74273">74273</a>, <a href="#ti74377">74377</a>, <a href="#ti744040">744040</a>
<h1>TI 74xx series</h1>
<h2 id="ti7400">7400</h2>
<p>quad 2-input NAND gate</p>
<code>
<b>library</b> ieee;
<b>use</b> ieee.std_logic_1164.<b>all</b>;

<b>entity</b> x7400 <b>is</b>
    <b>port</b> (a1, b1, a2, b2, a3, b3, a4, b4: <b>in</b> std_logic; y1, y2, y3, y4: <b>out</b> std_logic);
<b>end entity</b>;

<b>architecture</b> behavior <b>of</b> x7400 <b>is</b>
<b>begin</b>
    y1 &lt;= <b>not</b> (a1 <b>and</b> b1);
    y2 &lt;= <b>not</b> (a2 <b>and</b> b2);
    y3 &lt;= <b>not</b> (a3 <b>and</b> b3);
    y4 &lt;= <b>not</b> (a4 <b>and</b> b4);
<b>end architecture</b>;
</code>
<img src="ti74xx/ti7400.svg" alt="ti700" width="200px"/>

<h2 id="ti7402">7402</h2>
<p>quad 2-input NOR gate</p>
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

<img src="ti74xx/ti7402.svg" alt="ti7402" width="200px"/>

<h2 id="ti7404">7404</h2>
<p>hex inverter gate</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti7404 is
    port (a: in std_logic_vector(5 downto 0);
          q: out std_logic_vector(5 downto 0));
end entity;

architecture behavior of ti7404 is
begin
    q &lt;= not a;
end architecture;
</code>
<img src="ti74xx/ti7404.svg" alt="ti7404" width="200px"/>

<h2 id="ti7408">7408</h2>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti7408 is
    port (a, b: in std_logic_vector(3 downto 0);
        q: out std_logic_vector(3 downto 0));
end entity;

architecture behavior of ti7408 is
begin
    q &lt;= a and b;
end architecture;
</code>

<h2 id="ti7432">7432</h2>
<p>quad 2-input OR gate</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti7432 is
    port (a1, b1, a2, b2, a3, b3, a4, b4: in std_logic; y1, y2, y3, y4: out std_logic);
end entity;

architecture behavior of ti7432 is
begin
    y1 &lt;= a1 or b1;
    y2 &lt;= a2 or b2;
    y3 &lt;= a3 or b3;
    y4 &lt;= a4 or b4;
end architecture;
</code>

<img src="ti74xx/ti7432.svg" alt="ti7432" width="200px"/>

<h2 id="ti7448">7448</h2>
<p>BCD to 7-segment decoder/driver</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti7448 is
    port(
        dec: in std_logic_vector(3 downto 0);
        q: out std_logic_vector(6 downto 0));
end entity ti7448;

architecture behavior of ti7448 is
begin
    with dec select
        q &lt;= "0111111" when "0000",
             "0000110" when "0001",
             "1011011" when "0010",
             "1001111" when "0011",
             "1100110" when "0100",
             "1101101" when "0101",
             "1111100" when "0110",
             "0000111" when "0111",
             "1111111" when "1000",
             "1100111" when "1001",
             "0000000" when others;
end architecture behavior;
</code>
<img src="ti74xx/ti7448.svg" alt="ti7448" width="300px"/>

<h2 id="ti7483">7483</h2>
<p>4-bit binary full adder</p>
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
<img src="ti74xx/ti7483.svg" alt="ti7483" width="300px"/>

<h2 id="ti74133">74133</h2>
<p>single 13-input NAND gate</p>
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
<img src="ti74xx/ti74133.svg" alt="ti74133"/>

<h2 id="ti74138">74138</h2>
<p>3-line to 8-line decoder/demultiplexer, inverting outputs</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74138 is
    port (en1, en2a_n, en2b_n: in std_logic;
        sel: in std_logic_vector(2 downto 0);
        y: out std_logic_vector(7 downto 0));
end entity;

architecture behavior of ti74138 is
signal en: std_logic;
signal demux: std_logic_vector(7 downto 0);
begin
    en &lt;= en1 and (not en2a_n) and (not en2b_n);
    y &lt;= demux when en='1' else "11111111";

    with sel select
        demux &lt;= "11111110" when "000",
                 "11111101" when "001",
                 "11111011" when "010",
                 "11110111" when "011",
                 "11101111" when "100",
                 "11011111" when "101",
                 "10111111" when "110",
                 "01111111" when "111";
end architecture;
</code>
<img src="ti74xx/ti74138.svg" alt="ti74138"/>

<h2 id="ti74139">74139</h2>
<p>dual 2 to 4-line decoder/demultiplexer, inverting outputs</p>
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

<img src="ti74xx/ti74139.svg" alt="ti74139" width="400px"/>

<h2 id="ti74150">74150</h2>
<p>16-line to 1-line data selector/multiplexer</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74150 is
    port (
        g_n: in std_logic;
        e: in std_logic_vector(15 downto 0);
        sel: in std_logic_vector(3 downto 0);
        w_n: out std_logic);
end entity;

architecture behavior of ti74150 is
signal tmp: std_logic;
begin
    with sel select
        tmp &lt;= e(0) when "0000",
               e(1) when "0001",
               e(2) when "0010",
               e(3) when "0011",
               e(4) when "0100",
               e(5) when "0101",
               e(6) when "0110",
               e(7) when "0111",
               e(8) when "1000",
               e(9) when "1001",
               e(10) when "1010",
               e(11) when "1011",
               e(12) when "1100",
               e(13) when "1101",
               e(14) when "1110",
               e(15) when "1111";

    w_n &lt;= g_n or not tmp;
end architecture;
</code>

<img src="ti74xx/ti74150.svg" alt="ti74150" width="400px"/>

<h2 id="ti74151">74151</h2>
<p>8-line to 1-line data selector/multiplexer</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74151 is
    port (g_n: in std_logic;
        e: in std_logic_vector(7 downto 0);
        sel: in std_logic_vector(2 downto 0);
        w_n, y: out std_logic);
end entity;

architecture behavior of ti74151 is
signal tmp, tmp2: std_logic;
begin
    with sel select
        tmp &lt;= e(0) when "000",
               e(1) when "001",
               e(2) when "010",
               e(3) when "011",
               e(4) when "100",
               e(5) when "101",
               e(6) when "110",
               e(7) when "111";

    tmp2 &lt;= g_n or not tmp;
    w_n &lt;= tmp2;
    y &lt;= not tmp2;
end architecture;
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
<img src="ti74xx/ti74174.svg" alt="ti74174" width="300px"/>

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
library ieee;
use ieee.std_logic_1164.all;

entity ti74240 is
    port (ena_n, enb_n: in std_logic;
        a, b: in std_logic_vector(3 downto 0);
        qa, qb: out std_logic_vector(3 downto 0));
end entity;

architecture behavior of ti74240 is
begin
    qa &lt;= not a when ena_n='0' else (others =&gt; '0');
    qb &lt;= not b when enb_n='0' else (others =&gt; '0');
end architecture;
</code>
<img src="ti74xx/ti74240.svg" alt="ti74240" width="300px"/>

<h2 id="ti74244">74244</h2>
<p>octal buffer, non-inverting outputs</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74244 is
    port (ena_n, enb_n: in std_logic;
        a, b: in std_logic_vector(3 downto 0);
        qa, qb: out std_logic_vector(3 downto 0));
end entity;

architecture behavior of ti74244 is
begin
    qa &lt;= a when ena_n='0' else (others =&gt; '0');
    qb &lt;= b when enb_n='0' else (others =&gt; '0');
end architecture;
</code>
<img src="ti74xx/ti74244.svg" alt="ti74244" width="300px"/>

<h2 id="ti74273">74273</h2>
<p>8-bit register, asynchronous clear</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74273 is
    port (
        clk, rst_n: in std_logic;
        d: in std_logic_vector(7 downto 0);
        q: out std_logic_vector(7 downto 0));
end entity;

architecture behavior of ti74273 is
begin
    process (clk,rst_n,d)
    begin
        if rst_n='0' then
            q &lt;= (others =&gt; '0');
        elsif rising_edge(clk) then
            q &lt;= d;
        end if;
    end process;
end architecture;
</code>
<img src="ti74xx/ti74273.svg" alt="ti74273" width="300px"/>

<h2 id="ti74377">74377</h2>
<p>8-bit register, clock enable</p>
<code>
library ieee;
use ieee.std_logic_1164.all;

entity ti74377 is
    port(en_n, clk: in std_logic;
        d: in std_logic_vector(7 downto 0);
        q: out std_logic_vector(7 downto 0));
end entity ti74377;

architecture behavior of ti74377 is
begin
    process (clk)
    begin
        if rising_edge(clk) and en_n='0' then
            q &lt;= d;
        end if;
    end process;
end architecture behavior;
</code>
<img src="ti74xx/ti74377.svg" alt="ti74377" width="300px"/>

<h2 id="ti74557">74557</h2>
<p>8-bit by 8-bit multiplier</p>
<code>
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ti74557 is
    port (a, b: in std_logic_vector(7 downto 0);
        q: out std_logic_vector(15 downto 0));
end entity;

architecture behavior of ti74557 is
signal ua, ub: unsigned(7 downto 0);
signal uq: unsigned(15 downto 0);
begin
    ua &lt;= unsigned(a);
    ub &lt;= unsigned(b);
    uq &lt;= ua * ub;
    q &lt;= std_logic_vector(uq);
end architecture;
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

<img src="ti74xx/ti744040.svg" alt="ti744040" width="400px"/>

</main>
</body>
</html>

