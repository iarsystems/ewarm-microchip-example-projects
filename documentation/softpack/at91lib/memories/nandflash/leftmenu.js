function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73885\" class=\"headerLeftMenuInActive\"><a id=\"aID73885\" href=\"#\" OnMouseOver=\"link('_dir','nandflash0',this)\" class=\"leftMenuLinkHeadInActive\">nandflash</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73664\" class=\"leftMenuInActive\"><a id=\"aID73664\" href=\"#\" OnMouseOver=\"link('_page','EccNandFlash0',this)\" class=\"leftMenuLinkInActive\">EccNandFlash</a></div>\n");
document.write("<div id=\"divID73665\" class=\"leftMenuInActive\"><a id=\"aID73665\" href=\"#\" OnMouseOver=\"link('_page','ManagedNandFlash0',this)\" class=\"leftMenuLinkInActive\">ManagedNandFlash</a></div>\n");
document.write("<div id=\"divID73666\" class=\"leftMenuInActive\"><a id=\"aID73666\" href=\"#\" OnMouseOver=\"link('_page','MappedNandFlash0',this)\" class=\"leftMenuLinkInActive\">MappedNandFlash</a></div>\n");
document.write("<div id=\"divID73668\" class=\"leftMenuInActive\"><a id=\"aID73668\" href=\"#\" OnMouseOver=\"link('_page','NandFlashErrorCodes0',this)\" class=\"leftMenuLinkInActive\">NandFlash Error Codes</a></div>\n");
document.write("<div id=\"divID73667\" class=\"leftMenuInActive\"><a id=\"aID73667\" href=\"#\" OnMouseOver=\"link('_page','NandFlashMaximumSupportedValues0',this)\" class=\"leftMenuLinkInActive\">NandFlash Maximum Supported Values</a></div>\n");
document.write("<div id=\"divID73669\" class=\"leftMenuInActive\"><a id=\"aID73669\" href=\"#\" OnMouseOver=\"link('_page','NandFlashModeloptions0',this)\" class=\"leftMenuLinkInActive\">NandFlashModel options</a></div>\n");
document.write("<div id=\"divID73670\" class=\"leftMenuInActive\"><a id=\"aID73670\" href=\"#\" OnMouseOver=\"link('_page','NandSpareScheme0',this)\" class=\"leftMenuLinkInActive\">NandSpareScheme</a></div>\n");
document.write("<div id=\"divID73671\" class=\"leftMenuInActive\"><a id=\"aID73671\" href=\"#\" OnMouseOver=\"link('_page','RawNandflash0',this)\" class=\"leftMenuLinkInActive\">RawNandflash</a></div>\n");
document.write("<div id=\"divID73672\" class=\"leftMenuInActive\"><a id=\"aID73672\" href=\"#\" OnMouseOver=\"link('_page','SkipBlockNandFlash0',this)\" class=\"leftMenuLinkInActive\">SkipBlockNandFlash</a></div>\n");
document.write("<div id=\"divID73673\" class=\"leftMenuInActive\"><a id=\"aID73673\" href=\"#\" OnMouseOver=\"link('_page','TranslatedNandFlash0',this)\" class=\"leftMenuLinkInActive\">TranslatedNandFlash</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74128\" class=\"leftMenuInActive\"><a id=\"aID74128\" href=\"#\" OnMouseOver=\"link('_unit','NandFlashModel_h0',this)\" class=\"leftMenuLinkInActive\">NandFlashModel.h</a></div>\n");
document.write("<div id=\"divID74129\" class=\"leftMenuInActive\"><a id=\"aID74129\" href=\"#\" OnMouseOver=\"link('_unit','NandFlashModelList_h0',this)\" class=\"leftMenuLinkInActive\">NandFlashModelList.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
