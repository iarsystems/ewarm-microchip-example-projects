function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74020\" class=\"headerLeftMenuInActive\"><a id=\"aID74020\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_lowpowermi_stkmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-lowpower-stk-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
