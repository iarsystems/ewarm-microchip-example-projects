function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74018\" class=\"headerLeftMenuInActive\"><a id=\"aID74018\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_slcdmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-slcd-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74321\" class=\"leftMenuInActive\"><a id=\"aID74321\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
