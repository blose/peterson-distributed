<?php
	$inputFilename = $argv[1];
	$tmpFilename = $inputFilename.".tmp";
	echo "Converting ".$argv[1]."...\n";
	$in_fd = fopen($inputFilename,"r");
	if ($in_fd === false) {
		echo "error reading file"; die;
	}
	$out_fd = fopen($tmpFilename,"w");
	
	$str = fgets($in_fd);
	fputs($out_fd, $str);
	
	fscanf($in_fd,"%lf %lf",$dt,$val);
	fprintf($out_fd,"%15.10e\t%9.7e\n",$dt,$val);
	$t = $dt;
	
	while (fscanf($in_fd,"%lf %lf",$tmp,$val) == 2) {
		$t+=$dt;
		fprintf($out_fd,"%15.10e\t%9.7e\n",$t,$val);
	}
	
	fclose($in_fd);
	fclose($out_fd);
	
?>