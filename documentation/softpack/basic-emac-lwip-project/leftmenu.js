function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73955\" class=\"headerLeftMenuInActive\"><a id=\"aID73955\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_emacmi_lwipmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-emac-lwip-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73954\" class=\"leftMenuInActive\"><a id=\"aID73954\" href=\"#\" OnMouseOver=\"link('_dir','arch/arch0',this)\" class=\"leftMenuLinkInActive\">arch</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74268\" class=\"leftMenuInActive\"><a id=\"aID74268\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
