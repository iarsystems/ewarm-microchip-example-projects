function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73884\" class=\"headerLeftMenuInActive\"><a id=\"aID73884\" href=\"#\" OnMouseOver=\"link('_dir','memories0',this)\" class=\"leftMenuLinkHeadInActive\">memories</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73883\" class=\"leftMenuInActive\"><a id=\"aID73883\" href=\"#\" OnMouseOver=\"link('_dir','flash/flash0',this)\" class=\"leftMenuLinkInActive\">flash</a></div>\n");
document.write("<div id=\"divID73885\" class=\"leftMenuInActive\"><a id=\"aID73885\" href=\"#\" OnMouseOver=\"link('_dir','nandflash/nandflash0',this)\" class=\"leftMenuLinkInActive\">nandflash</a></div>\n");
document.write("<div id=\"divID73886\" class=\"leftMenuInActive\"><a id=\"aID73886\" href=\"#\" OnMouseOver=\"link('_dir','norflash/norflash0',this)\" class=\"leftMenuLinkInActive\">norflash</a></div>\n");
document.write("<div id=\"divID73887\" class=\"leftMenuInActive\"><a id=\"aID73887\" href=\"#\" OnMouseOver=\"link('_dir','sdmmc/sdmmc0',this)\" class=\"leftMenuLinkInActive\">sdmmc</a></div>\n");
document.write("<div id=\"divID73888\" class=\"leftMenuInActive\"><a id=\"aID73888\" href=\"#\" OnMouseOver=\"link('_dir','spi-flash/spimi_flash0',this)\" class=\"leftMenuLinkInActive\">spi-flash</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74126\" class=\"leftMenuInActive\"><a id=\"aID74126\" href=\"#\" OnMouseOver=\"link('_unit','MEDFlash_h0',this)\" class=\"leftMenuLinkInActive\">MEDFlash.h</a></div>\n");
document.write("<div id=\"divID74127\" class=\"leftMenuInActive\"><a id=\"aID74127\" href=\"#\" OnMouseOver=\"link('_unit','Media_h0',this)\" class=\"leftMenuLinkInActive\">Media.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
