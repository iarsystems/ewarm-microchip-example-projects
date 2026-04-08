function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73860\" class=\"headerLeftMenuInActive\"><a id=\"aID73860\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9xemi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9xe-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73859\" class=\"leftMenuInActive\"><a id=\"aID73859\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9xe128/at91sam9xe1280',this)\" class=\"leftMenuLinkInActive\">at91sam9xe128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73662\" class=\"leftMenuInActive\"><a id=\"aID73662\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_Oscillatoram_PLLParameters0',this)\" class=\"leftMenuLinkInActive\">SAM9XE - Oscillator & PLL Parameters</a></div>\n");
document.write("<div id=\"divID73655\" class=\"leftMenuInActive\"><a id=\"aID73655\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73659\" class=\"leftMenuInActive\"><a id=\"aID73659\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - External components</a></div>\n");
document.write("<div id=\"divID73661\" class=\"leftMenuInActive\"><a id=\"aID73661\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73660\" class=\"leftMenuInActive\"><a id=\"aID73660\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - Memories</a></div>\n");
document.write("<div id=\"divID73656\" class=\"leftMenuInActive\"><a id=\"aID73656\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73658\" class=\"leftMenuInActive\"><a id=\"aID73658\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73657\" class=\"leftMenuInActive\"><a id=\"aID73657\" href=\"#\" OnMouseOver=\"link('_page','SAM9XEmi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9XE-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74105\" class=\"leftMenuInActive\"><a id=\"aID74105\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74106\" class=\"leftMenuInActive\"><a id=\"aID74106\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
