function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73888\" class=\"headerLeftMenuInActive\"><a id=\"aID73888\" href=\"#\" OnMouseOver=\"link('_dir','spimi_flash0',this)\" class=\"leftMenuLinkHeadInActive\">spi-flash</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74134\" class=\"leftMenuInActive\"><a id=\"aID74134\" href=\"#\" OnMouseOver=\"link('_unit','at26_h0',this)\" class=\"leftMenuLinkInActive\">at26.h</a></div>\n");
document.write("<div id=\"divID74135\" class=\"leftMenuInActive\"><a id=\"aID74135\" href=\"#\" OnMouseOver=\"link('_unit','at26d_h0',this)\" class=\"leftMenuLinkInActive\">at26d.h</a></div>\n");
document.write("<div id=\"divID74136\" class=\"leftMenuInActive\"><a id=\"aID74136\" href=\"#\" OnMouseOver=\"link('_unit','at45_h0',this)\" class=\"leftMenuLinkInActive\">at45.h</a></div>\n");
document.write("<div id=\"divID74137\" class=\"leftMenuInActive\"><a id=\"aID74137\" href=\"#\" OnMouseOver=\"link('_unit','at45d_h0',this)\" class=\"leftMenuLinkInActive\">at45d.h</a></div>\n");
document.write("<div id=\"divID74138\" class=\"leftMenuInActive\"><a id=\"aID74138\" href=\"#\" OnMouseOver=\"link('_unit','spid_h0',this)\" class=\"leftMenuLinkInActive\">spid.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
