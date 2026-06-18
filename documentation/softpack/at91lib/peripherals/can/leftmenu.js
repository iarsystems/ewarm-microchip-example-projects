function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73892\" class=\"headerLeftMenuInActive\"><a id=\"aID73892\" href=\"#\" OnMouseOver=\"link('_dir','can0',this)\" class=\"leftMenuLinkHeadInActive\">can</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
