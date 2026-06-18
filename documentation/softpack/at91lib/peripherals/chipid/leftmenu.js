function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73893\" class=\"headerLeftMenuInActive\"><a id=\"aID73893\" href=\"#\" OnMouseOver=\"link('_dir','chipid0',this)\" class=\"leftMenuLinkHeadInActive\">chipid</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74143\" class=\"leftMenuInActive\"><a id=\"aID74143\" href=\"#\" OnMouseOver=\"link('_unit','chipid_h0',this)\" class=\"leftMenuLinkInActive\">chipid.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
