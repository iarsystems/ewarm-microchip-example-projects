function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73834\" class=\"headerLeftMenuInActive\"><a id=\"aID73834\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7smi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam7s-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73833\" class=\"leftMenuInActive\"><a id=\"aID73833\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7s128/at91sam7s1280',this)\" class=\"leftMenuLinkInActive\">at91sam7s128</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73536\" class=\"leftMenuInActive\"><a id=\"aID73536\" href=\"#\" OnMouseOver=\"link('_page','SAM7Smi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM7S-EK - External components</a></div>\n");
document.write("<div id=\"divID73537\" class=\"leftMenuInActive\"><a id=\"aID73537\" href=\"#\" OnMouseOver=\"link('_page','SAM7Smi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM7S-EK - Individual chip definition</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74055\" class=\"leftMenuInActive\"><a id=\"aID74055\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74056\" class=\"leftMenuInActive\"><a id=\"aID74056\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
document.write("<div id=\"divID74057\" class=\"leftMenuInActive\"><a id=\"aID74057\" href=\"#\" OnMouseOver=\"link('_unit','board_memories_h0',this)\" class=\"leftMenuLinkInActive\">board_memories.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
