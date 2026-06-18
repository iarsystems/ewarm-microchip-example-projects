function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73882\" class=\"headerLeftMenuInActive\"><a id=\"aID73882\" href=\"#\" OnMouseOver=\"link('_dir','twi0',this)\" class=\"leftMenuLinkHeadInActive\">twi</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
