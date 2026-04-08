function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73862\" class=\"headerLeftMenuInActive\"><a id=\"aID73862\" href=\"#\" OnMouseOver=\"link('_dir','ads78430',this)\" class=\"leftMenuLinkHeadInActive\">ads7843</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74107\" class=\"leftMenuInActive\"><a id=\"aID74107\" href=\"#\" OnMouseOver=\"link('_unit','ads7843_c0',this)\" class=\"leftMenuLinkInActive\">ads7843.c</a></div>\n");
document.write("<div id=\"divID74108\" class=\"leftMenuInActive\"><a id=\"aID74108\" href=\"#\" OnMouseOver=\"link('_unit','ads7843_h0',this)\" class=\"leftMenuLinkInActive\">ads7843.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
