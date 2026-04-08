function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73827\" class=\"headerLeftMenuInActive\"><a id=\"aID73827\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7a30',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7a3</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73510\" class=\"leftMenuInActive\"><a id=\"aID73510\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7A3mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7A3 - ARM core features</a></div>\n");
document.write("<div id=\"divID73511\" class=\"leftMenuInActive\"><a id=\"aID73511\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7A3mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7A3 - IP features</a></div>\n");
document.write("<div id=\"divID73512\" class=\"leftMenuInActive\"><a id=\"aID73512\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7A3mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7A3 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74040\" class=\"leftMenuInActive\"><a id=\"aID74040\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
