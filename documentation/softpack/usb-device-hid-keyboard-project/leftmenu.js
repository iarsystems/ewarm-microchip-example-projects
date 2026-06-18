function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73993\" class=\"headerLeftMenuInActive\"><a id=\"aID73993\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_hidmi_keyboardmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-hid-keyboard-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74302\" class=\"leftMenuInActive\"><a id=\"aID74302\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
