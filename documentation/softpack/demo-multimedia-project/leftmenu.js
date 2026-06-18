function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73981\" class=\"headerLeftMenuInActive\"><a id=\"aID73981\" href=\"#\" OnMouseOver=\"link('_dir','demomi_multimediami_project0',this)\" class=\"leftMenuLinkHeadInActive\">demo-multimedia-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74290\" class=\"leftMenuInActive\"><a id=\"aID74290\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
