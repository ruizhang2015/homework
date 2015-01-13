#!/usr/perl/bin/perl
#v_exp6

$queryca="query_ca100";
$querytx="query_tx100";
$frsltDir="57qrslt/";
$brsltDir="57brslt/";
$dataDir="data/";
$rsltDir="57prslt/";
@filePrefix=("ca250k","ca500k","ca1000k","ca1500k","ca2000k","tx250k","tx500k","tx1000k","tx1500k","tx2000k");
@m=("100","225","400","625","900","1225");
$builtrslt=".brslt";
$gdo=".gdo";
$ggo=".ggo";
$cdo=".cdo";
$cgo=".cgo";
$rsltFile1="exp6_1.rslt";
$rsltFile2="exp6_2.rslt";
$rsltFile3="exp6_3.rslt";
my $str = "echo exp6:vary P > $dataDir$rsltDir$rsltFile1"; 
print "$str\n";system $str;
my $str = "echo exp6:vary P > $dataDir$rsltDir$rsltFile2"; 
print "$str\n";system $str;
my $str = "echo exp6:vary P > $dataDir$rsltDir$rsltFile3"; 
print "$str\n";system $str;

my $str = "echo query on CA >> $dataDir$rsltDir$rsltFile1"; 
print "$str\n";system $str;
my $str = "echo query on CA >> $dataDir$rsltDir$rsltFile2"; 
print "$str\n";system $str;
my $str = "echo query on CA >> $dataDir$rsltDir$rsltFile3"; 
print "$str\n";system $str;
for($j=0;$j<5;$j+=1)
{	
	for($i=3;$i<4;$i+=1)
	{	
		my $str = "MinCs/MinCsQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$builtrslt $m[$i] $dataDir$queryca $dataDir$frsltDir$filePrefix[$j]$m[$i]_1 >> $dataDir$rsltDir$rsltFile1"; 
		print "$str\n";system $str;
		my $str = "MinDup/MinDupQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$gdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$ggo $dataDir$queryca $dataDir$frsltDir$filePrefix[$j]$m[$i]_2 >> $dataDir$rsltDir$rsltFile2"; 
		print "$str\n";system $str;
		my $str = "MinMax/MinMaxQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$cdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$cgo $dataDir$queryca $dataDir$frsltDir$filePrefix[$j]$m[$i]_3 >> $dataDir$rsltDir$rsltFile3"; 
		print "$str\n";system $str;
	}
}

=comment  description
my $str = "echo query on TX >> $dataDir$rsltDir$rsltFile1"; 
print "$str\n";system $str;
my $str = "echo query on TX >> $dataDir$rsltDir$rsltFile2"; 
print "$str\n";system $str;
my $str = "echo query on TX >> $dataDir$rsltDir$rsltFile3"; 
print "$str\n";system $str;
for($j=5;$j<10;$j+=1)
{	
	for($i=2;$i<3;$i+=1)
	{	
		my $str = "grid/gridQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$builtrslt $m[$i] $dataDir$querytx $dataDir$frsltDir$filePrefix[$j]$m[$i]_1 >> $dataDir$rsltDir$rsltFile1"; 
		print "$str\n";system $str;
		my $str = "greedy/greedyQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$gdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$ggo $dataDir$querytx $dataDir$frsltDir$filePrefix[$j]$m[$i]_2 >> $dataDir$rsltDir$rsltFile2"; 
		print "$str\n";system $str;
		my $str = "cutMax/cutMaxQuery.exe $dataDir$brsltDir$filePrefix[$j]$m[$i]$cdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$cgo $dataDir$querytx $dataDir$frsltDir$filePrefix[$j]$m[$i]_3 >> $dataDir$rsltDir$rsltFile3"; 
		print "$str\n";system $str;
	}
}
=cut
