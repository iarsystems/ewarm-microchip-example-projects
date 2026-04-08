function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73901\" class=\"headerLeftMenuInActive\"><a id=\"aID73901\" href=\"#\" OnMouseOver=\"link('_dir','isi0',this)\" class=\"leftMenuLinkHeadInActive\">isi</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74149\" class=\"leftMenuInActive\"><a id=\"aID74149\" href=\"#\" OnMouseOver=\"link('_unit','isi_c0',this)\" class=\"leftMenuLinkInActive\">isi.c</a></div>\n");
document.write("<div id=\"divID74150\" class=\"leftMenuInActive\"><a id=\"aID74150\" href=\"#\" OnMouseOver=\"link('_unit','isi_h0',this)\" class=\"leftMenuLinkInActive\">isi.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
