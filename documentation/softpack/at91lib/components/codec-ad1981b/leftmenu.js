function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73863\" class=\"headerLeftMenuInActive\"><a id=\"aID73863\" href=\"#\" OnMouseOver=\"link('_dir','codecmi_ad1981b0',this)\" class=\"leftMenuLinkHeadInActive\">codec-ad1981b</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
