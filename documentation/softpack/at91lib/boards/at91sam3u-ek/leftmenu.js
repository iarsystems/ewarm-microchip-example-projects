function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73826\" class=\"headerLeftMenuInActive\"><a id=\"aID73826\" href=\"#\" OnMouseOver=\"link('_dir','at91sam3umi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam3u-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73825\" class=\"leftMenuInActive\"><a id=\"aID73825\" href=\"#\" OnMouseOver=\"link('_dir','at91sam3u4/at91sam3u40',this)\" class=\"leftMenuLinkInActive\">at91sam3u4</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73503\" class=\"leftMenuInActive\"><a id=\"aID73503\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73507\" class=\"leftMenuInActive\"><a id=\"aID73507\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - External components</a></div>\n");
document.write("<div id=\"divID73509\" class=\"leftMenuInActive\"><a id=\"aID73509\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73508\" class=\"leftMenuInActive\"><a id=\"aID73508\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - Memories</a></div>\n");
document.write("<div id=\"divID73504\" class=\"leftMenuInActive\"><a id=\"aID73504\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73506\" class=\"leftMenuInActive\"><a id=\"aID73506\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73505\" class=\"leftMenuInActive\"><a id=\"aID73505\" href=\"#\" OnMouseOver=\"link('_page','SAM3UEmi_EKmi_USBdevice0',this)\" class=\"leftMenuLinkInActive\">SAM3UE-EK - USB device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74036\" class=\"leftMenuInActive\"><a id=\"aID74036\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74037\" class=\"leftMenuInActive\"><a id=\"aID74037\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74038\" class=\"leftMenuInActive\"><a id=\"aID74038\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
document.write("<div id=\"divID74039\" class=\"leftMenuInActive\"><a id=\"aID74039\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
