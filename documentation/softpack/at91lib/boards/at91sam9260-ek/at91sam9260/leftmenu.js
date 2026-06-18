function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73841\" class=\"headerLeftMenuInActive\"><a id=\"aID73841\" href=\"#\" OnMouseOver=\"link('_dir','at91sam92600',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9260</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73566\" class=\"leftMenuInActive\"><a id=\"aID73566\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9260mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9260 - ARM core features</a></div>\n");
document.write("<div id=\"divID73567\" class=\"leftMenuInActive\"><a id=\"aID73567\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9260mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9260 - IP features</a></div>\n");
document.write("<div id=\"divID73568\" class=\"leftMenuInActive\"><a id=\"aID73568\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9260mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9260 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74072\" class=\"leftMenuInActive\"><a id=\"aID74072\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
