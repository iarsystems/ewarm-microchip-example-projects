function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73967\" class=\"headerLeftMenuInActive\"><a id=\"aID73967\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_norflashmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-norflash-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74278\" class=\"leftMenuInActive\"><a id=\"aID74278\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
