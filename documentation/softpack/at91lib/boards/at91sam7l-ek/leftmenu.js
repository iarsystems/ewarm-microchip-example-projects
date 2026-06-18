function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73830\" class=\"headerLeftMenuInActive\"><a id=\"aID73830\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7lmi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7l-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73829\" class=\"leftMenuInActive\"><a id=\"aID73829\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7l128/at91sam7l1280',this)\" class=\"leftMenuLinkInActive\">at91sam7l128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73522\" class=\"leftMenuInActive\"><a id=\"aID73522\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73526\" class=\"leftMenuInActive\"><a id=\"aID73526\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK - External components</a></div>\n");
document.write("<div id=\"divID73527\" class=\"leftMenuInActive\"><a id=\"aID73527\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK - Individual chip definition</a></div>\n");
document.write("<div id=\"divID73525\" class=\"leftMenuInActive\"><a id=\"aID73525\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK - Memories</a></div>\n");
document.write("<div id=\"divID73523\" class=\"leftMenuInActive\"><a id=\"aID73523\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73524\" class=\"leftMenuInActive\"><a id=\"aID73524\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK - PIO definitions</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74046\" class=\"leftMenuInActive\"><a id=\"aID74046\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74047\" class=\"leftMenuInActive\"><a id=\"aID74047\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74048\" class=\"leftMenuInActive\"><a id=\"aID74048\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
document.write("<div id=\"divID74049\" class=\"leftMenuInActive\"><a id=\"aID74049\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
