function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73969\" class=\"headerLeftMenuInActive\"><a id=\"aID73969\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_rttmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-rtt-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74280\" class=\"leftMenuInActive\"><a id=\"aID74280\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
