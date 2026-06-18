function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73832\" class=\"headerLeftMenuInActive\"><a id=\"aID73832\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7lmi_stk0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7l-stk</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73831\" class=\"leftMenuInActive\"><a id=\"aID73831\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7l128/at91sam7l1280',this)\" class=\"leftMenuLinkInActive\">at91sam7l128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73528\" class=\"leftMenuInActive\"><a id=\"aID73528\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_STKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM7L-STK - Board Description</a></div>\n");
document.write("<div id=\"divID73531\" class=\"leftMenuInActive\"><a id=\"aID73531\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_STKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM7L-STK - Memories</a></div>\n");
document.write("<div id=\"divID73529\" class=\"leftMenuInActive\"><a id=\"aID73529\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_STKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM7L-STK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73530\" class=\"leftMenuInActive\"><a id=\"aID73530\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_STKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM7L-STK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73532\" class=\"leftMenuInActive\"><a id=\"aID73532\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_STKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7L-STK- Individual chip definition</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74051\" class=\"leftMenuInActive\"><a id=\"aID74051\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74052\" class=\"leftMenuInActive\"><a id=\"aID74052\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_c0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.c</a></div>\n");
document.write("<div id=\"divID74053\" class=\"leftMenuInActive\"><a id=\"aID74053\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
