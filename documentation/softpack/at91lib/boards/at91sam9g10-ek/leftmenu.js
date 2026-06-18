function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73848\" class=\"headerLeftMenuInActive\"><a id=\"aID73848\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g10mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9g10-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73847\" class=\"leftMenuInActive\"><a id=\"aID73847\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g10/at91sam9g100',this)\" class=\"leftMenuLinkInActive\">at91sam9g10</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73598\" class=\"leftMenuInActive\"><a id=\"aID73598\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73602\" class=\"leftMenuInActive\"><a id=\"aID73602\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - External components</a></div>\n");
document.write("<div id=\"divID73604\" class=\"leftMenuInActive\"><a id=\"aID73604\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73603\" class=\"leftMenuInActive\"><a id=\"aID73603\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - Memories</a></div>\n");
document.write("<div id=\"divID73599\" class=\"leftMenuInActive\"><a id=\"aID73599\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73601\" class=\"leftMenuInActive\"><a id=\"aID73601\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73600\" class=\"leftMenuInActive\"><a id=\"aID73600\" href=\"#\" OnMouseOver=\"link('_page','SAM9G10mi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9G10-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74083\" class=\"leftMenuInActive\"><a id=\"aID74083\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74084\" class=\"leftMenuInActive\"><a id=\"aID74084\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
