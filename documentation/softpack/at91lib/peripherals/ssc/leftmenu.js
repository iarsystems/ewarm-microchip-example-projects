function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73916\" class=\"headerLeftMenuInActive\"><a id=\"aID73916\" href=\"#\" OnMouseOver=\"link('_dir','ssc0',this)\" class=\"leftMenuLinkHeadInActive\">ssc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73686\" class=\"leftMenuInActive\"><a id=\"aID73686\" href=\"#\" OnMouseOver=\"link('_page','SSCconfigurationmacros0',this)\" class=\"leftMenuLinkInActive\">SSC configuration macros</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
