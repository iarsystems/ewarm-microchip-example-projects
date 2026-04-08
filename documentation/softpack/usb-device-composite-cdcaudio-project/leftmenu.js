function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74023\" class=\"headerLeftMenuInActive\"><a id=\"aID74023\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_compositemi_cdcaudiomi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-composite-cdcaudio-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74324\" class=\"leftMenuInActive\"><a id=\"aID74324\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
