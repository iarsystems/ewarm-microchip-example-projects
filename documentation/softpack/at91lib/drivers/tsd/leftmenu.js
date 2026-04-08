function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73881\" class=\"headerLeftMenuInActive\"><a id=\"aID73881\" href=\"#\" OnMouseOver=\"link('_dir','tsd0',this)\" class=\"leftMenuLinkHeadInActive\">tsd</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74124\" class=\"leftMenuInActive\"><a id=\"aID74124\" href=\"#\" OnMouseOver=\"link('_unit','tsd_h0',this)\" class=\"leftMenuLinkInActive\">tsd.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
