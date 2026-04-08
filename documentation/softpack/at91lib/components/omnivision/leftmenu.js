function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73871\" class=\"headerLeftMenuInActive\"><a id=\"aID73871\" href=\"#\" OnMouseOver=\"link('_dir','omnivision0',this)\" class=\"leftMenuLinkHeadInActive\">omnivision</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73872\" class=\"leftMenuInActive\"><a id=\"aID73872\" href=\"#\" OnMouseOver=\"link('_dir','ov9655/ov96550',this)\" class=\"leftMenuLinkInActive\">ov9655</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74115\" class=\"leftMenuInActive\"><a id=\"aID74115\" href=\"#\" OnMouseOver=\"link('_unit','omnivision_h0',this)\" class=\"leftMenuLinkInActive\">omnivision.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
