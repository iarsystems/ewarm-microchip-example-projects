function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73852\" class=\"headerLeftMenuInActive\"><a id=\"aID73852\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g45mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9g45-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73851\" class=\"leftMenuInActive\"><a id=\"aID73851\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g45/at91sam9g450',this)\" class=\"leftMenuLinkInActive\">at91sam9g45</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73618\" class=\"leftMenuInActive\"><a id=\"aID73618\" href=\"#\" OnMouseOver=\"link('_page','SAM9G45mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9G45-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73622\" class=\"leftMenuInActive\"><a id=\"aID73622\" href=\"#\" OnMouseOver=\"link('_page','SAM9G45mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9G45-EK - External components</a></div>\n");
document.write("<div id=\"divID73623\" class=\"leftMenuInActive\"><a id=\"aID73623\" href=\"#\" OnMouseOver=\"link('_page','SAM9G45mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9G45-EK - Memories</a></div>\n");
document.write("<div id=\"divID73619\" class=\"leftMenuInActive\"><a id=\"aID73619\" href=\"#\" OnMouseOver=\"link('_page','SAM9G45mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9G45-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73621\" class=\"leftMenuInActive\"><a id=\"aID73621\" href=\"#\" OnMouseOver=\"link('_page','SAM9G45mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9G45-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73624\" class=\"leftMenuInActive\"><a id=\"aID73624\" href=\"#\" OnMouseOver=\"link('_page','SAM9G45mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9G45-EK- Individual chip definition</a></div>\n");
document.write("<div id=\"divID73620\" class=\"leftMenuInActive\"><a id=\"aID73620\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74090\" class=\"leftMenuInActive\"><a id=\"aID74090\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74091\" class=\"leftMenuInActive\"><a id=\"aID74091\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74092\" class=\"leftMenuInActive\"><a id=\"aID74092\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
