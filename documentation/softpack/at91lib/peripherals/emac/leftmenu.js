function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73899\" class=\"headerLeftMenuInActive\"><a id=\"aID73899\" href=\"#\" OnMouseOver=\"link('_dir','emac0',this)\" class=\"leftMenuLinkHeadInActive\">emac</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
