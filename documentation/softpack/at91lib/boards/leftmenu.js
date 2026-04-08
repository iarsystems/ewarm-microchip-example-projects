function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73861\" class=\"headerLeftMenuInActive\"><a id=\"aID73861\" href=\"#\" OnMouseOver=\"link('_dir','boards0',this)\" class=\"leftMenuLinkHeadInActive\">boards</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID73822\" class=\"leftMenuInActive\"><a id=\"aID73822\" href=\"#\" OnMouseOver=\"link('_dir','at91cap9-dk/at91cap9mi_dk0',this)\" class=\"leftMenuLinkInActive\">at91cap9-dk</a></div>\n");
document.write("<div id=\"divID73824\" class=\"leftMenuInActive\"><a id=\"aID73824\" href=\"#\" OnMouseOver=\"link('_dir','at91cap9-stk/at91cap9mi_stk0',this)\" class=\"leftMenuLinkInActive\">at91cap9-stk</a></div>\n");
document.write("<div id=\"divID73826\" class=\"leftMenuInActive\"><a id=\"aID73826\" href=\"#\" OnMouseOver=\"link('_dir','at91sam3u-ek/at91sam3umi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam3u-ek</a></div>\n");
document.write("<div id=\"divID73828\" class=\"leftMenuInActive\"><a id=\"aID73828\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7a3-ek/at91sam7a3mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam7a3-ek</a></div>\n");
document.write("<div id=\"divID73830\" class=\"leftMenuInActive\"><a id=\"aID73830\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7l-ek/at91sam7lmi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam7l-ek</a></div>\n");
document.write("<div id=\"divID73832\" class=\"leftMenuInActive\"><a id=\"aID73832\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7l-stk/at91sam7lmi_stk0',this)\" class=\"leftMenuLinkInActive\">at91sam7l-stk</a></div>\n");
document.write("<div id=\"divID73834\" class=\"leftMenuInActive\"><a id=\"aID73834\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7s-ek/at91sam7smi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam7s-ek</a></div>\n");
document.write("<div id=\"divID73836\" class=\"leftMenuInActive\"><a id=\"aID73836\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7se-ek/at91sam7semi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam7se-ek</a></div>\n");
document.write("<div id=\"divID73838\" class=\"leftMenuInActive\"><a id=\"aID73838\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7x-ek/at91sam7xmi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam7x-ek</a></div>\n");
document.write("<div id=\"divID73840\" class=\"leftMenuInActive\"><a id=\"aID73840\" href=\"#\" OnMouseOver=\"link('_dir','at91sam7xc-ek/at91sam7xcmi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam7xc-ek</a></div>\n");
document.write("<div id=\"divID73842\" class=\"leftMenuInActive\"><a id=\"aID73842\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9260-ek/at91sam9260mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9260-ek</a></div>\n");
document.write("<div id=\"divID73844\" class=\"leftMenuInActive\"><a id=\"aID73844\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9261-ek/at91sam9261mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9261-ek</a></div>\n");
document.write("<div id=\"divID73846\" class=\"leftMenuInActive\"><a id=\"aID73846\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9263-ek/at91sam9263mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9263-ek</a></div>\n");
document.write("<div id=\"divID73848\" class=\"leftMenuInActive\"><a id=\"aID73848\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g10-ek/at91sam9g10mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9g10-ek</a></div>\n");
document.write("<div id=\"divID73850\" class=\"leftMenuInActive\"><a id=\"aID73850\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g20-ek/at91sam9g20mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9g20-ek</a></div>\n");
document.write("<div id=\"divID73852\" class=\"leftMenuInActive\"><a id=\"aID73852\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9g45-ek/at91sam9g45mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9g45-ek</a></div>\n");
document.write("<div id=\"divID73854\" class=\"leftMenuInActive\"><a id=\"aID73854\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m10-ek/at91sam9m10mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9m10-ek</a></div>\n");
document.write("<div id=\"divID73856\" class=\"leftMenuInActive\"><a id=\"aID73856\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9m11-ek/at91sam9m11mi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9m11-ek</a></div>\n");
document.write("<div id=\"divID73858\" class=\"leftMenuInActive\"><a id=\"aID73858\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9rl-ek/at91sam9rlmi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9rl-ek</a></div>\n");
document.write("<div id=\"divID73860\" class=\"leftMenuInActive\"><a id=\"aID73860\" href=\"#\" OnMouseOver=\"link('_dir','at91sam9xe-ek/at91sam9xemi_ek0',this)\" class=\"leftMenuLinkInActive\">at91sam9xe-ek</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
