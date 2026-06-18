function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73938\" class=\"headerLeftMenuInActive\"><a id=\"aID73938\" href=\"#\" OnMouseOver=\"link('_dir','hidmi_keyboard0',this)\" class=\"leftMenuLinkHeadInActive\">hid-keyboard</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73786\" class=\"leftMenuInActive\"><a id=\"aID73786\" href=\"#\" OnMouseOver=\"link('_page','HIDDeviceDescriptorIDs0',this)\" class=\"leftMenuLinkInActive\">HID Device Descriptor IDs</a></div>\n");
document.write("<div id=\"divID73787\" class=\"leftMenuInActive\"><a id=\"aID73787\" href=\"#\" OnMouseOver=\"link('_page','HIDEndpoints0',this)\" class=\"leftMenuLinkInActive\">HID Endpoints</a></div>\n");
document.write("<div id=\"divID73788\" class=\"leftMenuInActive\"><a id=\"aID73788\" href=\"#\" OnMouseOver=\"link('_page','HIDKeypadkeys0',this)\" class=\"leftMenuLinkInActive\">HID Keypad keys</a></div>\n");
document.write("<div id=\"divID73785\" class=\"leftMenuInActive\"><a id=\"aID73785\" href=\"#\" OnMouseOver=\"link('_page','USBHIDBasic0',this)\" class=\"leftMenuLinkInActive\">USB HID Basic</a></div>\n");
document.write("<div id=\"divID73783\" class=\"leftMenuInActive\"><a id=\"aID73783\" href=\"#\" OnMouseOver=\"link('_page','USBHIDKeyboard0',this)\" class=\"leftMenuLinkInActive\">USB HID Keyboard</a></div>\n");
document.write("<div id=\"divID73784\" class=\"leftMenuInActive\"><a id=\"aID73784\" href=\"#\" OnMouseOver=\"link('_page','USBHIDMouse0',this)\" class=\"leftMenuLinkInActive\">USB HID Mouse</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74232\" class=\"leftMenuInActive\"><a id=\"aID74232\" href=\"#\" OnMouseOver=\"link('_unit','HIDDKeyboardCallbacks_h0',this)\" class=\"leftMenuLinkInActive\">HIDDKeyboardCallbacks.h</a></div>\n");
document.write("<div id=\"divID74233\" class=\"leftMenuInActive\"><a id=\"aID74233\" href=\"#\" OnMouseOver=\"link('_unit','HIDDKeyboardDriver_h0',this)\" class=\"leftMenuLinkInActive\">HIDDKeyboardDriver.h</a></div>\n");
document.write("<div id=\"divID74234\" class=\"leftMenuInActive\"><a id=\"aID74234\" href=\"#\" OnMouseOver=\"link('_unit','HIDDKeyboardDriverDescriptors_h0',this)\" class=\"leftMenuLinkInActive\">HIDDKeyboardDriverDescriptors.h</a></div>\n");
document.write("<div id=\"divID74235\" class=\"leftMenuInActive\"><a id=\"aID74235\" href=\"#\" OnMouseOver=\"link('_unit','HIDDKeyboardInputReport_h0',this)\" class=\"leftMenuLinkInActive\">HIDDKeyboardInputReport.h</a></div>\n");
document.write("<div id=\"divID74236\" class=\"leftMenuInActive\"><a id=\"aID74236\" href=\"#\" OnMouseOver=\"link('_unit','HIDDKeyboardOutputReport_h0',this)\" class=\"leftMenuLinkInActive\">HIDDKeyboardOutputReport.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
