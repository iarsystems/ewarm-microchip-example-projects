function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73894\" class=\"headerLeftMenuInActive\"><a id=\"aID73894\" href=\"#\" OnMouseOver=\"link('_dir','cp150',this)\" class=\"leftMenuLinkHeadInActive\">cp15</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74144\" class=\"leftMenuInActive\"><a id=\"aID74144\" href=\"#\" OnMouseOver=\"link('_unit','cp15_h0',this)\" class=\"leftMenuLinkInActive\">cp15.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
