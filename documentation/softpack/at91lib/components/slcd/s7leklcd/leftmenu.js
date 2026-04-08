function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73874\" class=\"headerLeftMenuInActive\"><a id=\"aID73874\" href=\"#\" OnMouseOver=\"link('_dir','s7leklcd0',this)\" class=\"leftMenuLinkHeadInActive\">s7leklcd</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73663\" class=\"leftMenuInActive\"><a id=\"aID73663\" href=\"#\" OnMouseOver=\"link('_page','SAM7Lmi_EKSLCDSegments0',this)\" class=\"leftMenuLinkInActive\">SAM7L-EK SLCD Segments</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74117\" class=\"leftMenuInActive\"><a id=\"aID74117\" href=\"#\" OnMouseOver=\"link('_unit','s7leklcd_h0',this)\" class=\"leftMenuLinkInActive\">s7leklcd.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
