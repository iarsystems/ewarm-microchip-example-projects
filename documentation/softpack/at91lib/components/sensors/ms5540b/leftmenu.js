function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73873\" class=\"headerLeftMenuInActive\"><a id=\"aID73873\" href=\"#\" OnMouseOver=\"link('_dir','ms5540b0',this)\" class=\"leftMenuLinkHeadInActive\">ms5540b</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
