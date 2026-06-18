function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73939\" class=\"headerLeftMenuInActive\"><a id=\"aID73939\" href=\"#\" OnMouseOver=\"link('_dir','hidmi_mouse0',this)\" class=\"leftMenuLinkHeadInActive\">hid-mouse</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73789\" class=\"leftMenuInActive\"><a id=\"aID73789\" href=\"#\" OnMouseOver=\"link('_page','HIDMouseButtonbitmaps0',this)\" class=\"leftMenuLinkInActive\">HID Mouse Button bitmaps</a></div>\n");
document.write("<div id=\"divID73790\" class=\"leftMenuInActive\"><a id=\"aID73790\" href=\"#\" OnMouseOver=\"link('_page','HIDMouseDeviceDescriptorIDs0',this)\" class=\"leftMenuLinkInActive\">HID Mouse Device Descriptor IDs</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74237\" class=\"leftMenuInActive\"><a id=\"aID74237\" href=\"#\" OnMouseOver=\"link('_unit','HIDDMouseDriver_h0',this)\" class=\"leftMenuLinkInActive\">HIDDMouseDriver.h</a></div>\n");
document.write("<div id=\"divID74238\" class=\"leftMenuInActive\"><a id=\"aID74238\" href=\"#\" OnMouseOver=\"link('_unit','HIDDMouseDriverDescriptors_h0',this)\" class=\"leftMenuLinkInActive\">HIDDMouseDriverDescriptors.h</a></div>\n");
document.write("<div id=\"divID74239\" class=\"leftMenuInActive\"><a id=\"aID74239\" href=\"#\" OnMouseOver=\"link('_unit','HIDDMouseInputReport_h0',this)\" class=\"leftMenuLinkInActive\">HIDDMouseInputReport.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
