function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73932\" class=\"headerLeftMenuInActive\"><a id=\"aID73932\" href=\"#\" OnMouseOver=\"link('_dir','cdcmi_serial0',this)\" class=\"leftMenuLinkHeadInActive\">cdc-serial</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73933\" class=\"leftMenuInActive\"><a id=\"aID73933\" href=\"#\" OnMouseOver=\"link('_dir','drv/drv0',this)\" class=\"leftMenuLinkInActive\">drv</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73767\" class=\"leftMenuInActive\"><a id=\"aID73767\" href=\"#\" OnMouseOver=\"link('_page','CDCSerialDeviceIDs0',this)\" class=\"leftMenuLinkInActive\">CDC Serial Device IDs</a></div>\n");
document.write("<div id=\"divID73768\" class=\"leftMenuInActive\"><a id=\"aID73768\" href=\"#\" OnMouseOver=\"link('_page','CDCSerialEndpoints0',this)\" class=\"leftMenuLinkInActive\">CDC Serial Endpoints</a></div>\n");
document.write("<div id=\"divID73766\" class=\"leftMenuInActive\"><a id=\"aID73766\" href=\"#\" OnMouseOver=\"link('_page','CDCSerialPortStates0',this)\" class=\"leftMenuLinkInActive\">CDC Serial Port States</a></div>\n");
document.write("<div id=\"divID73763\" class=\"leftMenuInActive\"><a id=\"aID73763\" href=\"#\" OnMouseOver=\"link('_page','USBCDCSerialDevice0',this)\" class=\"leftMenuLinkInActive\">USB CDC Serial Device</a></div>\n");
document.write("<div id=\"divID73764\" class=\"leftMenuInActive\"><a id=\"aID73764\" href=\"#\" OnMouseOver=\"link('_page','USBCDCSerialHostDriver0',this)\" class=\"leftMenuLinkInActive\">USB CDC Serial Host Driver</a></div>\n");
document.write("<div id=\"divID73765\" class=\"leftMenuInActive\"><a id=\"aID73765\" href=\"#\" OnMouseOver=\"link('_page','USBDualPortCDCSerialDevice0',this)\" class=\"leftMenuLinkInActive\">USB Dual Port CDC Serial Device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74218\" class=\"leftMenuInActive\"><a id=\"aID74218\" href=\"#\" OnMouseOver=\"link('_unit','CDCDSerialDriver_h0',this)\" class=\"leftMenuLinkInActive\">CDCDSerialDriver.h</a></div>\n");
document.write("<div id=\"divID74219\" class=\"leftMenuInActive\"><a id=\"aID74219\" href=\"#\" OnMouseOver=\"link('_unit','CDCDSerialDriverDescriptors_h0',this)\" class=\"leftMenuLinkInActive\">CDCDSerialDriverDescriptors.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
