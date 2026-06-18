function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73838\" class=\"headerLeftMenuInActive\"><a id=\"aID73838\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7xmi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7x-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73837\" class=\"leftMenuInActive\"><a id=\"aID73837\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7x128/at91sam7x1280',this)\" class=\"leftMenuLinkInActive\">at91sam7x128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73551\" class=\"leftMenuInActive\"><a id=\"aID73551\" href=\"#\" OnMouseOver=\"link('_page','SAM7Xmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM7X-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73555\" class=\"leftMenuInActive\"><a id=\"aID73555\" href=\"#\" OnMouseOver=\"link('_page','SAM7Xmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM7X-EK - External components</a></div>\n");
document.write("<div id=\"divID73556\" class=\"leftMenuInActive\"><a id=\"aID73556\" href=\"#\" OnMouseOver=\"link('_page','SAM7Xmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7X-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73552\" class=\"leftMenuInActive\"><a id=\"aID73552\" href=\"#\" OnMouseOver=\"link('_page','SAM7Xmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM7X-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73554\" class=\"leftMenuInActive\"><a id=\"aID73554\" href=\"#\" OnMouseOver=\"link('_page','SAM7Xmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM7X-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73553\" class=\"leftMenuInActive\"><a id=\"aID73553\" href=\"#\" OnMouseOver=\"link('_page','SAM7Xmi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM7X-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74063\" class=\"leftMenuInActive\"><a id=\"aID74063\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74064\" class=\"leftMenuInActive\"><a id=\"aID74064\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74065\" class=\"leftMenuInActive\"><a id=\"aID74065\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
document.write("<div id=\"divID74066\" class=\"leftMenuInActive\"><a id=\"aID74066\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
