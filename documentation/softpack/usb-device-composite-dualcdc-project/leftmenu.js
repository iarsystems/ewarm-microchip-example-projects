function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73990\" class=\"headerLeftMenuInActive\"><a id=\"aID73990\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_compositemi_dualcdcmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-composite-dualcdc-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74299\" class=\"leftMenuInActive\"><a id=\"aID74299\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
