function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73908\" class=\"headerLeftMenuInActive\"><a id=\"aID73908\" href=\"#\" OnMouseOver=\"link('_dir','pwmc0',this)\" class=\"leftMenuLinkHeadInActive\">pwmc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74155\" class=\"leftMenuInActive\"><a id=\"aID74155\" href=\"#\" OnMouseOver=\"link('_unit','pwmc_h0',this)\" class=\"leftMenuLinkInActive\">pwmc.h</a></div>\n");
document.write("<div id=\"divID74156\" class=\"leftMenuInActive\"><a id=\"aID74156\" href=\"#\" OnMouseOver=\"link('_unit','pwmc2_h0',this)\" class=\"leftMenuLinkInActive\">pwmc2.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
