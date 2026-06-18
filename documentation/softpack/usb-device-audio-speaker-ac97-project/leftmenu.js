function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73986\" class=\"headerLeftMenuInActive\"><a id=\"aID73986\" href=\"#\" OnMouseOver=\"link('_dir','usbmi_devicemi_audiomi_speakermi_ac97mi_project0',this)\" class=\"leftMenuLinkHeadInActive\">usb-device-audio-speaker-ac97-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74295\" class=\"leftMenuInActive\"><a id=\"aID74295\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
