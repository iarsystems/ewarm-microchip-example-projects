function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73911\" class=\"headerLeftMenuInActive\"><a id=\"aID73911\" href=\"#\" OnMouseOver=\"link('_dir','rtt0',this)\" class=\"leftMenuLinkHeadInActive\">rtt</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74157\" class=\"leftMenuInActive\"><a id=\"aID74157\" href=\"#\" OnMouseOver=\"link('_unit','rtt_h0',this)\" class=\"leftMenuLinkInActive\">rtt.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
