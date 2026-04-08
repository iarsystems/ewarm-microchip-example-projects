function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74026\" class=\"headerLeftMenuInActive\"><a id=\"aID74026\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_safemi_securemi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-safe-secure-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
