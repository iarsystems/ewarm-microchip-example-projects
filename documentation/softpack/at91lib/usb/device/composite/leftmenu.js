function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73934\" class=\"headerLeftMenuInActive\"><a id=\"aID73934\" href=\"#\" OnMouseOver=\"link('_dir','composite0',this)\" class=\"leftMenuLinkHeadInActive\">composite</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73935\" class=\"leftMenuInActive\"><a id=\"aID73935\" href=\"#\" OnMouseOver=\"link('_dir','drv/drv0',this)\" class=\"leftMenuLinkInActive\">drv</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73769\" class=\"leftMenuInActive\"><a id=\"aID73769\" href=\"#\" OnMouseOver=\"link('_page','MSinterfaceam_endpointdescriptorsettings0',this)\" class=\"leftMenuLinkInActive\">MS interface & endpoint descriptor settings</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74220\" class=\"leftMenuInActive\"><a id=\"aID74220\" href=\"#\" OnMouseOver=\"link('_unit','CDCHIDDDriver_h0',this)\" class=\"leftMenuLinkInActive\">CDCHIDDDriver.h</a></div>\n");
document.write("<div id=\"divID74221\" class=\"leftMenuInActive\"><a id=\"aID74221\" href=\"#\" OnMouseOver=\"link('_unit','CDCMSDDDriver_h0',this)\" class=\"leftMenuLinkInActive\">CDCMSDDDriver.h</a></div>\n");
document.write("<div id=\"divID74222\" class=\"leftMenuInActive\"><a id=\"aID74222\" href=\"#\" OnMouseOver=\"link('_unit','COMPOSITEDDriver_h0',this)\" class=\"leftMenuLinkInActive\">COMPOSITEDDriver.h</a></div>\n");
document.write("<div id=\"divID74223\" class=\"leftMenuInActive\"><a id=\"aID74223\" href=\"#\" OnMouseOver=\"link('_unit','DUALCDCDDriver_h0',this)\" class=\"leftMenuLinkInActive\">DUALCDCDDriver.h</a></div>\n");
document.write("<div id=\"divID74224\" class=\"leftMenuInActive\"><a id=\"aID74224\" href=\"#\" OnMouseOver=\"link('_unit','HIDMSDDDriver_h0',this)\" class=\"leftMenuLinkInActive\">HIDMSDDDriver.h</a></div>\n");
document.write("<div id=\"divID74225\" class=\"leftMenuInActive\"><a id=\"aID74225\" href=\"#\" OnMouseOver=\"link('_unit','MSDDFunctionDriver_h0',this)\" class=\"leftMenuLinkInActive\">MSDDFunctionDriver.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
