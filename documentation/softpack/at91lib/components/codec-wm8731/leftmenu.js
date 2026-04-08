function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73864\" class=\"headerLeftMenuInActive\"><a id=\"aID73864\" href=\"#\" OnMouseOver=\"link('_dir','codecmi_wm87310',this)\" class=\"leftMenuLinkHeadInActive\">codec-wm8731</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74109\" class=\"leftMenuInActive\"><a id=\"aID74109\" href=\"#\" OnMouseOver=\"link('_unit','wm8731_c0',this)\" class=\"leftMenuLinkInActive\">wm8731.c</a></div>\n");
document.write("<div id=\"divID74110\" class=\"leftMenuInActive\"><a id=\"aID74110\" href=\"#\" OnMouseOver=\"link('_unit','wm8731_h0',this)\" class=\"leftMenuLinkInActive\">wm8731.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
