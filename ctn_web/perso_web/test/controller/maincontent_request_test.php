<?php
require_once realpath( dirname(__FILE__ ) . '/../../global-config.php');
?>

<html>
<head>
<title>Update Content Test</title>
</head>
<body>
	<h1>TEST REQUEST CONTROLLER UPDATE CONTENT</h1>
	<form method="get" target="_blank" action="<?php echo GlobalConfig::DOMAINE_NAME.'controller/MainContentRequest.php';?>">
		<p>
			lang: <input type="text" name='lang'/>
		</p>
		<p>
			content: <input type="text" name='content'/>
		</p>
		<p>
			ismobile: <input type="text" name='ismobile'/> (enter "true" if mobile request)
		</p>
		<p>
			<input type="submit" value="getMainContent">
		</p>
	</form>
</body>
</html>
