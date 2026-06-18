function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74013\" class=\"headerLeftMenuInActive\"><a id=\"aID74013\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_barometermi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-barometer-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74319\" class=\"leftMenuInActive\"><a id=\"aID74319\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
