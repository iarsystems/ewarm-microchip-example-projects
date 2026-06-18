function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73945\" class=\"headerLeftMenuInActive\"><a id=\"aID73945\" href=\"#\" OnMouseOver=\"link('_dir','at91bootstrap0',this)\" class=\"leftMenuLinkHeadInActive\">at91bootstrap</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74263\" class=\"leftMenuInActive\"><a id=\"aID74263\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
