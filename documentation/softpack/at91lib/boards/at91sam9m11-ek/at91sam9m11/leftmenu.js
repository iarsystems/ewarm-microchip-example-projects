function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73855\" class=\"headerLeftMenuInActive\"><a id=\"aID73855\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m110',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9m11</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73634\" class=\"leftMenuInActive\"><a id=\"aID73634\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9M11mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9M11 - ARM core features</a></div>\n");
document.write("<div id=\"divID73635\" class=\"leftMenuInActive\"><a id=\"aID73635\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9M11mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9M11 - IP features</a></div>\n");
document.write("<div id=\"divID73636\" class=\"leftMenuInActive\"><a id=\"aID73636\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9M11mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9M11 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74097\" class=\"leftMenuInActive\"><a id=\"aID74097\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
