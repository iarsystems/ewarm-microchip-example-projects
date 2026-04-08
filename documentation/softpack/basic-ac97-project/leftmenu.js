function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73946\" class=\"headerLeftMenuInActive\"><a id=\"aID73946\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_ac97mi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-ac97-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74264\" class=\"leftMenuInActive\"><a id=\"aID74264\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
