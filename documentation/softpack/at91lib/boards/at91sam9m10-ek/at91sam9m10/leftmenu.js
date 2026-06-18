function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73853\" class=\"headerLeftMenuInActive\"><a id=\"aID73853\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m100',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9m10</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73625\" class=\"leftMenuInActive\"><a id=\"aID73625\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9M10mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9M10 - ARM core features</a></div>\n");
document.write("<div id=\"divID73626\" class=\"leftMenuInActive\"><a id=\"aID73626\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9M10mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9M10 - IP features</a></div>\n");
document.write("<div id=\"divID73627\" class=\"leftMenuInActive\"><a id=\"aID73627\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9M10mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9M10 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74093\" class=\"leftMenuInActive\"><a id=\"aID74093\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
