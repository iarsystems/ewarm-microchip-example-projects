function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73950\" class=\"headerLeftMenuInActive\"><a id=\"aID73950\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_cryptomi_timingmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-crypto-timing-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
