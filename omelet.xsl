<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xpath-default-namespace="http://www.w3.org/1999/xhtml" version="2.0">
    <xsl:output omit-xml-declaration="yes"/>

    <xsl:template match="/">
        <html>
            <head>
                <title>Omelet</title>
                <link rel="stylesheet" type="text/css" href="common.css"/>
                <link rel="stylesheet" type="text/css" href="omelet.css"/>
            </head>
            <body>
                <nav class="mainmenu">
                    <a href="index.html">Home</a>
                    <a href="https://github.com/jterweeme">GitHub</a>
                    <a href="index.html">TBD</a>
                    <a href="index.html">TBD</a>
                    <a href="index.html">TBD</a>
                </nav>
                <xsl:apply-templates select="diagram"/>
            </body>
        </html>
    </xsl:template>

    <xsl:template match="diagram">
        <figure class="diagram">
            <xsl:apply-templates select="element[type = 'com.umlet.element.Class']"/>
            <xsl:apply-templates select="element[type = 'com.umlet.element.Relation']"/>
        </figure>
    </xsl:template>

    <xsl:template name="klasse" match="element[type = 'com.umlet.element.Class']">
        <ul>
            <xsl:attribute name="style">
                <xsl:apply-templates select="coordinates"/>
            </xsl:attribute>
            <xsl:apply-templates select="panel_attributes"/>
        </ul>
    </xsl:template>

    <xsl:template name="onzin">
        <xsl:param name="inhoud"/>
        <xsl:choose>
            <xsl:when test="starts-with($inhoud, '*')">
                <b>
                    <xsl:value-of select="substring-before(substring-after($inhoud, '*'), '*')"/>
                </b>
            </xsl:when>
            <xsl:when test="starts-with($inhoud, '*')">
                <b><xsl:value-of select="substring-after($inhoud, '*')"/></b>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$inhoud"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="additional_attributes">
        <xsl:text>left: </xsl:text>
        <xsl:value-of select="substring-before(., ';')"/>
        <xsl:text>px; top: </xsl:text>
        <xsl:value-of select="substring-before(substring-after(., ';'), ';')"/>
        <xsl:text>px;</xsl:text>
    </xsl:template>

    <xsl:template name="relation" match="element[type = 'com.umlet.element.Relation']">
        <figure>
            <xsl:attribute name="style">
                <xsl:apply-templates select="coordinates"/>
            </xsl:attribute>
            <img src="diamond.png" alt="">
                <xsl:attribute name="style">
                    <xsl:apply-templates select="additional_attributes"/>
                </xsl:attribute>
            </img>
            <hr/>
        </figure>
    </xsl:template>   

    <xsl:template match="panel_attributes">
        <li>
            <xsl:call-template name="onzin">
                <xsl:with-param name="inhoud" select="substring-before(., '--')"/>
            </xsl:call-template>
        </li>
        <li>
            <xsl:value-of select="substring-before(substring-after(., '--'), '--')"/>
        </li>
        <li>
            <xsl:value-of select="substring-after(substring-after(., '--'), '--')"/>
        </li>
    </xsl:template>

    <xsl:template match="coordinates">
        <xsl:text>width: </xsl:text>
        <xsl:value-of select="w"/>
        <xsl:text>px; left: </xsl:text>
        <xsl:value-of select="x"/>
        <xsl:text>px; top: </xsl:text>
        <xsl:value-of select="y"/>
        <xsl:text>px; height: </xsl:text>
        <xsl:value-of select="h"/>
        <xsl:text>px;</xsl:text>
    </xsl:template>
</xsl:stylesheet>

