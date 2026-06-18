function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID73930\" class=\"headerLeftMenuInActive\"><a id=\"aID73930\" href=\"#\" OnMouseOver=\"link('_dir','audiomi_speaker0',this)\" class=\"leftMenuLinkHeadInActive\">audio-speaker</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID73756\" class=\"leftMenuInActive\"><a id=\"aID73756\" href=\"#\" OnMouseOver=\"link('_page','AudioSpeakerChannelNumbers0',this)\" class=\"leftMenuLinkInActive\">Audio Speaker Channel Numbers</a></div>\n");
document.write("<div id=\"divID73757\" class=\"leftMenuInActive\"><a id=\"aID73757\" href=\"#\" OnMouseOver=\"link('_page','AudioSpeakerDeviceCodes0',this)\" class=\"leftMenuLinkInActive\">Audio Speaker Device Codes</a></div>\n");
document.write("<div id=\"divID73758\" class=\"leftMenuInActive\"><a id=\"aID73758\" href=\"#\" OnMouseOver=\"link('_page','AudioSpeakerEndpointNumbers0',this)\" class=\"leftMenuLinkInActive\">Audio Speaker Endpoint Numbers</a></div>\n");
document.write("<div id=\"divID73760\" class=\"leftMenuInActive\"><a id=\"aID73760\" href=\"#\" OnMouseOver=\"link('_page','AudioSpeakerEntityIDs0',this)\" class=\"leftMenuLinkInActive\">Audio Speaker Entity IDs</a></div>\n");
document.write("<div id=\"divID73759\" class=\"leftMenuInActive\"><a id=\"aID73759\" href=\"#\" OnMouseOver=\"link('_page','AudioSpeakerInterfaceIDs0',this)\" class=\"leftMenuLinkInActive\">Audio Speaker Interface IDs</a></div>\n");
document.write("<div id=\"divID73755\" class=\"leftMenuInActive\"><a id=\"aID73755\" href=\"#\" OnMouseOver=\"link('_page','AudioSpeakerstreaminformation0',this)\" class=\"leftMenuLinkInActive\">Audio Speaker stream information</a></div>\n");
document.write("<div id=\"divID73762\" class=\"leftMenuInActive\"><a id=\"aID73762\" href=\"#\" OnMouseOver=\"link('_page','USBAudioRecorder0',this)\" class=\"leftMenuLinkInActive\">USB Audio Recorder</a></div>\n");
document.write("<div id=\"divID73761\" class=\"leftMenuInActive\"><a id=\"aID73761\" href=\"#\" OnMouseOver=\"link('_page','USBAudioSpeakerDevice0',this)\" class=\"leftMenuLinkInActive\">USB Audio Speaker Device</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID74213\" class=\"leftMenuInActive\"><a id=\"aID74213\" href=\"#\" OnMouseOver=\"link('_unit','AUDDSpeakerChannel_h0',this)\" class=\"leftMenuLinkInActive\">AUDDSpeakerChannel.h</a></div>\n");
document.write("<div id=\"divID74214\" class=\"leftMenuInActive\"><a id=\"aID74214\" href=\"#\" OnMouseOver=\"link('_unit','AUDDSpeakerDriver_h0',this)\" class=\"leftMenuLinkInActive\">AUDDSpeakerDriver.h</a></div>\n");
document.write("<div id=\"divID74215\" class=\"leftMenuInActive\"><a id=\"aID74215\" href=\"#\" OnMouseOver=\"link('_unit','AUDDSpeakerDriverDescriptors_h0',this)\" class=\"leftMenuLinkInActive\">AUDDSpeakerDriverDescriptors.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
