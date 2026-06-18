function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73898\" class=\"headerLeftMenuInActive\"><a id=\"aID73898\" href=\"#\" OnMouseOver=\"link('_dir','efc0',this)\" class=\"leftMenuLinkHeadInActive\">efc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74147\" class=\"leftMenuInActive\"><a id=\"aID74147\" href=\"#\" OnMouseOver=\"link('_unit','efc_h0',this)\" class=\"leftMenuLinkInActive\">efc.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
