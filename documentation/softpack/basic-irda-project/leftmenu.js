function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74015\" class=\"headerLeftMenuInActive\"><a id=\"aID74015\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_irdami_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-irda-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
