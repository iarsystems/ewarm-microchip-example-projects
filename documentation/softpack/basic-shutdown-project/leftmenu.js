function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73974\" class=\"headerLeftMenuInActive\"><a id=\"aID73974\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_shutdownmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-shutdown-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74285\" class=\"leftMenuInActive\"><a id=\"aID74285\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
