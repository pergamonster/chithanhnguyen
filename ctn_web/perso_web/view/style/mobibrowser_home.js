$(document).ready(function() {
	
	reset_home_style();
	
	$("#maincontent").live('divinnerreloaded',function(event) {
		reset_home_style();
	});

});

function reset_home_style()
{
	$('h1').css({"color":"#FFFF00"});
}