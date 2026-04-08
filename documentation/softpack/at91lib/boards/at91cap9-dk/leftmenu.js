function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73822\" class=\"headerLeftMenuInActive\"><a id=\"aID73822\" href=\"#\" OnMouseOver=\"link('_dir','at91cap9mi_dk0',this)\" class=\"leftMenuLinkHeadInActive\">at91cap9-dk</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73821\" class=\"leftMenuInActive\"><a id=\"aID73821\" href=\"#\" OnMouseOver=\"link('_dir','at91cap9/at91cap90',this)\" class=\"leftMenuLinkInActive\">at91cap9</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73486\" class=\"leftMenuInActive\"><a id=\"aID73486\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - Board Description</a></div>\n");
document.write("<div id=\"divID73490\" class=\"leftMenuInActive\"><a id=\"aID73490\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - External components</a></div>\n");
document.write("<div id=\"divID73492\" class=\"leftMenuInActive\"><a id=\"aID73492\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73491\" class=\"leftMenuInActive\"><a id=\"aID73491\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - Memories</a></div>\n");
document.write("<div id=\"divID73487\" class=\"leftMenuInActive\"><a id=\"aID73487\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73489\" class=\"leftMenuInActive\"><a id=\"aID73489\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73488\" class=\"leftMenuInActive\"><a id=\"aID73488\" href=\"#\" OnMouseOver=\"link('_page','AT91CAP9mi_DKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">AT91CAP9-DK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74030\" class=\"leftMenuInActive\"><a id=\"aID74030\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74031\" class=\"leftMenuInActive\"><a id=\"aID74031\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
