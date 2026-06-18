function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73831\" class=\"headerLeftMenuInActive\"><a id=\"aID73831\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7l1280',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7l128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74050\" class=\"leftMenuInActive\"><a id=\"aID74050\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
