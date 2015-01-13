#!/usr/perl/bin/perl
#v_exp2
$brsltDir="57brslt/";
$dataDir="data/";
$rsltDir="57prslt/";
@filePrefix=("ca250k","ca500k","ca1000k","ca1500k","ca2000k","tx250k","tx500k","tx1000k","tx1500k","tx2000k");
@m=("100","225","625","400","900","1225");
$builtrslt=".brslt";
$gdo=".gdo";
$ggo=".ggo";
$cdo=".cdo";
$cgo=".cgo";
$rsltFile1="exp2_1.rslt";
$rsltFile2="exp2_2.rslt";
$rsltFile3="exp2_3.rslt";
my $str = "echo exp2:vary P > $dataDir$rsltDir$rsltFile1"; 
print "$str\n";system $str;
my $str = "echo exp2:vary P > $dataDir$rsltDir$rsltFile2"; 
print "$str\n";system $str;
my $str = "echo exp2:vary P > $dataDir$rsltDir$rsltFile3"; 
print "$str\n";system $str;

my $str = "echo m      p_time     e_time       avg        min          max        total >> $dataDir$rsltDir$rsltFile1"; 
print "$str\n";system $str;
my $str = "echo m      p_time     e_time       avg        min          max        total >> $dataDir$rsltDir$rsltFile2"; 
print "$str\n";system $str;
my $str = "echo m      p_time     e_time       avg        min          max        total >> $dataDir$rsltDir$rsltFile3"; 
print "$str\n";system $str;

for($j=0;$j<5;$j+=1)
{	
	#my $str = "echo m=$m[$i] >> $dataDir$rsltDir$rsltFile1"; 
	#print "$str\n";system $str;
	for($i=2;$i<3;$i+=1)
	{	
		my $str = "MinCs/buildPartition.exe $dataDir$filePrefix[$j] $m[$i] $dataDir$brsltDir$filePrefix[$j]$m[$i]$builtrslt >> $dataDir$rsltDir$rsltFile1"; 
		print "$str\n";system $str;
		my $str = "MinDup/buildPartition.exe $dataDir$filePrefix[$j] $m[$i] $dataDir$brsltDir$filePrefix[$j]$m[$i]$gdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$ggo >> $dataDir$rsltDir$rsltFile2"; 
		print "$str\n";system $str;
		my $str = "MinMax/buildPartition.exe $dataDir$filePrefix[$j] $m[$i] $dataDir$brsltDir$filePrefix[$j]$m[$i]$cdo $dataDir$brsltDir$filePrefix[$j]$m[$i]$cgo >> $dataDir$rsltDir$rsltFile3"; 
		print "$str\n";system $str;
	}
}
