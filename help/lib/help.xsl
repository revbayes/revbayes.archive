<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    
    <xsl:output method="html"
                doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" 
                doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"
                indent="yes"/>

    <xsl:template match="/">
        <html>
            <head>
                <title>RevBayes Manual Pages :: <xsl:value-of select="//name" /></title>
                <link rel="stylesheet" type="text/css" href="lib/help.css"/>
            </head>
            <body>
                
                <div class="header">
                    <a href="html/index.html">[Index]</a>
                </div>
                
                <h1>RevBayes Manual Pages :: <xsl:value-of select="//name" /></h1>
                
                <h2>
                    Name
                </h2>
                <p class="indent">
                    <xsl:value-of select="//name" />
                </p>
                
                <h2>
                    Description
                </h2>   
                <xsl:for-each select="//description/p">                        
                    <p class="indent">
                        <xsl:value-of select="." />
                    </p>            
                </xsl:for-each>
                
                
                <h2>
                    Usage
                </h2>
                <p class="indent">
                    <xsl:value-of select="//usage" />   
                </p>
                
                <h2>
                    Details
                </h2>
                <xsl:for-each select="//details/p">                        
                    <p class="indent">
                        <xsl:value-of select="." />
                    </p>            
                </xsl:for-each>
                
                
                <h2>
                    Arguments
                </h2>   
                <xsl:for-each select="//argument">                        
                    <p class="indent">
                        <b>
                            <xsl:value-of select="label" />
                        </b>
                        
                        <div class="indent">
                            <xsl:value-of select="description" /> 
                            <br />
                            Type : <xsl:value-of select="type" /> 
                            <br />
                            Value Type : <xsl:value-of select="valueType" /> 
                            <br />
                            <xsl:if test="option">
                                Options :
                                <div  class="indent">
                                    <xsl:for-each select="option">  
                                        - <xsl:value-of select="." /> 
                                        <br />
                                    </xsl:for-each>  
                                </div>
                            </xsl:if>
                            <xsl:if test="defaultValue">
                                Default Value : <xsl:value-of select="defaultValue" /> 
                                <br />
                            </xsl:if>
                        </div>
                    </p>            
                </xsl:for-each>
                
                <h2>
                    Return Value
                </h2>
                <p class="indent">
                    <xsl:value-of select="//value" />   
                </p>
                
                <h2>
                    Author
                </h2>
                <p class="indent">
                    <xsl:value-of select="//author" />   
                </p>
                
                <h2>
                    References
                </h2>   
                <xsl:for-each select="//reference">                        
                    <p class="indent">                       
                        <b>
                            <xsl:value-of select="info" /> 
                        </b> 
                        <br />                        
                        <div class="indent">
                            <xsl:value-of select="citation" /> 
                            <br />
                            Link : <a href="{url}">
                                <xsl:value-of select="url" />
                            </a>
                            <br />
                            DOI : <xsl:value-of select="doi" /> 
                            <br />                            
                        </div>
                    </p>
                </xsl:for-each>
                
                <h2>
                    See Also
                </h2>   
                <xsl:for-each select="//seeAlso/function">                        
                    <p class="indent">                        
                        <a href="{.}.xml">
                            <xsl:value-of select="." />
                        </a>
                    </p>            
                </xsl:for-each>
                
                <h2>
                    Example
                </h2>
                <p>
                    <pre>
                        <xsl:value-of select="//example" />   
                    </pre>
                </p>
        
                <br />
                <br />
                <br />
                <br />
                <hr />
                <p class="footer">
                    <b>RevBayes</b>
                    <br />
                    <i>
                        An R-like environment for phylogenetic analysis using Bayesian inference. <br />
                        Released under the GPL license, version 3. <br />
                    </i>
                    Learn more on the <a href="http://sourceforge.net/apps/mediawiki/revbayes/index.php?title=RevBayes_MediaWiki" >RevBayes Wiki</a> 
                    <br />
                </p>
            </body>
        </html>
    </xsl:template>

</xsl:stylesheet>