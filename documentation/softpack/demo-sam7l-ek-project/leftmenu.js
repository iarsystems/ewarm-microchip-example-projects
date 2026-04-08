function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74019\" class=\"headerLeftMenuInActive\"><a id=\"aID74019\" href=\"#\" OnMouseOver=\"link('_dir','demomi_sam7lmi_ekmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">demo-sam7l-ek-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
