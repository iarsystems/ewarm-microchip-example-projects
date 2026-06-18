function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73900\" class=\"headerLeftMenuInActive\"><a id=\"aID73900\" href=\"#\" OnMouseOver=\"link('_dir','irq0',this)\" class=\"leftMenuLinkHeadInActive\">irq</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74148\" class=\"leftMenuInActive\"><a id=\"aID74148\" href=\"#\" OnMouseOver=\"link('_unit','irq_h0',this)\" class=\"leftMenuLinkInActive\">irq.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
