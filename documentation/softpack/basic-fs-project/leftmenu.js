function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73963\" class=\"headerLeftMenuInActive\"><a id=\"aID73963\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_fsmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-fs-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
