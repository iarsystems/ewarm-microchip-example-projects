function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74005\" class=\"headerLeftMenuInActive\"><a id=\"aID74005\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_sdmmcmi_testmi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-sdmmc-test-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74312\" class=\"leftMenuInActive\"><a id=\"aID74312\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
