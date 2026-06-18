function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73956\" class=\"headerLeftMenuInActive\"><a id=\"aID73956\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_emacmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-emac-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74269\" class=\"leftMenuInActive\"><a id=\"aID74269\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
