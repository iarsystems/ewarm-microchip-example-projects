function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73828\" class=\"headerLeftMenuInActive\"><a id=\"aID73828\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7a3mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7a3-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73827\" class=\"leftMenuInActive\"><a id=\"aID73827\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7a3/at91sam7a30',this)\" class=\"leftMenuLinkInActive\">at91sam7a3</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73513\" class=\"leftMenuInActive\"><a id=\"aID73513\" href=\"#\" OnMouseOver=\"link('_page','SAM7A3mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM7A3-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73517\" class=\"leftMenuInActive\"><a id=\"aID73517\" href=\"#\" OnMouseOver=\"link('_page','SAM7A3mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM7A3-EK - External components</a></div>\n");
document.write("<div id=\"divID73518\" class=\"leftMenuInActive\"><a id=\"aID73518\" href=\"#\" OnMouseOver=\"link('_page','SAM7A3mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7A3-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73514\" class=\"leftMenuInActive\"><a id=\"aID73514\" href=\"#\" OnMouseOver=\"link('_page','SAM7A3mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM7A3-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73516\" class=\"leftMenuInActive\"><a id=\"aID73516\" href=\"#\" OnMouseOver=\"link('_page','SAM7A3mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM7A3-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73515\" class=\"leftMenuInActive\"><a id=\"aID73515\" href=\"#\" OnMouseOver=\"link('_page','SAM7A3mi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM7A3-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74041\" class=\"leftMenuInActive\"><a id=\"aID74041\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74042\" class=\"leftMenuInActive\"><a id=\"aID74042\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74043\" class=\"leftMenuInActive\"><a id=\"aID74043\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
document.write("<div id=\"divID74044\" class=\"leftMenuInActive\"><a id=\"aID74044\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
