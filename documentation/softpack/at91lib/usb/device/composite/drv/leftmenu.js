function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73935\" class=\"headerLeftMenuInActive\"><a id=\"aID73935\" href=\"#\" OnMouseOver=\"link('_dir','drv0',this)\" class=\"leftMenuLinkHeadInActive\">drv</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
