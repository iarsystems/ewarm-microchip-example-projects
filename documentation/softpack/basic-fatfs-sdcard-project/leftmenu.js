function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73962\" class=\"headerLeftMenuInActive\"><a id=\"aID73962\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_fatfsmi_sdcardmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-fatfs-sdcard-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74274\" class=\"leftMenuInActive\"><a id=\"aID74274\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
