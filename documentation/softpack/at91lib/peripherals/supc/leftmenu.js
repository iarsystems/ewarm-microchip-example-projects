function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73917\" class=\"headerLeftMenuInActive\"><a id=\"aID73917\" href=\"#\" OnMouseOver=\"link('_dir','supc0',this)\" class=\"leftMenuLinkHeadInActive\">supc</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
