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
if (isset($_GET['m']))
	if (file_exists($_GET['m'].".off+"))
	{
		header("Content-type: application/octet-steam");
		header("Content-disposition: attachment; filename=model.off+");
		readfile($_GET['m'].".off+");
	}
