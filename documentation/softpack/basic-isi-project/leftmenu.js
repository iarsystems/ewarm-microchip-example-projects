function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73964\" class=\"headerLeftMenuInActive\"><a id=\"aID73964\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_isimi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-isi-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74275\" class=\"leftMenuInActive\"><a id=\"aID74275\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
