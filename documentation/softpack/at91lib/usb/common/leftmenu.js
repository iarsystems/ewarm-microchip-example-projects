function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73926\" class=\"headerLeftMenuInActive\"><a id=\"aID73926\" href=\"#\" OnMouseOver=\"link('_dir','common0',this)\" class=\"leftMenuLinkHeadInActive\">common</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73924\" class=\"leftMenuInActive\"><a id=\"aID73924\" href=\"#\" OnMouseOver=\"link('_dir','audio/audio0',this)\" class=\"leftMenuLinkInActive\">audio</a></div>\n");
document.write("<div id=\"divID73925\" class=\"leftMenuInActive\"><a id=\"aID73925\" href=\"#\" OnMouseOver=\"link('_dir','cdc/cdc0',this)\" class=\"leftMenuLinkInActive\">cdc</a></div>\n");
document.write("<div id=\"divID73927\" class=\"leftMenuInActive\"><a id=\"aID73927\" href=\"#\" OnMouseOver=\"link('_dir','core/core0',this)\" class=\"leftMenuLinkInActive\">core</a></div>\n");
document.write("<div id=\"divID73928\" class=\"leftMenuInActive\"><a id=\"aID73928\" href=\"#\" OnMouseOver=\"link('_dir','hid/hid0',this)\" class=\"leftMenuLinkInActive\">hid</a></div>\n");
document.write("<div id=\"divID73929\" class=\"leftMenuInActive\"><a id=\"aID73929\" href=\"#\" OnMouseOver=\"link('_dir','massstorage/massstorage0',this)\" class=\"leftMenuLinkInActive\">massstorage</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
