function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73829\" class=\"headerLeftMenuInActive\"><a id=\"aID73829\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7l1280',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7l128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73519\" class=\"leftMenuInActive\"><a id=\"aID73519\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7L128mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7L128 - ARM core features</a></div>\n");
document.write("<div id=\"divID73520\" class=\"leftMenuInActive\"><a id=\"aID73520\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7L128mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7L128 - IP features</a></div>\n");
document.write("<div id=\"divID73521\" class=\"leftMenuInActive\"><a id=\"aID73521\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM7L128mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM7L128 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74045\" class=\"leftMenuInActive\"><a id=\"aID74045\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
