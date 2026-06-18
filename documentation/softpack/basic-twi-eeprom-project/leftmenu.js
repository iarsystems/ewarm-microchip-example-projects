function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73978\" class=\"headerLeftMenuInActive\"><a id=\"aID73978\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_twimi_eeprommi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-twi-eeprom-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74288\" class=\"leftMenuInActive\"><a id=\"aID74288\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
