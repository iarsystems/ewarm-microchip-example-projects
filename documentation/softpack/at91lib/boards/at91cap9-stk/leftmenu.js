function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73824\" class=\"headerLeftMenuInActive\"><a id=\"aID73824\" href=\"#\" OnMouseOver=\"link('_dir','at91cap9mi_stk0',this)\" class=\"leftMenuLinkHeadInActive\">at91cap9-stk</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73823\" class=\"leftMenuInActive\"><a id=\"aID73823\" href=\"#\" OnMouseOver=\"link('_dir','at91cap9/at91cap90',this)\" class=\"leftMenuLinkInActive\">at91cap9</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73493\" class=\"leftMenuInActive\"><a id=\"aID73493\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK - Board Description</a></div>\n");
document.write("<div id=\"divID73497\" class=\"leftMenuInActive\"><a id=\"aID73497\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK - External components</a></div>\n");
document.write("<div id=\"divID73498\" class=\"leftMenuInActive\"><a id=\"aID73498\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK - Memories</a></div>\n");
document.write("<div id=\"divID73494\" class=\"leftMenuInActive\"><a id=\"aID73494\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73496\" class=\"leftMenuInActive\"><a id=\"aID73496\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73495\" class=\"leftMenuInActive\"><a id=\"aID73495\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK - USB device</a></div>\n");
document.write("<div id=\"divID73499\" class=\"leftMenuInActive\"><a id=\"aID73499\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_STKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-STK- Individual chip definition</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74033\" class=\"leftMenuInActive\"><a id=\"aID74033\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74034\" class=\"leftMenuInActive\"><a id=\"aID74034\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
