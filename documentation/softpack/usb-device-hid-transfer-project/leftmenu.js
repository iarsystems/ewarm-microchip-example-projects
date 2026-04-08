function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73996\" class=\"headerLeftMenuInActive\"><a id=\"aID73996\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_hidmi_transfermi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-hid-transfer-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73995\" class=\"leftMenuInActive\"><a id=\"aID73995\" href=\"#\" OnMouseOver=\"link('_dir','HIDTest/HIDTest0',this)\" class=\"leftMenuLinkInActive\">HIDTest</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74304\" class=\"leftMenuInActive\"><a id=\"aID74304\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
