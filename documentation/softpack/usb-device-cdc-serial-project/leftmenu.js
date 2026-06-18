function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73987\" class=\"headerLeftMenuInActive\"><a id=\"aID73987\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_cdcmi_serialmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-cdc-serial-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74296\" class=\"leftMenuInActive\"><a id=\"aID74296\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
