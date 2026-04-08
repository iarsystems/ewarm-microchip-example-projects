function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73895\" class=\"headerLeftMenuInActive\"><a id=\"aID73895\" href=\"#\" OnMouseOver=\"link('_dir','dbgu0',this)\" class=\"leftMenuLinkHeadInActive\">dbgu</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74145\" class=\"leftMenuInActive\"><a id=\"aID74145\" href=\"#\" OnMouseOver=\"link('_unit','dbgu_h0',this)\" class=\"leftMenuLinkInActive\">dbgu.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
