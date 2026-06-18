function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73825\" class=\"headerLeftMenuInActive\"><a id=\"aID73825\" href=\"#\" OnMouseOver=\"link('_dir','at91sam3u40',this)\" class=\"leftMenuLinkHeadInActive\">at91sam3u4</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73500\" class=\"leftMenuInActive\"><a id=\"aID73500\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM3U4mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM3U4 - ARM core features</a></div>\n");
document.write("<div id=\"divID73501\" class=\"leftMenuInActive\"><a id=\"aID73501\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM3U4mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM3U4 - IP features</a></div>\n");
document.write("<div id=\"divID73502\" class=\"leftMenuInActive\"><a id=\"aID73502\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM3U4mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM3U4 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74035\" class=\"leftMenuInActive\"><a id=\"aID74035\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
