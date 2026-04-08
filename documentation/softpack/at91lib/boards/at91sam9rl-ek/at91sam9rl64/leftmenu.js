function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73857\" class=\"headerLeftMenuInActive\"><a id=\"aID73857\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9rl640',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9rl64</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73643\" class=\"leftMenuInActive\"><a id=\"aID73643\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9RL64mi_ARMcorefeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9RL64 - ARM core features</a></div>\n");
document.write("<div id=\"divID73644\" class=\"leftMenuInActive\"><a id=\"aID73644\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9RL64mi_IPfeatures0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9RL64 - IP features</a></div>\n");
document.write("<div id=\"divID73645\" class=\"leftMenuInActive\"><a id=\"aID73645\" href=\"#\" OnMouseOver=\"link('_page','AT91SAM9RL64mi_Misc0',this)\" class=\"leftMenuLinkInActive\">AT91SAM9RL64 - Misc </a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74101\" class=\"leftMenuInActive\"><a id=\"aID74101\" href=\"#\" OnMouseOver=\"link('_unit','chip_h0',this)\" class=\"leftMenuLinkInActive\">chip.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
