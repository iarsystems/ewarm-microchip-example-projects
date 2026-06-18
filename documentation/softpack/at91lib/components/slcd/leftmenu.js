function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74028\" class=\"headerLeftMenuInActive\"><a id=\"aID74028\" href=\"#\" OnMouseOver=\"link('_dir','slcd0',this)\" class=\"leftMenuLinkHeadInActive\">slcd</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73874\" class=\"leftMenuInActive\"><a id=\"aID73874\" href=\"#\" OnMouseOver=\"link('_dir','s7leklcd/s7leklcd0',this)\" class=\"leftMenuLinkInActive\">s7leklcd</a></div>\n");
document.write("<div id=\"divID73875\" class=\"leftMenuInActive\"><a id=\"aID73875\" href=\"#\" OnMouseOver=\"link('_dir','s7lstklcd/s7lstklcd0',this)\" class=\"leftMenuLinkInActive\">s7lstklcd</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
