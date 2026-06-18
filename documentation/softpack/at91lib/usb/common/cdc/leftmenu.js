function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73925\" class=\"headerLeftMenuInActive\"><a id=\"aID73925\" href=\"#\" OnMouseOver=\"link('_dir','cdc0',this)\" class=\"leftMenuLinkHeadInActive\">cdc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73702\" class=\"leftMenuInActive\"><a id=\"aID73702\" href=\"#\" OnMouseOver=\"link('_page','CDCACMCapabilities0',this)\" class=\"leftMenuLinkInActive\">CDC ACM Capabilities</a></div>\n");
document.write("<div id=\"divID73703\" class=\"leftMenuInActive\"><a id=\"aID73703\" href=\"#\" OnMouseOver=\"link('_page','CDCCallManagementCapabilities0',this)\" class=\"leftMenuLinkInActive\">CDC CallManagement Capabilities</a></div>\n");
document.write("<div id=\"divID73704\" class=\"leftMenuInActive\"><a id=\"aID73704\" href=\"#\" OnMouseOver=\"link('_page','CDCCommunicationInterfaceValues0',this)\" class=\"leftMenuLinkInActive\">CDC Communication Interface Values</a></div>\n");
document.write("<div id=\"divID73705\" class=\"leftMenuInActive\"><a id=\"aID73705\" href=\"#\" OnMouseOver=\"link('_page','CDCDataInterfaceValues0',this)\" class=\"leftMenuLinkInActive\">CDC Data Interface Values</a></div>\n");
document.write("<div id=\"divID73709\" class=\"leftMenuInActive\"><a id=\"aID73709\" href=\"#\" OnMouseOver=\"link('_page','CDCDescriptorSubtypes0',this)\" class=\"leftMenuLinkInActive\">CDC Descriptor Subtypes</a></div>\n");
document.write("<div id=\"divID73708\" class=\"leftMenuInActive\"><a id=\"aID73708\" href=\"#\" OnMouseOver=\"link('_page','CDCDescriptroTypes0',this)\" class=\"leftMenuLinkInActive\">CDC Descriptro Types</a></div>\n");
document.write("<div id=\"divID73706\" class=\"leftMenuInActive\"><a id=\"aID73706\" href=\"#\" OnMouseOver=\"link('_page','CDCDeviceDescriptorValues0',this)\" class=\"leftMenuLinkInActive\">CDC Device Descriptor Values</a></div>\n");
document.write("<div id=\"divID73712\" class=\"leftMenuInActive\"><a id=\"aID73712\" href=\"#\" OnMouseOver=\"link('_page','CDCLineCodingParityCheckings0',this)\" class=\"leftMenuLinkInActive\">CDC LineCoding ParityCheckings</a></div>\n");
document.write("<div id=\"divID73711\" class=\"leftMenuInActive\"><a id=\"aID73711\" href=\"#\" OnMouseOver=\"link('_page','CDCLineCodingStopBits0',this)\" class=\"leftMenuLinkInActive\">CDC LineCoding StopBits</a></div>\n");
document.write("<div id=\"divID73710\" class=\"leftMenuInActive\"><a id=\"aID73710\" href=\"#\" OnMouseOver=\"link('_page','CDCRequestCodes0',this)\" class=\"leftMenuLinkInActive\">CDC Request Codes</a></div>\n");
document.write("<div id=\"divID73707\" class=\"leftMenuInActive\"><a id=\"aID73707\" href=\"#\" OnMouseOver=\"link('_page','CDCSpecificationReleaseNumbers0',this)\" class=\"leftMenuLinkInActive\">CDC Specification Release Numbers</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74177\" class=\"leftMenuInActive\"><a id=\"aID74177\" href=\"#\" OnMouseOver=\"link('_unit','CDCAbstractControlManagementDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCAbstractControlManagementDescriptor.h</a></div>\n");
document.write("<div id=\"divID74178\" class=\"leftMenuInActive\"><a id=\"aID74178\" href=\"#\" OnMouseOver=\"link('_unit','CDCCallManagementDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCCallManagementDescriptor.h</a></div>\n");
document.write("<div id=\"divID74179\" class=\"leftMenuInActive\"><a id=\"aID74179\" href=\"#\" OnMouseOver=\"link('_unit','CDCCommunicationInterfaceDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCCommunicationInterfaceDescriptor.h</a></div>\n");
document.write("<div id=\"divID74180\" class=\"leftMenuInActive\"><a id=\"aID74180\" href=\"#\" OnMouseOver=\"link('_unit','CDCDataInterfaceDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCDataInterfaceDescriptor.h</a></div>\n");
document.write("<div id=\"divID74181\" class=\"leftMenuInActive\"><a id=\"aID74181\" href=\"#\" OnMouseOver=\"link('_unit','CDCDeviceDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCDeviceDescriptor.h</a></div>\n");
document.write("<div id=\"divID74182\" class=\"leftMenuInActive\"><a id=\"aID74182\" href=\"#\" OnMouseOver=\"link('_unit','CDCGenericDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCGenericDescriptor.h</a></div>\n");
document.write("<div id=\"divID74183\" class=\"leftMenuInActive\"><a id=\"aID74183\" href=\"#\" OnMouseOver=\"link('_unit','CDCGenericRequest_h0',this)\" class=\"leftMenuLinkInActive\">CDCGenericRequest.h</a></div>\n");
document.write("<div id=\"divID74184\" class=\"leftMenuInActive\"><a id=\"aID74184\" href=\"#\" OnMouseOver=\"link('_unit','CDCHeaderDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCHeaderDescriptor.h</a></div>\n");
document.write("<div id=\"divID74185\" class=\"leftMenuInActive\"><a id=\"aID74185\" href=\"#\" OnMouseOver=\"link('_unit','CDCLineCoding_h0',this)\" class=\"leftMenuLinkInActive\">CDCLineCoding.h</a></div>\n");
document.write("<div id=\"divID74186\" class=\"leftMenuInActive\"><a id=\"aID74186\" href=\"#\" OnMouseOver=\"link('_unit','CDCSetControlLineStateRequest_h0',this)\" class=\"leftMenuLinkInActive\">CDCSetControlLineStateRequest.h</a></div>\n");
document.write("<div id=\"divID74187\" class=\"leftMenuInActive\"><a id=\"aID74187\" href=\"#\" OnMouseOver=\"link('_unit','CDCUnionDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">CDCUnionDescriptor.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
