function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73923\" class=\"headerLeftMenuInActive\"><a id=\"aID73923\" href=\"#\" OnMouseOver=\"link('_dir','usart0',this)\" class=\"leftMenuLinkHeadInActive\">usart</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73687\" class=\"leftMenuInActive\"><a id=\"aID73687\" href=\"#\" OnMouseOver=\"link('_page','USARTmodes0',this)\" class=\"leftMenuLinkInActive\">USART modes</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
