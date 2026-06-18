function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73936\" class=\"headerLeftMenuInActive\"><a id=\"aID73936\" href=\"#\" OnMouseOver=\"link('_dir','core0',this)\" class=\"leftMenuLinkHeadInActive\">core</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73774\" class=\"leftMenuInActive\"><a id=\"aID73774\" href=\"#\" OnMouseOver=\"link('_page','Endpointstates0',this)\" class=\"leftMenuLinkInActive\">Endpoint states</a></div>\n");
document.write("<div id=\"divID73773\" class=\"leftMenuInActive\"><a id=\"aID73773\" href=\"#\" OnMouseOver=\"link('_page','UDPregisterfieldvalues0',this)\" class=\"leftMenuLinkInActive\">UDP register field values</a></div>\n");
document.write("<div id=\"divID73775\" class=\"leftMenuInActive\"><a id=\"aID73775\" href=\"#\" OnMouseOver=\"link('_page','UDP_CSRregisteraccess0',this)\" class=\"leftMenuLinkInActive\">UDP_CSR register access</a></div>\n");
document.write("<div id=\"divID73770\" class=\"leftMenuInActive\"><a id=\"aID73770\" href=\"#\" OnMouseOver=\"link('_page','USBdeviceAPIreturnvalues0',this)\" class=\"leftMenuLinkInActive\">USB device API return values</a></div>\n");
document.write("<div id=\"divID73772\" class=\"leftMenuInActive\"><a id=\"aID73772\" href=\"#\" OnMouseOver=\"link('_page','USBdeviceLEDs0',this)\" class=\"leftMenuLinkInActive\">USB device LEDs</a></div>\n");
document.write("<div id=\"divID73771\" class=\"leftMenuInActive\"><a id=\"aID73771\" href=\"#\" OnMouseOver=\"link('_page','USBdevicestates0',this)\" class=\"leftMenuLinkInActive\">USB device states</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74226\" class=\"leftMenuInActive\"><a id=\"aID74226\" href=\"#\" OnMouseOver=\"link('_unit','USBD_h0',this)\" class=\"leftMenuLinkInActive\">USBD.h</a></div>\n");
document.write("<div id=\"divID74227\" class=\"leftMenuInActive\"><a id=\"aID74227\" href=\"#\" OnMouseOver=\"link('_unit','USBD_UDP_c0',this)\" class=\"leftMenuLinkInActive\">USBD_UDP.c</a></div>\n");
document.write("<div id=\"divID74228\" class=\"leftMenuInActive\"><a id=\"aID74228\" href=\"#\" OnMouseOver=\"link('_unit','USBDCallbacks_h0',this)\" class=\"leftMenuLinkInActive\">USBDCallbacks.h</a></div>\n");
document.write("<div id=\"divID74229\" class=\"leftMenuInActive\"><a id=\"aID74229\" href=\"#\" OnMouseOver=\"link('_unit','USBDDriver_h0',this)\" class=\"leftMenuLinkInActive\">USBDDriver.h</a></div>\n");
document.write("<div id=\"divID74230\" class=\"leftMenuInActive\"><a id=\"aID74230\" href=\"#\" OnMouseOver=\"link('_unit','USBDDriverCallbacks_h0',this)\" class=\"leftMenuLinkInActive\">USBDDriverCallbacks.h</a></div>\n");
document.write("<div id=\"divID74231\" class=\"leftMenuInActive\"><a id=\"aID74231\" href=\"#\" OnMouseOver=\"link('_unit','USBDDriverDescriptors_h0',this)\" class=\"leftMenuLinkInActive\">USBDDriverDescriptors.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
