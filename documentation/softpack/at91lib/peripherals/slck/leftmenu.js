function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73914\" class=\"headerLeftMenuInActive\"><a id=\"aID73914\" href=\"#\" OnMouseOver=\"link('_dir','slck0',this)\" class=\"leftMenuLinkHeadInActive\">slck</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
