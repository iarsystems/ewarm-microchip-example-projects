function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73820\" class=\"headerLeftMenuInActive\"><a id=\"aID73820\" href=\"#\" OnMouseOver=\"link('_dir','at91lib0',this)\" class=\"leftMenuLinkHeadInActive\">at91lib</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73861\" class=\"leftMenuInActive\"><a id=\"aID73861\" href=\"#\" OnMouseOver=\"link('_dir','boards/boards0',this)\" class=\"leftMenuLinkInActive\">boards</a></div>\n");
document.write("<div id=\"divID73865\" class=\"leftMenuInActive\"><a id=\"aID73865\" href=\"#\" OnMouseOver=\"link('_dir','components/components0',this)\" class=\"leftMenuLinkInActive\">components</a></div>\n");
document.write("<div id=\"divID73878\" class=\"leftMenuInActive\"><a id=\"aID73878\" href=\"#\" OnMouseOver=\"link('_dir','drivers/drivers0',this)\" class=\"leftMenuLinkInActive\">drivers</a></div>\n");
document.write("<div id=\"divID73884\" class=\"leftMenuInActive\"><a id=\"aID73884\" href=\"#\" OnMouseOver=\"link('_dir','memories/memories0',this)\" class=\"leftMenuLinkInActive\">memories</a></div>\n");
document.write("<div id=\"divID73904\" class=\"leftMenuInActive\"><a id=\"aID73904\" href=\"#\" OnMouseOver=\"link('_dir','peripherals/peripherals0',this)\" class=\"leftMenuLinkInActive\">peripherals</a></div>\n");
document.write("<div id=\"divID73942\" class=\"leftMenuInActive\"><a id=\"aID73942\" href=\"#\" OnMouseOver=\"link('_dir','usb/usb0',this)\" class=\"leftMenuLinkInActive\">usb</a></div>\n");
document.write("<div id=\"divID73943\" class=\"leftMenuInActive\"><a id=\"aID73943\" href=\"#\" OnMouseOver=\"link('_dir','utility/utility0',this)\" class=\"leftMenuLinkInActive\">utility</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
