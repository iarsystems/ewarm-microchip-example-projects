function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73846\" class=\"headerLeftMenuInActive\"><a id=\"aID73846\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9263mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9263-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73845\" class=\"leftMenuInActive\"><a id=\"aID73845\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9263/at91sam92630',this)\" class=\"leftMenuLinkInActive\">at91sam9263</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73588\" class=\"leftMenuInActive\"><a id=\"aID73588\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73592\" class=\"leftMenuInActive\"><a id=\"aID73592\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - External components</a></div>\n");
document.write("<div id=\"divID73594\" class=\"leftMenuInActive\"><a id=\"aID73594\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73593\" class=\"leftMenuInActive\"><a id=\"aID73593\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - Memories</a></div>\n");
document.write("<div id=\"divID73589\" class=\"leftMenuInActive\"><a id=\"aID73589\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73591\" class=\"leftMenuInActive\"><a id=\"aID73591\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73590\" class=\"leftMenuInActive\"><a id=\"aID73590\" href=\"#\" OnMouseOver=\"link('_page','SAM9263mi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM9263-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74080\" class=\"leftMenuInActive\"><a id=\"aID74080\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74081\" class=\"leftMenuInActive\"><a id=\"aID74081\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
