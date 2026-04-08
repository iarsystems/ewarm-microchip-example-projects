function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73847\" class=\"headerLeftMenuInActive\"><a id=\"aID73847\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g100',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9g10</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73595\" class=\"leftMenuInActive\"><a id=\"aID73595\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G10mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G10 - ARM core features</a></div>\n");
document.write("<div id=\"divID73596\" class=\"leftMenuInActive\"><a id=\"aID73596\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G10mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G10 - IP features</a></div>\n");
document.write("<div id=\"divID73597\" class=\"leftMenuInActive\"><a id=\"aID73597\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G10mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G10 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74082\" class=\"leftMenuInActive\"><a id=\"aID74082\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
