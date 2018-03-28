#!/usr/bin/perl -w
#

printf("#!/usr/bin/sh\n\n");
foreach my $file (<*>){
  if(($file ne "section") && ($file ne "xml") && ($file ne "AUTHORS") && ($file ne "autogen.sh") && ($file ne "ChangeLog") && ($file ne "configure.ac") && ($file ne "Makefile.am") && ($file ne "NEWS") && ($file ne "README") && ($file ne "clean.pl")){ 
    printf "if [ -f %16s ];then rm -rf %16s;fi\n",$file,$file if -f $file;
    printf "if [ -d %16s ];then rm -rf %16s;fi\n",$file,$file if -d $file;
  }
}
