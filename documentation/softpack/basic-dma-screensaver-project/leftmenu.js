function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73953\" class=\"headerLeftMenuInActive\"><a id=\"aID73953\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_dmami_screensavermi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-dma-screensaver-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74267\" class=\"leftMenuInActive\"><a id=\"aID74267\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
