function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73918\" class=\"headerLeftMenuInActive\"><a id=\"aID73918\" href=\"#\" OnMouseOver=\"link('_dir','systick0',this)\" class=\"leftMenuLinkHeadInActive\">systick</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74159\" class=\"leftMenuInActive\"><a id=\"aID74159\" href=\"#\" OnMouseOver=\"link('_unit','systick_h0',this)\" class=\"leftMenuLinkInActive\">systick.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
