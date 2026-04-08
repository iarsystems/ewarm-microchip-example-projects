function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73920\" class=\"headerLeftMenuInActive\"><a id=\"aID73920\" href=\"#\" OnMouseOver=\"link('_dir','tdes0',this)\" class=\"leftMenuLinkHeadInActive\">tdes</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74161\" class=\"leftMenuInActive\"><a id=\"aID74161\" href=\"#\" OnMouseOver=\"link('_unit','tdes_h0',this)\" class=\"leftMenuLinkInActive\">tdes.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
