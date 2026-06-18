function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73859\" class=\"headerLeftMenuInActive\"><a id=\"aID73859\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9xe1280',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9xe128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73652\" class=\"leftMenuInActive\"><a id=\"aID73652\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9XE128mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9XE128 - ARM core features</a></div>\n");
document.write("<div id=\"divID73653\" class=\"leftMenuInActive\"><a id=\"aID73653\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9XE128mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9XE128 - IP features</a></div>\n");
document.write("<div id=\"divID73654\" class=\"leftMenuInActive\"><a id=\"aID73654\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9XE128mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9XE128 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74104\" class=\"leftMenuInActive\"><a id=\"aID74104\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
