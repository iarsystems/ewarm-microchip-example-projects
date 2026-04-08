function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73866\" class=\"headerLeftMenuInActive\"><a id=\"aID73866\" href=\"#\" OnMouseOver=\"link('_dir','dacmi_at73c2130',this)\" class=\"leftMenuLinkHeadInActive\">dac-at73c213</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
