function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73903\" class=\"headerLeftMenuInActive\"><a id=\"aID73903\" href=\"#\" OnMouseOver=\"link('_dir','mci0',this)\" class=\"leftMenuLinkHeadInActive\">mci</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73684\" class=\"leftMenuInActive\"><a id=\"aID73684\" href=\"#\" OnMouseOver=\"link('_page','mci0',this)\" class=\"leftMenuLinkInActive\">mci</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
