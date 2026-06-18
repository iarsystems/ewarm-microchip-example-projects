function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73929\" class=\"headerLeftMenuInActive\"><a id=\"aID73929\" href=\"#\" OnMouseOver=\"link('_dir','massstorage0',this)\" class=\"leftMenuLinkHeadInActive\">massstorage</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73753\" class=\"leftMenuInActive\"><a id=\"aID73753\" href=\"#\" OnMouseOver=\"link('_page','MSdevicecodes0',this)\" class=\"leftMenuLinkInActive\">MS device codes</a></div>\n");
document.write("<div id=\"divID73754\" class=\"leftMenuInActive\"><a id=\"aID73754\" href=\"#\" OnMouseOver=\"link('_page','MSinterfacecodes0',this)\" class=\"leftMenuLinkInActive\">MS interface codes</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74211\" class=\"leftMenuInActive\"><a id=\"aID74211\" href=\"#\" OnMouseOver=\"link('_unit','MSDeviceDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">MSDeviceDescriptor.h</a></div>\n");
document.write("<div id=\"divID74212\" class=\"leftMenuInActive\"><a id=\"aID74212\" href=\"#\" OnMouseOver=\"link('_unit','MSInterfaceDescriptor_h0',this)\" class=\"leftMenuLinkInActive\">MSInterfaceDescriptor.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
