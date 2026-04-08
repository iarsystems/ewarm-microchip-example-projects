function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73890\" class=\"headerLeftMenuInActive\"><a id=\"aID73890\" href=\"#\" OnMouseOver=\"link('_dir','adc0',this)\" class=\"leftMenuLinkHeadInActive\">adc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74140\" class=\"leftMenuInActive\"><a id=\"aID74140\" href=\"#\" OnMouseOver=\"link('_unit','adc_h0',this)\" class=\"leftMenuLinkInActive\">adc.h</a></div>\n");
document.write("<div id=\"divID74141\" class=\"leftMenuInActive\"><a id=\"aID74141\" href=\"#\" OnMouseOver=\"link('_unit','adc12_h0',this)\" class=\"leftMenuLinkInActive\">adc12.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
