function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73851\" class=\"headerLeftMenuInActive\"><a id=\"aID73851\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g450',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9g45</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73615\" class=\"leftMenuInActive\"><a id=\"aID73615\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G45mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G45 - ARM core features</a></div>\n");
document.write("<div id=\"divID73616\" class=\"leftMenuInActive\"><a id=\"aID73616\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G45mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G45 - IP features</a></div>\n");
document.write("<div id=\"divID73617\" class=\"leftMenuInActive\"><a id=\"aID73617\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9G45mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9G45 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74089\" class=\"leftMenuInActive\"><a id=\"aID74089\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
