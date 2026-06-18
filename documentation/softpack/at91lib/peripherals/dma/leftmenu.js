function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73896\" class=\"headerLeftMenuInActive\"><a id=\"aID73896\" href=\"#\" OnMouseOver=\"link('_dir','dma0',this)\" class=\"leftMenuLinkHeadInActive\">dma</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
