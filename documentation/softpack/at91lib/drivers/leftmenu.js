function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73878\" class=\"headerLeftMenuInActive\"><a id=\"aID73878\" href=\"#\" OnMouseOver=\"link('_dir','drivers0',this)\" class=\"leftMenuLinkHeadInActive\">drivers</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73876\" class=\"leftMenuInActive\"><a id=\"aID73876\" href=\"#\" OnMouseOver=\"link('_dir','async/async0',this)\" class=\"leftMenuLinkInActive\">async</a></div>\n");
document.write("<div id=\"divID73877\" class=\"leftMenuInActive\"><a id=\"aID73877\" href=\"#\" OnMouseOver=\"link('_dir','dmad/dmad0',this)\" class=\"leftMenuLinkInActive\">dmad</a></div>\n");
document.write("<div id=\"divID73879\" class=\"leftMenuInActive\"><a id=\"aID73879\" href=\"#\" OnMouseOver=\"link('_dir','lcd/lcd0',this)\" class=\"leftMenuLinkInActive\">lcd</a></div>\n");
document.write("<div id=\"divID73880\" class=\"leftMenuInActive\"><a id=\"aID73880\" href=\"#\" OnMouseOver=\"link('_dir','macb/macb0',this)\" class=\"leftMenuLinkInActive\">macb</a></div>\n");
document.write("<div id=\"divID73881\" class=\"leftMenuInActive\"><a id=\"aID73881\" href=\"#\" OnMouseOver=\"link('_dir','tsd/tsd0',this)\" class=\"leftMenuLinkInActive\">tsd</a></div>\n");
document.write("<div id=\"divID73882\" class=\"leftMenuInActive\"><a id=\"aID73882\" href=\"#\" OnMouseOver=\"link('_dir','twi/twi0',this)\" class=\"leftMenuLinkInActive\">twi</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
