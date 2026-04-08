function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74002\" class=\"headerLeftMenuInActive\"><a id=\"aID74002\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_parallelmi_lcdmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-parallel-lcd-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74310\" class=\"leftMenuInActive\"><a id=\"aID74310\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
