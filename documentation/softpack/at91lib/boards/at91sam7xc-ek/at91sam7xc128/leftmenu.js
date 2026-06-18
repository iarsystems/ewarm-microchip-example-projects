function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73839\" class=\"headerLeftMenuInActive\"><a id=\"aID73839\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7xc1280',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7xc128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73557\" class=\"leftMenuInActive\"><a id=\"aID73557\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7XC128mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7XC128 - ARM core features</a></div>\n");
document.write("<div id=\"divID73558\" class=\"leftMenuInActive\"><a id=\"aID73558\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7XC128mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7XC128 - IP features</a></div>\n");
document.write("<div id=\"divID73559\" class=\"leftMenuInActive\"><a id=\"aID73559\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7XC128mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7XC128 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74067\" class=\"leftMenuInActive\"><a id=\"aID74067\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
