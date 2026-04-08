function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73872\" class=\"headerLeftMenuInActive\"><a id=\"aID73872\" href=\"#\" OnMouseOver=\"link('_dir','ov96550',this)\" class=\"leftMenuLinkHeadInActive\">ov9655</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74116\" class=\"leftMenuInActive\"><a id=\"aID74116\" href=\"#\" OnMouseOver=\"link('_unit','ov9655_h0',this)\" class=\"leftMenuLinkInActive\">ov9655.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
