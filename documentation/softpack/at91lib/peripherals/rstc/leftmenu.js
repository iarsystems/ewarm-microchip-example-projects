function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73909\" class=\"headerLeftMenuInActive\"><a id=\"aID73909\" href=\"#\" OnMouseOver=\"link('_dir','rstc0',this)\" class=\"leftMenuLinkHeadInActive\">rstc</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
