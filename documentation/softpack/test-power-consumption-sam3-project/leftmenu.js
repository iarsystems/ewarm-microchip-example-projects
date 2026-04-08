function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74009\" class=\"headerLeftMenuInActive\"><a id=\"aID74009\" href=\"#\" OnMouseOver=\"link('_dir','testmi_powermi_consumptionmi_sam3mi_project0',this)\" class=\"leftMenuLinkHeadInActive\">test-power-consumption-sam3-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74316\" class=\"leftMenuInActive\"><a id=\"aID74316\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
