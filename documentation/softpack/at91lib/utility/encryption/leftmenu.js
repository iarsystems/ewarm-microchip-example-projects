function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73944\" class=\"headerLeftMenuInActive\"><a id=\"aID73944\" href=\"#\" OnMouseOver=\"link('_dir','encryption0',this)\" class=\"leftMenuLinkHeadInActive\">encryption</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74251\" class=\"leftMenuInActive\"><a id=\"aID74251\" href=\"#\" OnMouseOver=\"link('_unit','aes_hardware_h0',this)\" class=\"leftMenuLinkInActive\">aes_hardware.h</a></div>\n");
document.write("<div id=\"divID74252\" class=\"leftMenuInActive\"><a id=\"aID74252\" href=\"#\" OnMouseOver=\"link('_unit','aes_reference_h0',this)\" class=\"leftMenuLinkInActive\">aes_reference.h</a></div>\n");
document.write("<div id=\"divID74253\" class=\"leftMenuInActive\"><a id=\"aID74253\" href=\"#\" OnMouseOver=\"link('_unit','encryption_h0',this)\" class=\"leftMenuLinkInActive\">encryption.h</a></div>\n");
document.write("<div id=\"divID74254\" class=\"leftMenuInActive\"><a id=\"aID74254\" href=\"#\" OnMouseOver=\"link('_unit','libtomcrypt_h0',this)\" class=\"leftMenuLinkInActive\">libtomcrypt.h</a></div>\n");
document.write("<div id=\"divID74255\" class=\"leftMenuInActive\"><a id=\"aID74255\" href=\"#\" OnMouseOver=\"link('_unit','tdes_hardware_h0',this)\" class=\"leftMenuLinkInActive\">tdes_hardware.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
