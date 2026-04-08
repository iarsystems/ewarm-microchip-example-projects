function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73833\" class=\"headerLeftMenuInActive\"><a id=\"aID73833\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7s1280',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7s128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73533\" class=\"leftMenuInActive\"><a id=\"aID73533\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7S128mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7S128 - ARM core features</a></div>\n");
document.write("<div id=\"divID73534\" class=\"leftMenuInActive\"><a id=\"aID73534\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7S128mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7S128 - IP features</a></div>\n");
document.write("<div id=\"divID73535\" class=\"leftMenuInActive\"><a id=\"aID73535\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7S128mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7S128 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74054\" class=\"leftMenuInActive\"><a id=\"aID74054\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
