function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73880\" class=\"headerLeftMenuInActive\"><a id=\"aID73880\" href=\"#\" OnMouseOver=\"link('_dir','macb0',this)\" class=\"leftMenuLinkHeadInActive\">macb</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74123\" class=\"leftMenuInActive\"><a id=\"aID74123\" href=\"#\" OnMouseOver=\"link('_unit','macb_h0',this)\" class=\"leftMenuLinkInActive\">macb.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
