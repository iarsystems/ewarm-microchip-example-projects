function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73931\" class=\"headerLeftMenuInActive\"><a id=\"aID73931\" href=\"#\" OnMouseOver=\"link('_dir','ccid0',this)\" class=\"leftMenuLinkHeadInActive\">ccid</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74216\" class=\"leftMenuInActive\"><a id=\"aID74216\" href=\"#\" OnMouseOver=\"link('_unit','cciddriver_c0',this)\" class=\"leftMenuLinkInActive\">cciddriver.c</a></div>\n");
document.write("<div id=\"divID74217\" class=\"leftMenuInActive\"><a id=\"aID74217\" href=\"#\" OnMouseOver=\"link('_unit','cciddriver_h0',this)\" class=\"leftMenuLinkInActive\">cciddriver.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
