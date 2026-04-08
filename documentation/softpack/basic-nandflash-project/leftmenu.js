function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73966\" class=\"headerLeftMenuInActive\"><a id=\"aID73966\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_nandflashmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-nandflash-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74277\" class=\"leftMenuInActive\"><a id=\"aID74277\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
