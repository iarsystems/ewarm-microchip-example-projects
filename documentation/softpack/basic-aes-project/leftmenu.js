function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73948\" class=\"headerLeftMenuInActive\"><a id=\"aID73948\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_aesmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-aes-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
