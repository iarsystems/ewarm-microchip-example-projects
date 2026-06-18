function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74004\" class=\"headerLeftMenuInActive\"><a id=\"aID74004\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_rtcmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-rtc-project</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
