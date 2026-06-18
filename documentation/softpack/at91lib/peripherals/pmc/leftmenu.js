function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73907\" class=\"headerLeftMenuInActive\"><a id=\"aID73907\" href=\"#\" OnMouseOver=\"link('_dir','pmc0',this)\" class=\"leftMenuLinkHeadInActive\">pmc</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
