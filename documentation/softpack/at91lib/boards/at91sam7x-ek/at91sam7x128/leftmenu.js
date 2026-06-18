function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73837\" class=\"headerLeftMenuInActive\"><a id=\"aID73837\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7x1280',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7x128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73548\" class=\"leftMenuInActive\"><a id=\"aID73548\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7X128mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7X128 - ARM core features</a></div>\n");
document.write("<div id=\"divID73549\" class=\"leftMenuInActive\"><a id=\"aID73549\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7X128mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7X128 - IP features</a></div>\n");
document.write("<div id=\"divID73550\" class=\"leftMenuInActive\"><a id=\"aID73550\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7X128mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7X128 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74062\" class=\"leftMenuInActive\"><a id=\"aID74062\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
