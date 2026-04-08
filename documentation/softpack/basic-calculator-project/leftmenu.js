function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74014\" class=\"headerLeftMenuInActive\"><a id=\"aID74014\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_calculatormi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-calculator-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74320\" class=\"leftMenuInActive\"><a id=\"aID74320\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
