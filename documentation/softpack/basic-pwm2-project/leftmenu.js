function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74003\" class=\"headerLeftMenuInActive\"><a id=\"aID74003\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_pwm2mi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-pwm2-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74311\" class=\"leftMenuInActive\"><a id=\"aID74311\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
