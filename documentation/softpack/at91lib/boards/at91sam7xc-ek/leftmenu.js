function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73840\" class=\"headerLeftMenuInActive\"><a id=\"aID73840\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7xcmi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7xc-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73839\" class=\"leftMenuInActive\"><a id=\"aID73839\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7xc128/at91sam7xc1280',this)\" class=\"leftMenuLinkInActive\">at91sam7xc128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73560\" class=\"leftMenuInActive\"><a id=\"aID73560\" href=\"#\" OnMouseOver=\"link('_page','SAM7XCmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM7XC-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73564\" class=\"leftMenuInActive\"><a id=\"aID73564\" href=\"#\" OnMouseOver=\"link('_page','SAM7XCmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM7XC-EK - External components</a></div>\n");
document.write("<div id=\"divID73565\" class=\"leftMenuInActive\"><a id=\"aID73565\" href=\"#\" OnMouseOver=\"link('_page','SAM7XCmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7XC-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73561\" class=\"leftMenuInActive\"><a id=\"aID73561\" href=\"#\" OnMouseOver=\"link('_page','SAM7XCmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM7XC-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73563\" class=\"leftMenuInActive\"><a id=\"aID73563\" href=\"#\" OnMouseOver=\"link('_page','SAM7XCmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM7XC-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73562\" class=\"leftMenuInActive\"><a id=\"aID73562\" href=\"#\" OnMouseOver=\"link('_page','SAM7XCmi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM7XC-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74068\" class=\"leftMenuInActive\"><a id=\"aID74068\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74069\" class=\"leftMenuInActive\"><a id=\"aID74069\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74070\" class=\"leftMenuInActive\"><a id=\"aID74070\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
document.write("<div id=\"divID74071\" class=\"leftMenuInActive\"><a id=\"aID74071\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
