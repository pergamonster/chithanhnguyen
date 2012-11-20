<?php
require_once realpath( dirname(__FILE__ ) . '/../global-config.php');
require_once GlobalConfig::SERVER_ROOT_DIR.'controller/GeneralRequestState.php';

/** controller helpers
 */

class ControllerHelper
{
	public static function getDataNavigatorListFile()
	{
		$datafilename .= GlobalConfig::SERVER_ROOT_DIR.'controller/data/NavigatorList.xml';
		return $datafilename;
	}

	/* Request is good if and only if satisfying
	 *  - lang must be 'en' or 'fr' or 'vn'
	*  - content must be listed in the data/NavigatorList.xml
	*/
	public static function isGoodRequest(&$_general_request)
	{
		$isgood = true;

		if( ($_general_request->_lang_state !=='en') && ($_general_request->_lang_state !=='fr') && ($_general_request->_lang_state !=='vn') )
		{
			$isgood = false;
		}

		if($isgood)//lang is already good
		{
			$datafilename = self::getDataNavigatorListFile();
				
			$xmlDoc = new DOMDocument;
			$xmlDoc->load($datafilename);

			$langnode   = $xmlDoc->getElementsByTagName($_general_request->_lang_state)->item(0);
			$buttonlist = $langnode->getElementsByTagName('navigatebutton');
				
			foreach($buttonlist as $button)
			{
				$buttonID = $button->getElementsByTagName('idNAME')->item(0);
				if($buttonID)
				{
					if($buttonID->nodeValue === $_general_request->_lang_state){
						$isgood = false;
					}
				}
			}
		}
		return $isgood;
	}
}

?>