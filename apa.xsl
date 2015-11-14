<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output indent="yes"/>

    <xsl:template match="/">
        <xsl:apply-templates select="lib"/>
    </xsl:template>

    <xsl:template match="lib">
        <xsl:apply-templates select="entry"/>
    </xsl:template>

    <xsl:template match="entry">
        <div>
            <xsl:apply-templates select="author"/>
            <xsl:apply-templates select="year"/>
            <xsl:apply-templates select="title"/>
            <xsl:apply-templates select="url"/>
        </div>
    </xsl:template>

    <xsl:template match="author">
        <xsl:value-of select="."/>
    </xsl:template>

    <xsl:template match="year">
        <xsl:text>(</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>)</xsl:text>
    </xsl:template>

    <xsl:template match="title">
        <i><xsl:value-of select="."/></i>
    </xsl:template>

    <xsl:template match="url">
        <xsl:value-of select="."/>
    </xsl:template>
</xsl:stylesheet>

