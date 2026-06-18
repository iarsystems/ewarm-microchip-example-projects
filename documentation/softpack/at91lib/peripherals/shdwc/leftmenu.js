function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73912\" class=\"headerLeftMenuInActive\"><a id=\"aID73912\" href=\"#\" OnMouseOver=\"link('_dir','shdwc0',this)\" class=\"leftMenuLinkHeadInActive\">shdwc</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
