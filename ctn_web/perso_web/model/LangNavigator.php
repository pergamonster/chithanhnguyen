<?php
require_once realpath( dirname(__FILE__ ) . '/../global-config.php');
require_once GlobalConfig::SERVER_ROOT_DIR.'controller/GeneralRequestState.php';

/** \class LangNavigator
 * This class handle the par "lang" of the navigator in the html file
 * Containing roughly three flags buttons in a div
 *
 * <div id="langnavigator">
 *	$htmlcontent
 * </div>
 * 
 * All html content of this DIV is stored in the member variable $htmlcontent	
 */

class LangNavigator
{
	public $htmlcontent;
	
	public function __construct(&$_general_request , $nbIndent=0)
	{
		$indent = '';
		for($i=0; $i< $nbIndent ; $i++)
		{
			$indent .= '	';
		}
		
		$this->htmlcontent='';
		$this->htmlcontent .= $indent.'<div id="langnavigator">'.PHP_EOL;
		$this->htmlcontent .= $indent.'	<table>'.PHP_EOL;
		$this->htmlcontent .= $indent.'		<tr>'.PHP_EOL;

		if( $_general_request->_isMobile )
		{
			$this->htmlcontent .= $indent.'			<td><a id="en" class="langbutton" href="#whole-page"><img class="langbutton" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/icon_en.png"/></a></td>'.PHP_EOL;
			$this->htmlcontent .= $indent.'			<td><a id="fr" class="langbutton" href="#whole-page"><img class="langbutton" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/icon_fr.png"/></a></td>'.PHP_EOL;
			$this->htmlcontent .= $indent.'			<td><a id="vn" class="langbutton" href="#whole-page"><img class="langbutton" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/icon_vn.png"/></a></td>'.PHP_EOL;
		}
		else
		{
			$this->htmlcontent .= $indent.'			<td><a id="en" class="langbutton" href="#"><img class="langbutton" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/icon_en.png"/></a></td>'.PHP_EOL;
			$this->htmlcontent .= $indent.'			<td><a id="fr" class="langbutton" href="#"><img class="langbutton" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/icon_fr.png"/></a></td>'.PHP_EOL;
			$this->htmlcontent .= $indent.'			<td><a id="vn" class="langbutton" href="#"><img class="langbutton" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/icon_vn.png"/></a></td>'.PHP_EOL;
		}
		$this->htmlcontent .= $indent.'		</tr>'.PHP_EOL;
		$this->htmlcontent .= $indent.'	</table>'.PHP_EOL;
		$this->htmlcontent .= $indent.'</div>'.PHP_EOL;
		
		//Printing logo if this is not on mobile
		if( !$_general_request->_isMobile )
		{
			$this->htmlcontent .= $indent.'<div align="center">'.PHP_EOL;
			$this->htmlcontent .= $indent.'	<img id="logo" src="'. GlobalConfig::DOMAINE_NAME . 'model/htmldata/image/ctn.jpg"/>'.PHP_EOL;
			$this->htmlcontent .= $indent.'</div>'.PHP_EOL;
		}
	}
}

?>