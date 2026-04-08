function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73922\" class=\"headerLeftMenuInActive\"><a id=\"aID73922\" href=\"#\" OnMouseOver=\"link('_dir','twi0',this)\" class=\"leftMenuLinkHeadInActive\">twi</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74162\" class=\"leftMenuInActive\"><a id=\"aID74162\" href=\"#\" OnMouseOver=\"link('_unit','twi_c0',this)\" class=\"leftMenuLinkInActive\">twi.c</a></div>\n");
document.write("<div id=\"divID74163\" class=\"leftMenuInActive\"><a id=\"aID74163\" href=\"#\" OnMouseOver=\"link('_unit','twi_h0',this)\" class=\"leftMenuLinkInActive\">twi.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
