function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74000\" class=\"headerLeftMenuInActive\"><a id=\"aID74000\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_fatfsmi_nandflashmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-fatfs-nandflash-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74308\" class=\"leftMenuInActive\"><a id=\"aID74308\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
