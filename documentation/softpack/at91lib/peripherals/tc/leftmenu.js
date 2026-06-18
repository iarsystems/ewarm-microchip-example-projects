function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73919\" class=\"headerLeftMenuInActive\"><a id=\"aID73919\" href=\"#\" OnMouseOver=\"link('_dir','tc0',this)\" class=\"leftMenuLinkHeadInActive\">tc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74160\" class=\"leftMenuInActive\"><a id=\"aID74160\" href=\"#\" OnMouseOver=\"link('_unit','tc_h0',this)\" class=\"leftMenuLinkInActive\">tc.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
