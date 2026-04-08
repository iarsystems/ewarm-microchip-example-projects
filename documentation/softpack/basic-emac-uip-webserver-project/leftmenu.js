function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73959\" class=\"headerLeftMenuInActive\"><a id=\"aID73959\" href=\"#\" OnMouseOver=\"link('_dir','basicmi_emacmi_uipmi_webservermi_project0',this)\" class=\"leftMenuLinkHeadInActive\">basic-emac-uip-webserver-project</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74272\" class=\"leftMenuInActive\"><a id=\"aID74272\" href=\"#\" OnMouseOver=\"link('_unit','main_c0',this)\" class=\"leftMenuLinkInActive\">main.c</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
