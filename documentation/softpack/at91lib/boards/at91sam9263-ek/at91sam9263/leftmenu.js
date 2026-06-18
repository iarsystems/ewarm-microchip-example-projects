function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73845\" class=\"headerLeftMenuInActive\"><a id=\"aID73845\" href=\"#\" OnMouseOver=\"link('_dir','at91sam92630',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9263</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73585\" class=\"leftMenuInActive\"><a id=\"aID73585\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9263mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9263 - ARM core features</a></div>\n");
document.write("<div id=\"divID73586\" class=\"leftMenuInActive\"><a id=\"aID73586\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9263mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9263 - IP features</a></div>\n");
document.write("<div id=\"divID73587\" class=\"leftMenuInActive\"><a id=\"aID73587\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9263mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9263 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74079\" class=\"leftMenuInActive\"><a id=\"aID74079\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
