
function initialize_mobile_browser()
{
	bindmobilestyle();
}

function bindmobilestyle()
{
	$('#page').attr('data-role','page');
	$('#header').attr('data-role','header');
	$('#content').attr('data-role','content');
	$('#footer').attr('data-role','footer');
}