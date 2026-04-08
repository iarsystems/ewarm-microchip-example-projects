function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74027\" class=\"headerLeftMenuInActive\"><a id=\"aID74027\" href=\"#\" OnMouseOver=\"link('_dir','sensors0',this)\" class=\"leftMenuLinkHeadInActive\">sensors</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73873\" class=\"leftMenuInActive\"><a id=\"aID73873\" href=\"#\" OnMouseOver=\"link('_dir','ms5540b/ms5540b0',this)\" class=\"leftMenuLinkInActive\">ms5540b</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
