function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73868\" class=\"headerLeftMenuInActive\"><a id=\"aID73868\" href=\"#\" OnMouseOver=\"link('_dir','iso78160',this)\" class=\"leftMenuLinkHeadInActive\">iso7816</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74113\" class=\"leftMenuInActive\"><a id=\"aID74113\" href=\"#\" OnMouseOver=\"link('_unit','iso7816_4_c0',this)\" class=\"leftMenuLinkInActive\">iso7816_4.c</a></div>\n");
document.write("<div id=\"divID74114\" class=\"leftMenuInActive\"><a id=\"aID74114\" href=\"#\" OnMouseOver=\"link('_unit','iso7816_4_h0',this)\" class=\"leftMenuLinkInActive\">iso7816_4.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
