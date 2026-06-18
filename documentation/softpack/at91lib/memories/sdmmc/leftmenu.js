function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73887\" class=\"headerLeftMenuInActive\"><a id=\"aID73887\" href=\"#\" OnMouseOver=\"link('_dir','sdmmc0',this)\" class=\"leftMenuLinkHeadInActive\">sdmmc</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73674\" class=\"leftMenuInActive\"><a id=\"aID73674\" href=\"#\" OnMouseOver=\"link('_page','sdcardsdmodeinitializationandidentification0',this)\" class=\"leftMenuLinkInActive\">sdcard sd mode initialization and identification</a></div>\n");
document.write("<div id=\"divID73676\" class=\"leftMenuInActive\"><a id=\"aID73676\" href=\"#\" OnMouseOver=\"link('_page','sdcardsdmoderead0',this)\" class=\"leftMenuLinkInActive\">sdcard sd mode read</a></div>\n");
document.write("<div id=\"divID73675\" class=\"leftMenuInActive\"><a id=\"aID73675\" href=\"#\" OnMouseOver=\"link('_page','sdcardsdmodewrite0',this)\" class=\"leftMenuLinkInActive\">sdcard sd mode write</a></div>\n");
document.write("<div id=\"divID73680\" class=\"leftMenuInActive\"><a id=\"aID73680\" href=\"#\" OnMouseOver=\"link('_page','sdcardspimodeinitializationandidentification0',this)\" class=\"leftMenuLinkInActive\">sdcard spi mode initialization and identification</a></div>\n");
document.write("<div id=\"divID73682\" class=\"leftMenuInActive\"><a id=\"aID73682\" href=\"#\" OnMouseOver=\"link('_page','sdcardspimoderead0',this)\" class=\"leftMenuLinkInActive\">sdcard spi mode read</a></div>\n");
document.write("<div id=\"divID73681\" class=\"leftMenuInActive\"><a id=\"aID73681\" href=\"#\" OnMouseOver=\"link('_page','sdcardspimodewrite0',this)\" class=\"leftMenuLinkInActive\">sdcard spi mode write</a></div>\n");
document.write("<div id=\"divID73677\" class=\"leftMenuInActive\"><a id=\"aID73677\" href=\"#\" OnMouseOver=\"link('_page','sdhc0',this)\" class=\"leftMenuLinkInActive\">sdhc</a></div>\n");
document.write("<div id=\"divID73678\" class=\"leftMenuInActive\"><a id=\"aID73678\" href=\"#\" OnMouseOver=\"link('_page','sdmmc_mci0',this)\" class=\"leftMenuLinkInActive\">sdmmc_mci</a></div>\n");
document.write("<div id=\"divID73679\" class=\"leftMenuInActive\"><a id=\"aID73679\" href=\"#\" OnMouseOver=\"link('_page','sdmmc_spi0',this)\" class=\"leftMenuLinkInActive\">sdmmc_spi</a></div>\n");
document.write("<div id=\"divID73683\" class=\"leftMenuInActive\"><a id=\"aID73683\" href=\"#\" OnMouseOver=\"link('_page','sdspi0',this)\" class=\"leftMenuLinkInActive\">sdspi</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
