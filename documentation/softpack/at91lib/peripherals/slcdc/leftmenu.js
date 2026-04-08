function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73913\" class=\"headerLeftMenuInActive\"><a id=\"aID73913\" href=\"#\" OnMouseOver=\"link('_dir','slcdc0',this)\" class=\"leftMenuLinkHeadInActive\">slcdc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74158\" class=\"leftMenuInActive\"><a id=\"aID74158\" href=\"#\" OnMouseOver=\"link('_unit','slcdc_h0',this)\" class=\"leftMenuLinkInActive\">slcdc.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
