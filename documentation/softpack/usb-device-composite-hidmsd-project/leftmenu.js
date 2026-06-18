function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73991\" class=\"headerLeftMenuInActive\"><a id=\"aID73991\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_compositemi_hidmsdmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-composite-hidmsd-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74300\" class=\"leftMenuInActive\"><a id=\"aID74300\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
