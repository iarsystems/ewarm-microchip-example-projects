function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73854\" class=\"headerLeftMenuInActive\"><a id=\"aID73854\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m10mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9m10-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73853\" class=\"leftMenuInActive\"><a id=\"aID73853\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m10/at91sam9m100',this)\" class=\"leftMenuLinkInActive\">at91sam9m10</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73628\" class=\"leftMenuInActive\"><a id=\"aID73628\" href=\"#\" OnMouseOver=\"link('_page','SAM9M10mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9M10-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73631\" class=\"leftMenuInActive\"><a id=\"aID73631\" href=\"#\" OnMouseOver=\"link('_page','SAM9M10mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9M10-EK - External components</a></div>\n");
document.write("<div id=\"divID73632\" class=\"leftMenuInActive\"><a id=\"aID73632\" href=\"#\" OnMouseOver=\"link('_page','SAM9M10mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9M10-EK - Memories</a></div>\n");
document.write("<div id=\"divID73629\" class=\"leftMenuInActive\"><a id=\"aID73629\" href=\"#\" OnMouseOver=\"link('_page','SAM9M10mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9M10-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73630\" class=\"leftMenuInActive\"><a id=\"aID73630\" href=\"#\" OnMouseOver=\"link('_page','SAM9M10mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9M10-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73633\" class=\"leftMenuInActive\"><a id=\"aID73633\" href=\"#\" OnMouseOver=\"link('_page','SAM9M10mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9M10-EK- Individual chip definition</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74094\" class=\"leftMenuInActive\"><a id=\"aID74094\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74095\" class=\"leftMenuInActive\"><a id=\"aID74095\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74096\" class=\"leftMenuInActive\"><a id=\"aID74096\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
