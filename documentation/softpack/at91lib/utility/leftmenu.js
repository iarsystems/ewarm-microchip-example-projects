function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73943\" class=\"headerLeftMenuInActive\"><a id=\"aID73943\" href=\"#\" OnMouseOver=\"link('_dir','utility0',this)\" class=\"leftMenuLinkHeadInActive\">utility</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73944\" class=\"leftMenuInActive\"><a id=\"aID73944\" href=\"#\" OnMouseOver=\"link('_dir','encryption/encryption0',this)\" class=\"leftMenuLinkInActive\">encryption</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73819\" class=\"leftMenuInActive\"><a id=\"aID73819\" href=\"#\" OnMouseOver=\"link('_page','HammingCodeErrors0',this)\" class=\"leftMenuLinkInActive\">Hamming Code Errors</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74249\" class=\"leftMenuInActive\"><a id=\"aID74249\" href=\"#\" OnMouseOver=\"link('_unit','assert_h0',this)\" class=\"leftMenuLinkInActive\">assert.h</a></div>\n");
document.write("<div id=\"divID74250\" class=\"leftMenuInActive\"><a id=\"aID74250\" href=\"#\" OnMouseOver=\"link('_unit','bmp_h0',this)\" class=\"leftMenuLinkInActive\">bmp.h</a></div>\n");
document.write("<div id=\"divID74256\" class=\"leftMenuInActive\"><a id=\"aID74256\" href=\"#\" OnMouseOver=\"link('_unit','led_h0',this)\" class=\"leftMenuLinkInActive\">led.h</a></div>\n");
document.write("<div id=\"divID74257\" class=\"leftMenuInActive\"><a id=\"aID74257\" href=\"#\" OnMouseOver=\"link('_unit','rand_h0',this)\" class=\"leftMenuLinkInActive\">rand.h</a></div>\n");
document.write("<div id=\"divID74258\" class=\"leftMenuInActive\"><a id=\"aID74258\" href=\"#\" OnMouseOver=\"link('_unit','retarget_c0',this)\" class=\"leftMenuLinkInActive\">retarget.c</a></div>\n");
document.write("<div id=\"divID74259\" class=\"leftMenuInActive\"><a id=\"aID74259\" href=\"#\" OnMouseOver=\"link('_unit','stdio_c0',this)\" class=\"leftMenuLinkInActive\">stdio.c</a></div>\n");
document.write("<div id=\"divID74260\" class=\"leftMenuInActive\"><a id=\"aID74260\" href=\"#\" OnMouseOver=\"link('_unit','string_c0',this)\" class=\"leftMenuLinkInActive\">string.c</a></div>\n");
document.write("<div id=\"divID74261\" class=\"leftMenuInActive\"><a id=\"aID74261\" href=\"#\" OnMouseOver=\"link('_unit','trace_h0',this)\" class=\"leftMenuLinkInActive\">trace.h</a></div>\n");
document.write("<div id=\"divID74262\" class=\"leftMenuInActive\"><a id=\"aID74262\" href=\"#\" OnMouseOver=\"link('_unit','video_h0',this)\" class=\"leftMenuLinkInActive\">video.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
