function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73910\" class=\"headerLeftMenuInActive\"><a id=\"aID73910\" href=\"#\" OnMouseOver=\"link('_dir','rtc0',this)\" class=\"leftMenuLinkHeadInActive\">rtc</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
