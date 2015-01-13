#!/usr/perl/bin/perl
#v_exp5

$brsltDir="57brslt/";
$dataDir="data/";
$rsltDir="57prslt/";
$frsltDir="57qrslt/";
$queryca="query_ca100";
$querytx="query_tx100";
@filePrefix=("ca250k","ca500k","ca1000k","ca1500k","ca2000k","tx250k","tx500k","tx1000k","tx1500k","tx2000k");
@m=("100","225","400","625","900","1225");
$gdo=".gdo";
$ggo=".ggo";
$cdo=".cdo";
$cgo=".cgo";
$builtrslt=".brslt";
$rsltFile1="exp5_1.rslt";
$rsltFile2="exp5_2.rslt";
$rsltFile3="exp5_3.rslt";

my $str = "echo exp5_query:vary m > $dataDir$rsltDir$rsltFile1"; 
print "$str\n";system $str;
my $str = "echo exp5_query:vary m > $dataDir$rsltDir$rsltFile2"; 
print "$str\n";system $str;
my $str = "echo exp5_query:vary m > $dataDir$rsltDir$rsltFile3"; 
print "$str\n";system $str;
for($j=2;$i<3;$i+=5)
{	
	my $str = "echo query on $filePrefix[$j] >> $dataDir$rsltDir$rsltFile1"; 
	print "$str\n";system $str;
	my $str = "echo query on $filePrefix[$j] >> $dataDir$rsltDir$rsltFile2"; 
	print "$str\n";system $str;
	my $str = "echo query on $filePrefix[$j] >> $dataDir$rsltDir$rsltFile3"; 
	print "$str\n";system $str;
	for($i=0;$i<6;$i+=1)
	{	
		if($j==2)
		{		
			my $str = "MinCs/MinCsQuery.exe $dataDir$brsltDir$filePrefix[2]$m[$i]$builtrslt $m[$i] $dataDir$queryca $dataDir$frsltDir$filePrefix[2]$m[$i]_1 >> $dataDir$rsltDir$rsltFile1"; 
			print "$str\n";system $str;
		}
		else
		{
			my $str = "MinCs/MinCsQuery.exe $dataDir$brsltDir$filePrefix[7]$m[$i]$builtrslt $m[$i] $dataDir$querytx $dataDir$frsltDir$filePrefix[7]$m[$i]_1 >> $dataDir$rsltDir$rsltFile1"; 
			print "$str\n";system $str;
		}
		if($j==2)
		{		
			my $str = "MinDup/MinDupQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$gdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$ggo $dataDir$queryca $dataDir$frsltDir$filePrefix[$j]$m[$i]_2 >> $dataDir$rsltDir$rsltFile2"; 
			print "$str\n";system $str;
		}
		else
		{
			my $str = "MinDup/MinDupQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$gdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$ggo $dataDir$querytx $dataDir$frsltDir$filePrefix[$j]$m[$i]_2 >> $dataDir$rsltDir$rsltFile2"; 
			print "$str\n";system $str;
		}
		if($j==2)
		{		
			my $str = "MinMax/MinMaxQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$cdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$cgo $dataDir$queryca $dataDir$frsltDir$filePrefix[$j]$m[$i]_3 >> $dataDir$rsltDir$rsltFile3"; 
			print "$str\n";system $str;
		}
		else
		{
			my $str = "MinMax/MinMaxQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$cdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$cgo $dataDir$querytx $dataDir$frsltDir$filePrefix[$j]$m[$i]_3 >> $dataDir$rsltDir$rsltFile3"; 
			print "$str\n";system $str;
		}
	}
}
