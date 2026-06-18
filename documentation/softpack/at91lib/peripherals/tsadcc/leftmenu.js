function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73921\" class=\"headerLeftMenuInActive\"><a id=\"aID73921\" href=\"#\" OnMouseOver=\"link('_dir','tsadcc0',this)\" class=\"leftMenuLinkHeadInActive\">tsadcc</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
