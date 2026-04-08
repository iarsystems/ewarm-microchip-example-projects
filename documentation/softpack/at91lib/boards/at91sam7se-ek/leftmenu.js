function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73836\" class=\"headerLeftMenuInActive\"><a id=\"aID73836\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7semi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7se-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73835\" class=\"leftMenuInActive\"><a id=\"aID73835\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7se256/at91sam7se2560',this)\" class=\"leftMenuLinkInActive\">at91sam7se256</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73541\" class=\"leftMenuInActive\"><a id=\"aID73541\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73545\" class=\"leftMenuInActive\"><a id=\"aID73545\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - External components</a></div>\n");
document.write("<div id=\"divID73547\" class=\"leftMenuInActive\"><a id=\"aID73547\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73546\" class=\"leftMenuInActive\"><a id=\"aID73546\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - Memories</a></div>\n");
document.write("<div id=\"divID73542\" class=\"leftMenuInActive\"><a id=\"aID73542\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73544\" class=\"leftMenuInActive\"><a id=\"aID73544\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73543\" class=\"leftMenuInActive\"><a id=\"aID73543\" href=\"#\" OnMouseOver=\"link('_page','SAM7SEmi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM7SE-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74059\" class=\"leftMenuInActive\"><a id=\"aID74059\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74060\" class=\"leftMenuInActive\"><a id=\"aID74060\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74061\" class=\"leftMenuInActive\"><a id=\"aID74061\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
