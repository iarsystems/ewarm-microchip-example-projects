function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73886\" class=\"headerLeftMenuInActive\"><a id=\"aID73886\" href=\"#\" OnMouseOver=\"link('_dir','norflash0',this)\" class=\"leftMenuLinkHeadInActive\">norflash</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74130\" class=\"leftMenuInActive\"><a id=\"aID74130\" href=\"#\" OnMouseOver=\"link('_unit','NorFlashAmd_h0',this)\" class=\"leftMenuLinkInActive\">NorFlashAmd.h</a></div>\n");
document.write("<div id=\"divID74131\" class=\"leftMenuInActive\"><a id=\"aID74131\" href=\"#\" OnMouseOver=\"link('_unit','NorFlashApi_h0',this)\" class=\"leftMenuLinkInActive\">NorFlashApi.h</a></div>\n");
document.write("<div id=\"divID74132\" class=\"leftMenuInActive\"><a id=\"aID74132\" href=\"#\" OnMouseOver=\"link('_unit','NorFlashCommon_h0',this)\" class=\"leftMenuLinkInActive\">NorFlashCommon.h</a></div>\n");
document.write("<div id=\"divID74133\" class=\"leftMenuInActive\"><a id=\"aID74133\" href=\"#\" OnMouseOver=\"link('_unit','NorFlashIntel_h0',this)\" class=\"leftMenuLinkInActive\">NorFlashIntel.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
