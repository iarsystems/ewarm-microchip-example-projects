function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73883\" class=\"headerLeftMenuInActive\"><a id=\"aID73883\" href=\"#\" OnMouseOver=\"link('_dir','flash0',this)\" class=\"leftMenuLinkHeadInActive\">flash</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74125\" class=\"leftMenuInActive\"><a id=\"aID74125\" href=\"#\" OnMouseOver=\"link('_unit','flashd_h0',this)\" class=\"leftMenuLinkInActive\">flashd.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
