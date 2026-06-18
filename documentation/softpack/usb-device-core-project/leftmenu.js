function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73992\" class=\"headerLeftMenuInActive\"><a id=\"aID73992\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_coremi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-core-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74301\" class=\"leftMenuInActive\"><a id=\"aID74301\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
