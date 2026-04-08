function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73875\" class=\"headerLeftMenuInActive\"><a id=\"aID73875\" href=\"#\" OnMouseOver=\"link('_dir','s7lstklcd0',this)\" class=\"leftMenuLinkHeadInActive\">s7lstklcd</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74118\" class=\"leftMenuInActive\"><a id=\"aID74118\" href=\"#\" OnMouseOver=\"link('_unit','s7lstklcd_h0',this)\" class=\"leftMenuLinkInActive\">s7lstklcd.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
