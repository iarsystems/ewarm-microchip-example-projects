function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73954\" class=\"headerLeftMenuInActive\"><a id=\"aID73954\" href=\"#\" OnMouseOver=\"link('_dir','arch0',this)\" class=\"leftMenuLinkHeadInActive\">arch</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
