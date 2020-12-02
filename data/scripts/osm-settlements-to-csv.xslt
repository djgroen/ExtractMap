<?xml version="1.0"?>
<!-- <xsl:stylesheet -->
<!--     xmlns:xsl="http://www.w3.org/1999/XSL/Transform" -->
<!--     xmlns:xs="http://www.w3.org/2001/XMLSchema" -->
<!--     xmlns:str="http://example.com/namespace" -->
<!--     version="2.0"> -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <!--
      Converts OSM xml to CSV
      USAGE:
      xsltproc ~/Downloads/loc_graph_extract/know_pruning/HidGIS/data/cities.xslt south-sudan-cities.osm > data/overpass_allpubs.csv && less data/overpass_allpubs.csv
  -->

  <!-- This "key" declaration speeds up by a million million times, the lookup from ways back to nodes -->
  <xsl:key name="nodeid" match="node" use="@id"/>

  <xsl:variable name="delim" select="','"/>

  <xsl:output method="text"/>
  <xsl:template match="/">

    <!-- Column headers -->
    <xsl:text>osmid</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>lat</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>lon</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>&#35;name</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>type</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>population</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>censusDate</xsl:text>
    <xsl:value-of select="$delim"/>
    <xsl:text>wikipedia&#xa;</xsl:text>

    <!-- Node pubs -->
    <xsl:for-each select="osm/node">
      <xsl:choose>
        <xsl:when test="tag[@k='place'][@v='town'] | tag[@k='place'][@v='city']  | tag[@k='place'][@v='village']">
	  <xsl:value-of select="@id"/>
	  <xsl:value-of select="$delim"/>
	  <xsl:value-of select="@lat"/>
	  <xsl:value-of select="$delim"/>
          <xsl:value-of select="@lon"/>
	  <xsl:value-of select="$delim"/>
	  <xsl:text>"</xsl:text>
	  <xsl:apply-templates select="tag[@k='name']"/>
	  <xsl:text>"</xsl:text>
	  <xsl:value-of select="$delim"/>
	  <xsl:apply-templates select="tag[@k='place']"/>
	  <xsl:value-of select="$delim"/>
	  <xsl:text>"</xsl:text>
	  <xsl:apply-templates select="tag[@k='population']"/>
	  <xsl:text>"</xsl:text>
	  <xsl:value-of select="$delim"/>
	  <xsl:text>"</xsl:text>
	  <xsl:apply-templates select="tag[@k='population:date']"/>
	  <xsl:text>"</xsl:text>
	  <xsl:value-of select="$delim"/>
	  <xsl:text>"</xsl:text>
	  <xsl:apply-templates select="tag[@k='wikipedia']"/>
	  <xsl:text>"</xsl:text>
          <xsl:text>&#xa;</xsl:text>
        </xsl:when>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="tag">
    <xsl:value-of select="@v"/>
  </xsl:template>

  <!-- <xsl:template match="tag[@k='population']"> -->
  <!--   <xsl:copy> -->
  <!--     <xsl:analyze-string select="@v" regex=".*([0-9\,]+).*" flags="x"> -->
  <!-- 	<xsl:matching-substring> -->
  <!-- 	  <xsl:value-of select="replace(regex-group(1),',','')"/> -->
  <!-- 	</xsl:matching-substring> -->
  <!--       <xsl:non-matching-substring> -->
  <!--           <xsl:value-of select="@v"/> -->
  <!--       </xsl:non-matching-substring> -->
  <!--     </xsl:analyze-string> -->
  <!--   </xsl:copy> -->
  <!-- </xsl:template> -->
  
</xsl:stylesheet>

