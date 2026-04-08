function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID74007\" class=\"headerLeftMenuInActive\"><a id=\"aID74007\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_twimi_tempsensormi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-twi-tempsensor-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74314\" class=\"leftMenuInActive\"><a id=\"aID74314\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
