function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73850\" class=\"headerLeftMenuInActive\"><a id=\"aID73850\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g20mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9g20-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73849\" class=\"leftMenuInActive\"><a id=\"aID73849\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g20/at91sam9g200',this)\" class=\"leftMenuLinkInActive\">at91sam9g20</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73608\" class=\"leftMenuInActive\"><a id=\"aID73608\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73612\" class=\"leftMenuInActive\"><a id=\"aID73612\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - External components</a></div>\n");
document.write("<div id=\"divID73614\" class=\"leftMenuInActive\"><a id=\"aID73614\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73613\" class=\"leftMenuInActive\"><a id=\"aID73613\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - Memories</a></div>\n");
document.write("<div id=\"divID73609\" class=\"leftMenuInActive\"><a id=\"aID73609\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73611\" class=\"leftMenuInActive\"><a id=\"aID73611\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73610\" class=\"leftMenuInActive\"><a id=\"aID73610\" href=\"#\" OnMouseOver=\"link('_page','SAM9G20mi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9G20-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74086\" class=\"leftMenuInActive\"><a id=\"aID74086\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74087\" class=\"leftMenuInActive\"><a id=\"aID74087\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74088\" class=\"leftMenuInActive\"><a id=\"aID74088\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
