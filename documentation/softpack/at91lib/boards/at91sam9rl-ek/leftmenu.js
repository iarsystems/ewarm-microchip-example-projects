function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73858\" class=\"headerLeftMenuInActive\"><a id=\"aID73858\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9rlmi_ek0',this)\" class=\"leftMenuLinkHeadInActive\">at91sam9rl-ek</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73857\" class=\"leftMenuInActive\"><a id=\"aID73857\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9rl64/at91sam9rl640',this)\" class=\"leftMenuLinkInActive\">at91sam9rl64</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73646\" class=\"leftMenuInActive\"><a id=\"aID73646\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_BoardDescription0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK - Board Description</a></div>\n");
document.write("<div id=\"divID73649\" class=\"leftMenuInActive\"><a id=\"aID73649\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_Externalcomponents0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK - External components</a></div>\n");
document.write("<div id=\"divID73650\" class=\"leftMenuInActive\"><a id=\"aID73650\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_Memories0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK - Memories</a></div>\n");
document.write("<div id=\"divID73647\" class=\"leftMenuInActive\"><a id=\"aID73647\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_Operatingfrequencies0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK - Operating frequencies</a></div>\n");
document.write("<div id=\"divID73648\" class=\"leftMenuInActive\"><a id=\"aID73648\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_PIOdefinitions0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK - PIO definitions</a></div>\n");
document.write("<div id=\"divID73651\" class=\"leftMenuInActive\"><a id=\"aID73651\" href=\"#\" OnMouseOver=\"link('_page','SAM9RLmi_EKmi_Individualchipdefinition0',this)\" class=\"leftMenuLinkInActive\">SAM9RL-EK- Individual chip definition</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74102\" class=\"leftMenuInActive\"><a id=\"aID74102\" href=\"#\" OnMouseOver=\"link('_unit','board_h0',this)\" class=\"leftMenuLinkInActive\">board.h</a></div>\n");
document.write("<div id=\"divID74103\" class=\"leftMenuInActive\"><a id=\"aID74103\" href=\"#\" OnMouseOver=\"link('_unit','board_lowlevel_h0',this)\" class=\"leftMenuLinkInActive\">board_lowlevel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
