function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74016\" class=\"headerLeftMenuInActive\"><a id=\"aID74016\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_keyboardmatrixmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-keyboardmatrix-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
