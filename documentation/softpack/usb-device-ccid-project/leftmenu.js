function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74012\" class=\"headerLeftMenuInActive\"><a id=\"aID74012\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_ccidmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-ccid-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74318\" class=\"leftMenuInActive\"><a id=\"aID74318\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
