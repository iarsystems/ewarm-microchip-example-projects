function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73856\" class=\"headerLeftMenuInActive\"><a id=\"aID73856\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m11mi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9m11-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73855\" class=\"leftMenuInActive\"><a id=\"aID73855\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m11/at91sam9m110',this)\" class=\"leftMenuLinkInActive\">at91sam9m11</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73637\" class=\"leftMenuInActive\"><a id=\"aID73637\" href=\"#\" OnMouseOver=\"link('_page','SAM9M11mi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9M11-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73640\" class=\"leftMenuInActive\"><a id=\"aID73640\" href=\"#\" OnMouseOver=\"link('_page','SAM9M11mi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9M11-EK - External components</a></div>\n");
document.write("<div id=\"divID73641\" class=\"leftMenuInActive\"><a id=\"aID73641\" href=\"#\" OnMouseOver=\"link('_page','SAM9M11mi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9M11-EK - Memories</a></div>\n");
document.write("<div id=\"divID73638\" class=\"leftMenuInActive\"><a id=\"aID73638\" href=\"#\" OnMouseOver=\"link('_page','SAM9M11mi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9M11-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73639\" class=\"leftMenuInActive\"><a id=\"aID73639\" href=\"#\" OnMouseOver=\"link('_page','SAM9M11mi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9M11-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73642\" class=\"leftMenuInActive\"><a id=\"aID73642\" href=\"#\" OnMouseOver=\"link('_page','SAM9M11mi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9M11-EK- Individual chip definition</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74098\" class=\"leftMenuInActive\"><a id=\"aID74098\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74099\" class=\"leftMenuInActive\"><a id=\"aID74099\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74100\" class=\"leftMenuInActive\"><a id=\"aID74100\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
