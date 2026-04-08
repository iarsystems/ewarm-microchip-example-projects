function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73891\" class=\"headerLeftMenuInActive\"><a id=\"aID73891\" href=\"#\" OnMouseOver=\"link('_dir','aes0',this)\" class=\"leftMenuLinkHeadInActive\">aes</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74142\" class=\"leftMenuInActive\"><a id=\"aID74142\" href=\"#\" OnMouseOver=\"link('_unit','aes_p_h0',this)\" class=\"leftMenuLinkInActive\">aes_p.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
