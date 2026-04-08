function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73870\" class=\"headerLeftMenuInActive\"><a id=\"aID73870\" href=\"#\" OnMouseOver=\"link('_dir','s7lekkbm0',this)\" class=\"leftMenuLinkHeadInActive\">s7lekkbm</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
