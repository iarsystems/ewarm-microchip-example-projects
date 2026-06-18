function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73933\" class=\"headerLeftMenuInActive\"><a id=\"aID73933\" href=\"#\" OnMouseOver=\"link('_dir','drv0',this)\" class=\"leftMenuLinkHeadInActive\">drv</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
