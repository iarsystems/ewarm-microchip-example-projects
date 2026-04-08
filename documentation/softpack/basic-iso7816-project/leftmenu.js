function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74010\" class=\"headerLeftMenuInActive\"><a id=\"aID74010\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_iso7816mi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-iso7816-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74317\" class=\"leftMenuInActive\"><a id=\"aID74317\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
