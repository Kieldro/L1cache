#!/usr/bin/perl
open(in, @ARGV[0]);
$counter=0;
$line=<in>;$counter++;chomp($line);

$line=<in>;$counter++;chomp($line);

$line=<in>;$counter++;chomp($line);
@arr=split(/\s+/,$line);
$size=@arr;
if($size!=6)
  {
    print "Error in line $counter\n";
    exit;
  }

print "Checker thinks Total misses = $arr[1]\n";
print "Checker thinks Read misses = $arr[3]\n";
print "Checker thinks Write misses = $arr[5]\n";

$line=<in>;$counter++;chomp($line);
$line=<in>;$counter++;chomp($line);

@arr=split(/\s+/,$line);
$size=@arr;
if($size!=6)
  {
    print "Error in line $counter\n";
    exit;
  }

print "Checker thinks Total miss rate = $arr[1]\n";
print "Checker thinks Read miss rate = $arr[3]\n";
print "Checker thinks Write miss rate = $arr[5]\n";

$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;


print "Checker thinks number of dirty blocks evicted = $arr[$size-1]\n";

$line=<in>;$counter++;chomp($line);
$line=<in>;$counter++;chomp($line);
$line=<in>;$counter++;chomp($line);


$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;


print "\nChecker thinks first output line of cache contents is:\n";
print "Set=$arr[0] V=$arr[1] Tag=$arr[2] D=$arr[3] Word0=$arr[4] ";
for($i=1;$i < $size-4;$i++)
 {
   print " Word$i=$arr[$i+4]"; 
 }
print "\n";


$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;


print "\nChecker thinks second output line of cache contents is:\n";
print "Set=$arr[0] V=$arr[1] Tag=$arr[2] D=$arr[3] Word0=$arr[4] ";
for($i=1;$i < $size-4;$i++)
 {
   print " Word$i=$arr[$i+4]"; 
 }
print "\n";


$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;


print "\nChecker thinks third output line of cache contents is:\n";
print "Set=$arr[0] V=$arr[1] Tag=$arr[2] D=$arr[3] Word0=$arr[4] ";
for($i=1;$i < $size-4;$i++)
 {
   print " Word$i=$arr[$i+4]"; 
 }
print "\n";




$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;


print "\nChecker thinks fourth output line of cache contents is:\n";
print "Set=$arr[0] V=$arr[1] Tag=$arr[2] D=$arr[3] Word0=$arr[4] ";
for($i=1;$i < $size-4;$i++)
 {
   print " Word$i=$arr[$i+4]"; 
 }
print "\n";



$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;


print "\nChecker thinks fifth output line of cache contents is:\n";
print "Set=$arr[0] V=$arr[1] Tag=$arr[2] D=$arr[3] Word0=$arr[4] ";
for($i=1;$i < $size-4;$i++)
 {
   print " Word$i=$arr[$i+4]"; 
 }
print "\n";





$line=<in>;$counter++;chomp($line);
while(!($line=~/MEMORY/))
 {
  $line=<in>;$counter++;chomp($line);
 }
  
$line=<in>;$counter++;chomp($line);

$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;
if($size!=9)
 {
   print "too many or too few values in line = $counter\n";
 }

print "\nChecker thinks first output line of memory contents is:\n";
print "Address= $arr[0] Word0=$arr[1] Word1=$arr[2] Word2=$arr[3] Word3=$arr[4] Word4=$arr[5] Word5=$arr[6] Word6=$arr[7] Word7=$arr[8]\n";



$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;

if($size!=9)
 {
   print "too many or too few values in line = $counter\n";
 }

print "\nChecker thinks second output line of memory contents is:\n";
print "Address= $arr[0] Word0=$arr[1] Word1=$arr[2] Word2=$arr[3] Word3=$arr[4] Word4=$arr[5] Word5=$arr[6] Word6=$arr[7] Word7=$arr[8]\n";


$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;

if($size!=9)
 {
   print "too many or too few values in line = $counter\n";
 }

print "\nChecker thinks third output line of memory contents is:\n";
print "Address= $arr[0] Word0=$arr[1] Word1=$arr[2] Word2=$arr[3] Word3=$arr[4] Word4=$arr[5] Word5=$arr[6] Word6=$arr[7] Word7=$arr[8]\n";



$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;

if($size!=9)
 {
   print "too many or too few values in line = $counter\n";
 }

print "\nChecker thinks fourth output line of memory contents is:\n";
print "Address= $arr[0] Word0=$arr[1] Word1=$arr[2] Word2=$arr[3] Word3=$arr[4] Word4=$arr[5] Word5=$arr[6] Word6=$arr[7] Word7=$arr[8]\n";



$line=<in>;$counter++;chomp($line);
my @arr=split(/\s+/,$line);
$size=@arr;

if($size!=9)
 {
   print "too many or too few values in line = $counter\n";
 }

print "\nChecker thinks fifth output line of memory contents is:\n";
print "Address= $arr[0] Word0=$arr[1] Word1=$arr[2] Word2=$arr[3] Word3=$arr[4] Word4=$arr[5] Word5=$arr[6] Word6=$arr[7] Word7=$arr[8]\n";



