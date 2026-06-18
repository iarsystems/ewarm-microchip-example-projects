function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73949\" class=\"headerLeftMenuInActive\"><a id=\"aID73949\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_canmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-can-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
