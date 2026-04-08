function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73940\" class=\"headerLeftMenuInActive\"><a id=\"aID73940\" href=\"#\" OnMouseOver=\"link('_dir','hidmi_transfer0',this)\" class=\"leftMenuLinkHeadInActive\">hid-transfer</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73792\" class=\"leftMenuInActive\"><a id=\"aID73792\" href=\"#\" OnMouseOver=\"link('_page','HIDTransferDeviceDescriptorIDs0',this)\" class=\"leftMenuLinkInActive\">HID Transfer Device Descriptor IDs</a></div>\n");
document.write("<div id=\"divID73791\" class=\"leftMenuInActive\"><a id=\"aID73791\" href=\"#\" OnMouseOver=\"link('_page','USBHIDTransfer0',this)\" class=\"leftMenuLinkInActive\">USB HID Transfer</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74240\" class=\"leftMenuInActive\"><a id=\"aID74240\" href=\"#\" OnMouseOver=\"link('_unit','HIDDTransferDriver_h0',this)\" class=\"leftMenuLinkInActive\">HIDDTransferDriver.h</a></div>\n");
document.write("<div id=\"divID74241\" class=\"leftMenuInActive\"><a id=\"aID74241\" href=\"#\" OnMouseOver=\"link('_unit','HIDDTransferDriverDesc_h0',this)\" class=\"leftMenuLinkInActive\">HIDDTransferDriverDesc.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
