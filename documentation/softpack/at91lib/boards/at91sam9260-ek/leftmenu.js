function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73842\" class=\"headerLeftMenuInActive\"><a id=\"aID73842\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9260mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9260-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73841\" class=\"leftMenuInActive\"><a id=\"aID73841\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9260/at91sam92600',this)\" class=\"leftMenuLinkInActive\">at91sam9260</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73569\" class=\"leftMenuInActive\"><a id=\"aID73569\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73573\" class=\"leftMenuInActive\"><a id=\"aID73573\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - External components</a></div>\n");
document.write("<div id=\"divID73575\" class=\"leftMenuInActive\"><a id=\"aID73575\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73574\" class=\"leftMenuInActive\"><a id=\"aID73574\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - Memories</a></div>\n");
document.write("<div id=\"divID73570\" class=\"leftMenuInActive\"><a id=\"aID73570\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73572\" class=\"leftMenuInActive\"><a id=\"aID73572\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73571\" class=\"leftMenuInActive\"><a id=\"aID73571\" href=\"#\" OnMouseOver=\"link('_page','SAM9260mi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9260-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74073\" class=\"leftMenuInActive\"><a id=\"aID74073\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74074\" class=\"leftMenuInActive\"><a id=\"aID74074\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74075\" class=\"leftMenuInActive\"><a id=\"aID74075\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
