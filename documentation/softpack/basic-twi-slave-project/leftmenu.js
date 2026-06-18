function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73979\" class=\"headerLeftMenuInActive\"><a id=\"aID73979\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_twimi_slavemi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-twi-slave-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
