<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output omit-xml-declaration="yes"/>

    <xsl:template match="/">
        <html>
            <head>
                <title>Omelet</title>
                <link rel="stylesheet" type="text/css" href="common.css"/>
            </head>
            <body><xsl:apply-templates select="diagram"/></body>
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
                <b><xsl:value-of select="substring-after($inhoud, '*')"/></b>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$inhoud"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="relation" match="element[type = 'com.umlet.element.Relation']">
        <figure>
            <xsl:attribute name="style">
                <xsl:apply-templates select="coordinates"/>
            </xsl:attribute>
            <div><hr/></div>
           <!-- <xsl:apply-templates select="panel_attributes"/> -->
        </figure>
    </xsl:template>   

    <xsl:template match="panel_attributes">
        <li>
            <xsl:call-template name="onzin">
                <xsl:with-param name="inhoud" select="substring-before(., '--')"/>
            </xsl:call-template>
        </li>
        <li>
            <xsl:value-of select="substring-after(., '--')"/>
        </li>
        
        <!-- <li><xsl:value-of select="."/></li> -->
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

