function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74001\" class=\"headerLeftMenuInActive\"><a id=\"aID74001\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_internalflashmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-internalflash-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74309\" class=\"leftMenuInActive\"><a id=\"aID74309\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
