function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74011\" class=\"headerLeftMenuInActive\"><a id=\"aID74011\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_microcanopenmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-microcanopen-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
