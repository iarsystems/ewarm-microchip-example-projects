function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73865\" class=\"headerLeftMenuInActive\"><a id=\"aID73865\" href=\"#\" OnMouseOver=\"link('_dir','components0',this)\" class=\"leftMenuLinkHeadInActive\">components</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73862\" class=\"leftMenuInActive\"><a id=\"aID73862\" href=\"#\" OnMouseOver=\"link('_dir','ads7843/ads78430',this)\" class=\"leftMenuLinkInActive\">ads7843</a></div>\n");
document.write("<div id=\"divID73863\" class=\"leftMenuInActive\"><a id=\"aID73863\" href=\"#\" OnMouseOver=\"link('_dir','codec-ad1981b/codecmi_ad1981b0',this)\" class=\"leftMenuLinkInActive\">codec-ad1981b</a></div>\n");
document.write("<div id=\"divID73864\" class=\"leftMenuInActive\"><a id=\"aID73864\" href=\"#\" OnMouseOver=\"link('_dir','codec-wm8731/codecmi_wm87310',this)\" class=\"leftMenuLinkInActive\">codec-wm8731</a></div>\n");
document.write("<div id=\"divID73866\" class=\"leftMenuInActive\"><a id=\"aID73866\" href=\"#\" OnMouseOver=\"link('_dir','dac-at73c213/dacmi_at73c2130',this)\" class=\"leftMenuLinkInActive\">dac-at73c213</a></div>\n");
document.write("<div id=\"divID73867\" class=\"leftMenuInActive\"><a id=\"aID73867\" href=\"#\" OnMouseOver=\"link('_dir','hx8347/hx83470',this)\" class=\"leftMenuLinkInActive\">hx8347</a></div>\n");
document.write("<div id=\"divID73868\" class=\"leftMenuInActive\"><a id=\"aID73868\" href=\"#\" OnMouseOver=\"link('_dir','iso7816/iso78160',this)\" class=\"leftMenuLinkInActive\">iso7816</a></div>\n");
document.write("<div id=\"divID73869\" class=\"leftMenuInActive\"><a id=\"aID73869\" href=\"#\" OnMouseOver=\"link('_dir','kbmatrix/kbmatrix0',this)\" class=\"leftMenuLinkInActive\">kbmatrix</a></div>\n");
document.write("<div id=\"divID73871\" class=\"leftMenuInActive\"><a id=\"aID73871\" href=\"#\" OnMouseOver=\"link('_dir','omnivision/omnivision0',this)\" class=\"leftMenuLinkInActive\">omnivision</a></div>\n");
document.write("<div id=\"divID74027\" class=\"leftMenuInActive\"><a id=\"aID74027\" href=\"#\" OnMouseOver=\"link('_dir','sensors/sensors0',this)\" class=\"leftMenuLinkInActive\">sensors</a></div>\n");
document.write("<div id=\"divID74028\" class=\"leftMenuInActive\"><a id=\"aID74028\" href=\"#\" OnMouseOver=\"link('_dir','slcd/slcd0',this)\" class=\"leftMenuLinkInActive\">slcd</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
