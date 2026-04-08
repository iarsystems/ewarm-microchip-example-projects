function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73902\" class=\"headerLeftMenuInActive\"><a id=\"aID73902\" href=\"#\" OnMouseOver=\"link('_dir','lcd0',this)\" class=\"leftMenuLinkHeadInActive\">lcd</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74151\" class=\"leftMenuInActive\"><a id=\"aID74151\" href=\"#\" OnMouseOver=\"link('_unit','lcd_h0',this)\" class=\"leftMenuLinkInActive\">lcd.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
