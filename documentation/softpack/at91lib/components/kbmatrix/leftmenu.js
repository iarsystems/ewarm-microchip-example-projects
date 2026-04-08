function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73869\" class=\"headerLeftMenuInActive\"><a id=\"aID73869\" href=\"#\" OnMouseOver=\"link('_dir','kbmatrix0',this)\" class=\"leftMenuLinkHeadInActive\">kbmatrix</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73870\" class=\"leftMenuInActive\"><a id=\"aID73870\" href=\"#\" OnMouseOver=\"link('_dir','s7lekkbm/s7lekkbm0',this)\" class=\"leftMenuLinkInActive\">s7lekkbm</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
