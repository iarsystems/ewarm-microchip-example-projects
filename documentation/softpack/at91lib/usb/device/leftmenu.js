function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73937\" class=\"headerLeftMenuInActive\"><a id=\"aID73937\" href=\"#\" OnMouseOver=\"link('_dir','device0',this)\" class=\"leftMenuLinkHeadInActive\">device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73930\" class=\"leftMenuInActive\"><a id=\"aID73930\" href=\"#\" OnMouseOver=\"link('_dir','audio-speaker/audiomi_speaker0',this)\" class=\"leftMenuLinkInActive\">audio-speaker</a></div>\n");
document.write("<div id=\"divID73931\" class=\"leftMenuInActive\"><a id=\"aID73931\" href=\"#\" OnMouseOver=\"link('_dir','ccid/ccid0',this)\" class=\"leftMenuLinkInActive\">ccid</a></div>\n");
document.write("<div id=\"divID73932\" class=\"leftMenuInActive\"><a id=\"aID73932\" href=\"#\" OnMouseOver=\"link('_dir','cdc-serial/cdcmi_serial0',this)\" class=\"leftMenuLinkInActive\">cdc-serial</a></div>\n");
document.write("<div id=\"divID73934\" class=\"leftMenuInActive\"><a id=\"aID73934\" href=\"#\" OnMouseOver=\"link('_dir','composite/composite0',this)\" class=\"leftMenuLinkInActive\">composite</a></div>\n");
document.write("<div id=\"divID73936\" class=\"leftMenuInActive\"><a id=\"aID73936\" href=\"#\" OnMouseOver=\"link('_dir','core/core0',this)\" class=\"leftMenuLinkInActive\">core</a></div>\n");
document.write("<div id=\"divID73938\" class=\"leftMenuInActive\"><a id=\"aID73938\" href=\"#\" OnMouseOver=\"link('_dir','hid-keyboard/hidmi_keyboard0',this)\" class=\"leftMenuLinkInActive\">hid-keyboard</a></div>\n");
document.write("<div id=\"divID73939\" class=\"leftMenuInActive\"><a id=\"aID73939\" href=\"#\" OnMouseOver=\"link('_dir','hid-mouse/hidmi_mouse0',this)\" class=\"leftMenuLinkInActive\">hid-mouse</a></div>\n");
document.write("<div id=\"divID73940\" class=\"leftMenuInActive\"><a id=\"aID73940\" href=\"#\" OnMouseOver=\"link('_dir','hid-transfer/hidmi_transfer0',this)\" class=\"leftMenuLinkInActive\">hid-transfer</a></div>\n");
document.write("<div id=\"divID73941\" class=\"leftMenuInActive\"><a id=\"aID73941\" href=\"#\" OnMouseOver=\"link('_dir','massstorage/massstorage0',this)\" class=\"leftMenuLinkInActive\">massstorage</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73779\" class=\"leftMenuInActive\"><a id=\"aID73779\" href=\"#\" OnMouseOver=\"link('_page','USBDeviceStateDiagram0',this)\" class=\"leftMenuLinkInActive\">USB Device State Diagram</a></div>\n");
document.write("<div id=\"divID73776\" class=\"leftMenuInActive\"><a id=\"aID73776\" href=\"#\" OnMouseOver=\"link('_page','USBDAPI0',this)\" class=\"leftMenuLinkInActive\">USBD API</a></div>\n");
document.write("<div id=\"divID73778\" class=\"leftMenuInActive\"><a id=\"aID73778\" href=\"#\" OnMouseOver=\"link('_page','USBDAPIMethods0',this)\" class=\"leftMenuLinkInActive\">USBD API Methods</a></div>\n");
document.write("<div id=\"divID73777\" class=\"leftMenuInActive\"><a id=\"aID73777\" href=\"#\" OnMouseOver=\"link('_page','USBDAPIStructures0',this)\" class=\"leftMenuLinkInActive\">USBD API Structures</a></div>\n");
document.write("<div id=\"divID73780\" class=\"leftMenuInActive\"><a id=\"aID73780\" href=\"#\" OnMouseOver=\"link('_page','USBDCallbackAPI0',this)\" class=\"leftMenuLinkInActive\">USBD Callback API</a></div>\n");
document.write("<div id=\"divID73781\" class=\"leftMenuInActive\"><a id=\"aID73781\" href=\"#\" OnMouseOver=\"link('_page','USBDStandardRequestHandler0',this)\" class=\"leftMenuLinkInActive\">USBD Standard Request Handler</a></div>\n");
document.write("<div id=\"divID73782\" class=\"leftMenuInActive\"><a id=\"aID73782\" href=\"#\" OnMouseOver=\"link('_page','VIDco_PIDco_SNam_Strings0',this)\" class=\"leftMenuLinkInActive\">VID, PID, SN & Strings</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
