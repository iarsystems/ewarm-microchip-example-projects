function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73867\" class=\"headerLeftMenuInActive\"><a id=\"aID73867\" href=\"#\" OnMouseOver=\"link('_dir','hx83470',this)\" class=\"leftMenuLinkHeadInActive\">hx8347</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74111\" class=\"leftMenuInActive\"><a id=\"aID74111\" href=\"#\" OnMouseOver=\"link('_unit','hx8347_c0',this)\" class=\"leftMenuLinkInActive\">hx8347.c</a></div>\n");
document.write("<div id=\"divID74112\" class=\"leftMenuInActive\"><a id=\"aID74112\" href=\"#\" OnMouseOver=\"link('_unit','hx8347_h0',this)\" class=\"leftMenuLinkInActive\">hx8347.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
