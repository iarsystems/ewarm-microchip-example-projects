function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73960\" class=\"headerLeftMenuInActive\"><a id=\"aID73960\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_fatefslmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-fatefsl-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
