function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73965\" class=\"headerLeftMenuInActive\"><a id=\"aID73965\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_lcdmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-lcd-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74276\" class=\"leftMenuInActive\"><a id=\"aID74276\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
