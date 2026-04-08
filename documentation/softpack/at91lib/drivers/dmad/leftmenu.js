function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73877\" class=\"headerLeftMenuInActive\"><a id=\"aID73877\" href=\"#\" OnMouseOver=\"link('_dir','dmad0',this)\" class=\"leftMenuLinkHeadInActive\">dmad</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74119\" class=\"leftMenuInActive\"><a id=\"aID74119\" href=\"#\" OnMouseOver=\"link('_unit','dmad_h0',this)\" class=\"leftMenuLinkInActive\">dmad.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
