function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73849\" class=\"headerLeftMenuInActive\"><a id=\"aID73849\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g200',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9g20</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73605\" class=\"leftMenuInActive\"><a id=\"aID73605\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G20mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G20 - ARM core features</a></div>\n");
document.write("<div id=\"divID73606\" class=\"leftMenuInActive\"><a id=\"aID73606\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G20mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G20 - IP features</a></div>\n");
document.write("<div id=\"divID73607\" class=\"leftMenuInActive\"><a id=\"aID73607\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G20mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G20 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74085\" class=\"leftMenuInActive\"><a id=\"aID74085\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
