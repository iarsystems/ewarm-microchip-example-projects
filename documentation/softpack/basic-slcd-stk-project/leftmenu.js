function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74021\" class=\"headerLeftMenuInActive\"><a id=\"aID74021\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_slcdmi_stkmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-slcd-stk-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74322\" class=\"leftMenuInActive\"><a id=\"aID74322\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
