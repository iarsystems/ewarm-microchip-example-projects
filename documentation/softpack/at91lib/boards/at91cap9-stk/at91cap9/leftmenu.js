function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73823\" class=\"headerLeftMenuInActive\"><a id=\"aID73823\" href=\"#\" OnMouseOver=\"link('_dir','at91cap90',this)\" class=\"leftMenuLinkHeadInActive\">at91cap9</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74032\" class=\"leftMenuInActive\"><a id=\"aID74032\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
