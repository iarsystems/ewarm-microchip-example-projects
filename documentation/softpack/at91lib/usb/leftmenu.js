function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73942\" class=\"headerLeftMenuInActive\"><a id=\"aID73942\" href=\"#\" OnMouseOver=\"link('_dir','usb0',this)\" class=\"leftMenuLinkHeadInActive\">usb</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73926\" class=\"leftMenuInActive\"><a id=\"aID73926\" href=\"#\" OnMouseOver=\"link('_dir','common/common0',this)\" class=\"leftMenuLinkInActive\">common</a></div>\n");
document.write("<div id=\"divID73937\" class=\"leftMenuInActive\"><a id=\"aID73937\" href=\"#\" OnMouseOver=\"link('_dir','device/device0',this)\" class=\"leftMenuLinkInActive\">device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73814\" class=\"leftMenuInActive\"><a id=\"aID73814\" href=\"#\" OnMouseOver=\"link('_page','AT91USBdeviceframework0',this)\" class=\"leftMenuLinkInActive\">AT91 USB device framework</a></div>\n");
document.write("<div id=\"divID73818\" class=\"leftMenuInActive\"><a id=\"aID73818\" href=\"#\" OnMouseOver=\"link('_page','USBDeviceEnumeration0',this)\" class=\"leftMenuLinkInActive\">USB Device Enumeration</a></div>\n");
document.write("<div id=\"divID73815\" class=\"leftMenuInActive\"><a id=\"aID73815\" href=\"#\" OnMouseOver=\"link('_page','USBDeviceFrameworkArchitecture0',this)\" class=\"leftMenuLinkInActive\">USB Device Framework Architecture</a></div>\n");
document.write("<div id=\"divID73816\" class=\"leftMenuInActive\"><a id=\"aID73816\" href=\"#\" OnMouseOver=\"link('_page','USBDeviceFrameworkDescription0',this)\" class=\"leftMenuLinkInActive\">USB Device Framework Description</a></div>\n");
document.write("<div id=\"divID73817\" class=\"leftMenuInActive\"><a id=\"aID73817\" href=\"#\" OnMouseOver=\"link('_page','USBDeviceFrameworkUsage0',this)\" class=\"leftMenuLinkInActive\">USB Device Framework Usage</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
