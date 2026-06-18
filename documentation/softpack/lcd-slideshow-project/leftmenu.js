function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73983\" class=\"headerLeftMenuInActive\"><a id=\"aID73983\" href=\"#\" OnMouseOver=\"link('_dir','lcdmi_slideshowmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">lcd-slideshow-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74292\" class=\"leftMenuInActive\"><a id=\"aID74292\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
