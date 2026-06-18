function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73985\" class=\"headerLeftMenuInActive\"><a id=\"aID73985\" href=\"#\" OnMouseOver=\"link('_dir','testmi_powermi_consumptionmi_periphmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">test-power-consumption-periph-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74294\" class=\"leftMenuInActive\"><a id=\"aID74294\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
