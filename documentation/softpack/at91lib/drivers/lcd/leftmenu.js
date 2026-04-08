function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73879\" class=\"headerLeftMenuInActive\"><a id=\"aID73879\" href=\"#\" OnMouseOver=\"link('_dir','lcd0',this)\" class=\"leftMenuLinkHeadInActive\">lcd</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74120\" class=\"leftMenuInActive\"><a id=\"aID74120\" href=\"#\" OnMouseOver=\"link('_unit','draw_h0',this)\" class=\"leftMenuLinkInActive\">draw.h</a></div>\n");
document.write("<div id=\"divID74121\" class=\"leftMenuInActive\"><a id=\"aID74121\" href=\"#\" OnMouseOver=\"link('_unit','font_h0',this)\" class=\"leftMenuLinkInActive\">font.h</a></div>\n");
document.write("<div id=\"divID74122\" class=\"leftMenuInActive\"><a id=\"aID74122\" href=\"#\" OnMouseOver=\"link('_unit','lcdd_h0',this)\" class=\"leftMenuLinkInActive\">lcdd.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
