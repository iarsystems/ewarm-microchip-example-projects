function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73982\" class=\"headerLeftMenuInActive\"><a id=\"aID73982\" href=\"#\" OnMouseOver=\"link('_dir','gettingmi_startedmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">getting-started-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74291\" class=\"leftMenuInActive\"><a id=\"aID74291\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
