function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73889\" class=\"headerLeftMenuInActive\"><a id=\"aID73889\" href=\"#\" OnMouseOver=\"link('_dir','ac97c0',this)\" class=\"leftMenuLinkHeadInActive\">ac97c</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74139\" class=\"leftMenuInActive\"><a id=\"aID74139\" href=\"#\" OnMouseOver=\"link('_unit','ac97c_h0',this)\" class=\"leftMenuLinkInActive\">ac97c.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
