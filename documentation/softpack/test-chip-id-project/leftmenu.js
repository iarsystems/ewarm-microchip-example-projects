function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74008\" class=\"headerLeftMenuInActive\"><a id=\"aID74008\" href=\"#\" OnMouseOver=\"link('_dir','testmi_chipmi_idmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">test-chip-id-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74315\" class=\"leftMenuInActive\"><a id=\"aID74315\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
