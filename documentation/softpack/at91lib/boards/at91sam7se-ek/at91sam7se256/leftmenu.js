function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73835\" class=\"headerLeftMenuInActive\"><a id=\"aID73835\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7se2560',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7se256</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73538\" class=\"leftMenuInActive\"><a id=\"aID73538\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7SE256mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7SE256 - ARM core features</a></div>\n");
document.write("<div id=\"divID73539\" class=\"leftMenuInActive\"><a id=\"aID73539\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7SE256mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7SE256 - IP features</a></div>\n");
document.write("<div id=\"divID73540\" class=\"leftMenuInActive\"><a id=\"aID73540\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7SE256mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7SE256 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74058\" class=\"leftMenuInActive\"><a id=\"aID74058\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
