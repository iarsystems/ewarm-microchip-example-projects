function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73821\" class=\"headerLeftMenuInActive\"><a id=\"aID73821\" href=\"#\" OnMouseOver=\"link('_dir','at91cap90',this)\" class=\"leftMenuLinkHeadInActive\">at91cap9</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73483\" class=\"leftMenuInActive\"><a id=\"aID73483\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9 - ARM core features</a></div>\n");
document.write("<div id=\"divID73484\" class=\"leftMenuInActive\"><a id=\"aID73484\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9 - IP features</a></div>\n");
document.write("<div id=\"divID73485\" class=\"leftMenuInActive\"><a id=\"aID73485\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74029\" class=\"leftMenuInActive\"><a id=\"aID74029\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
