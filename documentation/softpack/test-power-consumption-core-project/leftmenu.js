function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73984\" class=\"headerLeftMenuInActive\"><a id=\"aID73984\" href=\"#\" OnMouseOver=\"link('_dir','testmi_powermi_consumptionmi_coremi_project0',this)\" class=\"leftMenuLinkHeadInActive\">test-power-consumption-core-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74293\" class=\"leftMenuInActive\"><a id=\"aID74293\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
