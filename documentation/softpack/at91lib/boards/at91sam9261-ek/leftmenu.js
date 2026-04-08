function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73844\" class=\"headerLeftMenuInActive\"><a id=\"aID73844\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9261mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9261-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73843\" class=\"leftMenuInActive\"><a id=\"aID73843\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9261/at91sam92610',this)\" class=\"leftMenuLinkInActive\">at91sam9261</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73579\" class=\"leftMenuInActive\"><a id=\"aID73579\" href=\"#\" OnMouseOver=\"link('_page','SAM9261mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9261-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73582\" class=\"leftMenuInActive\"><a id=\"aID73582\" href=\"#\" OnMouseOver=\"link('_page','SAM9261mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9261-EK - External components</a></div>\n");
document.write("<div id=\"divID73584\" class=\"leftMenuInActive\"><a id=\"aID73584\" href=\"#\" OnMouseOver=\"link('_page','SAM9261mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9261-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73583\" class=\"leftMenuInActive\"><a id=\"aID73583\" href=\"#\" OnMouseOver=\"link('_page','SAM9261mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9261-EK - Memories</a></div>\n");
document.write("<div id=\"divID73581\" class=\"leftMenuInActive\"><a id=\"aID73581\" href=\"#\" OnMouseOver=\"link('_page','SAM9261mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9261-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73580\" class=\"leftMenuInActive\"><a id=\"aID73580\" href=\"#\" OnMouseOver=\"link('_page','SAM9261mi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9261-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74077\" class=\"leftMenuInActive\"><a id=\"aID74077\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74078\" class=\"leftMenuInActive\"><a id=\"aID74078\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
