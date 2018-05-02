<?php
if (isset($_GET['k']))
	if (file_exists($_GET['k'].".jpg"))
	{

		echo file_get_contents($_GET['k'].".jpg");
#		unlink($base);
#		unlink($lFile);
#		unlink($imgFile);
#		unlink($outFile);
	}
