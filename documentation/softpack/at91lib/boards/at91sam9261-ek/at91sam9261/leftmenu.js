function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73843\" class=\"headerLeftMenuInActive\"><a id=\"aID73843\" href=\"#\" OnMouseOver=\"link('_dir','at91sam92610',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9261</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73576\" class=\"leftMenuInActive\"><a id=\"aID73576\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9261mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9261 - ARM core features</a></div>\n");
document.write("<div id=\"divID73577\" class=\"leftMenuInActive\"><a id=\"aID73577\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9261mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9261 - IP features</a></div>\n");
document.write("<div id=\"divID73578\" class=\"leftMenuInActive\"><a id=\"aID73578\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9261mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9261 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74076\" class=\"leftMenuInActive\"><a id=\"aID74076\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
