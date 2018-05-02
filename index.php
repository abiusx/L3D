<?php
/*
sudo Xvfb :1 -screen 0 1024x768x24 </dev/null &
export DISPLAY=":1"
*/
?>
<h1>L+System</h1>
<h2>Generating Stochastic Fast L-System Trees</h2>
<?php
$timeLimit=15;
if (isset($_POST['description']))
{
	echo "Please wait, this might take a while...";
	ob_flush();
	flush();
	$start=microtime(true);
	$base=tempnam(NULL,"L3D_");
	$lFile=$base.".l++";
	file_put_contents($lFile,$_POST['description']);
	$imgFile=$base.".jpg";
	$outFile=$base.".off+";
	shell_exec("ulimit -t {$timeLimit};".__DIR__."/src/meshpro '{$lFile}' '{$outFile}'");
	putenv("DISPLAY=:1");
	$str=("ulimit -t {$timeLimit};". __DIR__."/src/meshview '{$outFile}' -exit_immediately -output_image '{$imgFile}' 2>&1");
	$r=shell_exec($str);
	#echo $str;
	$totalTime=microtime(true)-$start;

	if (file_exists($imgFile))
	{
		echo "<br/><img border='1' src='./img.php?k={$base}' /><br/>";
		echo "Generated in ".$totalTime." seconds.<br/>";
		echo "Download the 3D model <a href='./img.php?m={$base}'>file</a>.<br/>";
	}
	else
	{
		echo "Error!<br/>";
	}
}

?>

<form method='post'>
Put your L or L++ description in here:
<br/>
<textarea onclick="#this.focus();this.select()" cols='80' rows='10' name='description'><?php
if (isset($_POST['description'])) echo htmlspecialchars($_POST['description']);
else
/*echo "3
22
5
F
F=FF-[&F^F^F]+[^F*&F&F*]\[^f*^f*&f*]
@";
*/
echo "#a nice decent tree!
6
25
15
fA
A=^f >(30) B\\B\\\\\B
B=[^^fL \\\\\\A L  ]
L=[^(60) [*(.3)] +(50)*(.28)]
L=[^(60)*(.3)]
L=[&(70)*(.3)]
@";
?></textarea>
<br/>
<input type='submit' />
</form>
<strong>Notice:</strong> time limit is <?php echo $timeLimit;?> seoncds, if generation exceeds, it will be stopped.<br/>

Find more samples <a href='https://github.com/abiusx/L3D/tree/master/L%2B%2B'>here</a> and <a href='https://github.com/abiusx/L3D/tree/master/L'>here</a>. Read about the format <a href='https://github.com/abiusx/L3D'>here</a>.<br/>

<!-- Piwik -->
<script type="text/javascript">
  var _paq = _paq || [];
  _paq.push(['trackPageView']);
  _paq.push(['enableLinkTracking']);
  (function() {
    var u="//buttercake.cs.virginia.edu/";
    _paq.push(['setTrackerUrl', u+'tracker.php']);
    _paq.push(['setSiteId', 1]);
    var d=document, g=d.createElement('script'), s=d.getElementsByTagName('script')[0];
    g.type='text/javascript'; g.async=true; g.defer=true; g.src=u+'trackerjs.php'; s.parentNode.insertBefore(g,s);
  })();
</script>
<noscript><p><img src="//buttercake.cs.virginia.edu/piwik/piwik.php?idsite=1" style="border:0;" alt="" /></p></noscript>
<!-- End Piwik Code -->

