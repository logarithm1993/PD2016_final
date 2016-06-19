#!/usr/bin/perl

use Getopt::Std;
use POSIX;
use List::Util qw( min max );

our($opt_h, $opt_p);
getopts('dph:');
if(($#ARGV < 1) || ($opt_h))
{
    print "Usage:\n";
    print "   $0 <.fp> <.aux> (Example: iccad2015_evaluate.pl adaptec1.fp adaptec1.aux 0.05)\n";
    exit(0);
}

my $prefix = substr($ARGV[1], 0 , -4);
my $aux = $ARGV[1];
my $eval_aux = $prefix."Eval.aux";
my $pl_file = $prefix.".pl";
my $ntu_out_file = $prefix."Eval.ntup.pl";
my $nctu_setting_file = "ICCAD12.NCTUgr.set";
my $nctu_out_file = $prefix.".gr";

my $margin = $ARGV[2];
my $macrofile  = $ARGV[0];
my $plfile = $prefix.".pl";
my $nodefile = $prefix.".nodes";
my $netfile = $prefix.".nets";
my $wtsfile = $prefix.".wts";
my $shapefile = $prefix.".shapes";
my $sclfile = $prefix.".scl";
my $routefile = $prefix.".route";

my $out_auxfile = $prefix."Eval.aux";
my $out_plfile = $prefix."Eval.pl";
my $out_sclfile = $prefix."Eval.scl";
my $out_routefile = $prefix."Eval.route";


my %PL;
my %chip_info;

# MAIN 
#checkOverlap($macrofile, $nodefile);
readDesign(\%PL, \%chip_info, $sclfile, $plfile, $macrofile, $nodefile);
checkDesign(\%PL, \%chip_info);

writeEvalFile(\%PL,$chip_info,$nodefile,$netfile,$wtsfile,$out_plfile,$out_sclfile,$shapefile,$out_routefile, $margin);
runNTUplace3($eval_aux, \%chip_info);
runNCTUgr($eval_aux, $ntu_out_file, $nctu_setting_file, $nctu_out_file);
evalRoutability($eval_aux, $ntu_out_file, $nctu_out_file);

############################################################################
####                            sub route                               ####
############################################################################

#---------------------------------------------------------------------------------------------------------------#
#sub checkOverlap{
#    my ($fp_file, $node_file) = @_;
#    #Check Overlap
#    print "Executing command 'python check_overlap.py -f $fp_file -n $node_file\n";
#    system("python check_overlap.py -f $fp_file -n $node_file");
#}
#---------------------------------------------------------------------------------------------------------------#
sub readDesign{
    my ($plRef, $chip_info, $sclfile, $plfile, $macrofile, $nodefile) = @_;
    readScl(\%chip_info, $sclfile);
    readPl(\%plRef, $plfile);
    readPl(\%plRef, $macrofile);
    readNode(\%plRef, $nodefile);
    shiftDesign(\%plRef, \%chip_info);
    calcChipInfo(\%plRef, \%chip_info);

}
#---------------------------------------------------------------------------------------------------------------#
sub shiftDesign{
    my ($plRef, $chip_info) = @_;
    my $BLX = POSIX::DBL_MAX;
    my $BLY = POSIX::DBL_MAX;
    
    #debug
    foreach $key (keys(%{$plRef})){
        my $node_x = $plRef->{$key}->{x};
        my $node_y = $plRef->{$key}->{y};
        my $node_w = $plRef->{$key}->{w};
        my $node_h = $plRef->{$key}->{h};
        if($plRef->{$key}->{fixed} eq 'false') {
            next;
        }
        if($node_w == 0) {
            next;
        }
        if($node_h == 0) {
            next;
        }
        if( $BLX > $node_x){
            $BLX = $node_x;
        }
        if( $BLY > $node_y){
            $BLY = $node_y;
        }
    }
    foreach $key (keys(%{$plRef})){
        my $node_x = $plRef->{$key}->{x};
        my $node_y = $plRef->{$key}->{y};
        $plRef->{$key}->{x} = $node_x - $BLX + 10;
        $plRef->{$key}->{y} = $node_y - $BLY + 10;
    }
}
#---------------------------------------------------------------------------------------------------------------#
sub checkDesign{
    my ($plRef, $chip_info) = @_;

    $total_move_site = $chip_info->{sum_move_site};
    $total_macro_site = $chip_info->{sum_macro_site};
    $design_sites = ($chip_info->{TRX} - $chip_info->{BLX}) * ($chip_info->{TRY} - $chip_info->{BLY});
   #if( ($total_move_site + $total_macro_site)/$design_sites > 0.90 ){
   #    die("Check Design Failed - Design Utilization Rate %.2f exceeds 90%\n",($total_move_site + $total_macro_site)/$design_sites );
   #}
    my $invalid = checkOverlap(\%plRef);
    if($invalid != 0) {
        printf("Invalid Macro Placement -- Aborting Evaluation\n");
        exit;
    } else {
        printf("Checking Macro Placement Completed\n");
    }
    printf("Design Utilization Rate : %.2f\n",($total_move_site + $total_macro_site)/$design_sites);
    
}
#---------------------------------------------------------------------------------------------------------------#
sub checkOverlap{
    my ($pl) = @_;
    my $macro_count = 0;
    my $invalid = 0;
    my %MACROPL;
    my %COMPARELIST;
    printf("Check Rectangle Overlap ...\n");
    foreach $key (keys(%{$pl})){
        my $pt = $pl->{$key};
        if($pt->{fixed} eq 'true') {
            $MACROPL->{$key}->{x} = $pl->{$key}->{x};
            $MACROPL->{$key}->{y} = $pl->{$key}->{y};
            $MACROPL->{$key}->{w} = $pl->{$key}->{w};
            $MACROPL->{$key}->{h} = $pl->{$key}->{h};
            $MACROPL->{$key}->{fixed} = 'true';
        }
    }

    foreach $keyA (keys(%{$MACROPL})){
        my $ptA = $MACROPL->{$keyA};
        $macro_count = $macro_count + 1;
        foreach $keyB (keys(%{$MACROPL})){
            my $ptB = $MACROPL->{$keyB};
            if($keyA eq $keyB){
                next;
            }
            my $ret = rectOverlap($ptA, $ptB, $keyA, $keyB);            
            $invalid = $invalid + $ret;
        }
    }
    printf("%d Overlapping Macro Placement\n",$invalid);
    return $invalid;
} 
#---------------------------------------------------------------------------------------------------------------#
# Sub Routing to Check Overlap
#---------------------------------------------------------------------------------------------------------------#
sub rectOverlap {
    my ($ptA, $ptB, $keyA, $keyB) = @_;
    my $aBLX = $ptA->{x};
    my $aBLY = $ptA->{y};
    my $aTRX = $ptA->{x} + $ptA->{w};
    my $aTRY = $ptA->{y} + $ptA->{h};
    my $bBLX = $ptB->{x};
    my $bBLY = $ptB->{y};
    my $bTRX = $ptB->{x} + $ptB->{w};
    my $bTRY = $ptB->{y} + $ptB->{h};
    my $ret = 1;
    if ( ($aBLX >= $bTRX) || ($aTRX <= $bBLX) ) {
        $ret = 0;
    }
    if ( ($aBLY >= $bTRY) || ($aTRY <= $bBLY) ) {
        $ret = 0;
    }
    if($ret == 1)
    {
        printf("\tOverlap %s %s\n", $keyA, $keyB);
        printf("\tNode %s BBOX - (%d %d) - (%d %d)\n",$keyA, $aBLX, $aBLY, $aTRX, $aTRY);
        printf("\tNode %s BBOX - (%d %d) - (%d %d)\n",$keyB, $bBLX, $bBLY, $bTRX, $bTRY);
        printf("\n");
    }
    return $ret;
}
#---------------------------------------------------------------------------------------------------------------#
sub writeEvalFile{
    my ($plRef,$chip_info,$nodefile,$netfile,$wtsfile,$out_plfile,$out_sclfile,$shapefile,$out_routefile,$margin) = @_;
    writeAux($out_auxfile, $nodefile,$netfile,$wtsfile,$out_plfile,$out_sclfile,$shapefile,$out_routefile);
    writePl(\%plRef, $out_plfile);
    writeScl(\%chip_info, $out_sclfile,$margin);
    writeRoute(\%plRef, \%chip_info, $out_routefile,$margin);
}
#---------------------------------------------------------------------------------------------------------------#
sub runNTUplace3{
    my ($eval_aux, $chip_info) = @_;
    my $urate = $chip_info->{urate} * 1.20;
    if($urate > 0.90) {
        $urate = 0.90;
    }
    print "Desing Utilization Rate : $chip_info->{urate}\n";
    print "Executing NTUplace3 with placement density $urate\n";
    system("./ntuplace3 -aux $eval_aux -util $urate > ntuplace.log");
}
#---------------------------------------------------------------------------------------------------------------#
sub runNCTUgr{
    my ($eval_aux, $ntu_out_file, $nctu_setting_file, $nctu_out_file) = @_;
    print "Running NCTUgr...\n";
    system("./NCTUgr ICCAD $eval_aux $ntu_out_file $nctu_setting_file $nctu_out_file > nctugr.log");
}
#---------------------------------------------------------------------------------------------------------------#
sub evalRoutability{
    my ($eval_aux, $ntu_out_file, $nctu_out_file) = @_;
    print "Running Routability Evaluation...\n";
    system("./evaluate_routability.pl -p $eval_aux $ntu_out_file $nctu_out_file > eval.log");
}
#---------------------------------------------------------------------------------------------------------------#




#---------------------------------------------------------------------------------------------------------------#
# Sub Routing for Read Macro
#---------------------------------------------------------------------------------------------------------------#
sub readScl{
    my ($chip_info, $filename) = @_;
    $chip_info->{width}   = POSIX::DBL_MIN;
    $chip_info->{height}  = POSIX::DBL_MIN;
    $chip_info->{chipBLX} = POSIX::DBL_MAX;
    $chip_info->{chipBLY} = POSIX::DBL_MAX;
    $chip_info->{row_height} = -1;
    open SCLFILE, "<$filename";
    while(<SCLFILE>){
        if($_ =~ /SubrowOrigin\s+:\s+(\d+)\s+NumSites\s+:\s+(\d+)/){
            $x_left = $1;
            $x_right = $2;
            if( $chip_info->{chipBLX} > $x_left){
                $chip_info->{chipBLX} = $x_left;
            }
            if( $chip_info->{width} < $x_right - $chip_info->{chipBLX}){
                $chip_info->{width} = $x_right - $chip_info->{chipBLX};
            }
        }
        if($_ =~ /Coordinate\s+:\s+(\d+)/){
            $y = $1;
            if( $chip_info->{chipBLY} > $y){
                $chip_info->{chipBLY} = $y;
            }
            if( $chip_info->{height} < $y - $chip_info->{chipBLY}){
                $chip_info->{height} = $y - $chip_info->{chipBLY};
            }
        }
        if($_ =~ /Height\s+:\s+(\d+)/){
            $chip_info->{row_height} = $1;
        }
    }
    close SCLFILE;
    ##debug
    #printf("SCLFILE\n");
    #printf("\tchip_width  = %d\n", $chip_info->{width});
    #printf("\tchip_height = %d\n", $chip_info->{height});
    #printf("\tBLX = %d, BLY = %d\n", $chip_info->{BLX}, $chip_info->{BLY});
    #printf("\trow_height = %d\n", $chip_info->{row_height});
}
#---------------------------------------------------------------------------------------------------------------#
sub readPl{
    my ($plRef, $filename) = @_;
    open INPL, "<$filename" or die "Can not open file $filename: $!";
    $num_macro = 0;
    $num_node = 0;
    while(<INPL>){
        #x_out_9_reg_9_	981	760	:	N	
        #NumNodes     :	35291
        #NumTerminals :	3010
        # \S : Match non-whitespace character
        # \s : Match whitespace character
        # \d : Match digit character
        # +  : Match 1 or more times
        # Example (Movable) : o521465                                0             0    :  N
        # Example (Fixed)   : A2/B4/C38/E1/F1/H1/o521466          4875           126    :  N  /FIXED
        #if( $_ =~ /(\S+)\s+(\d+)\s+(\d+)\s+:\s+N/){

        if( $_ =~ /(\S+)\s+(\d+)\s+(\d+)\s+:\s+N\s+(\S+)/){
            my $node = $1;
            my $x = $2;
            my $y = $3;
            my $fixed = $4;
            $plRef->{$node}->{x} = $x;
            $plRef->{$node}->{y} = $y;
            $plRef->{$node}->{fixed} = 'true';
            $num_macro ++;
            $num_node ++;
        }
        elsif( $_ =~ /(\S+)\s+(\d+)\s+(\d+)\s+:\s+N/){
            my $node = $1;
            my $x = $2;
            my $y = $3;
            $plRef->{$node}->{x} = $x;
            $plRef->{$node}->{y} = $y;
            $plRef->{$node}->{fixed} = 'false';
            $num_node ++;
        }
    }
    close INPL;
    #printf("PLFILE\n");
    #printf("\t#node  = %d\n",$num_node);
    #printf("\t#macro = %d\n",$num_macro);
    #$CNT=0;
    ##debug
    #foreach $key (keys(%{$plRef})){
    #    printf("\t%s (%d %d)\n", $key, $plRef->{$key}->{x}, $plRef->{$key}->{y});
    #    $CNT++;
    #    if($CNT>10) {last;}
    #}
}
#---------------------------------------------------------------------------------------------------------------#
sub readNode{
    my ($nodeRef, $filename) = @_;
    open INNODE, "<$filename" or die "Can not open file $filename: $!";
    while(<INNODE>){
        # Example A1/o0             4             9
        # Example A2/B4/C38/E1/F1/H1/o521466          3700          3771    terminal

        # if( $_ =~ /(\S+)\s+(\d+)\s+(\d+)\s+terminal/){
        if( $_ =~ /(\S+)\s+(\d+)\s+(\d+)\s+/){
            my $node = $1;
            my $w = $2;
            my $h = $3;
            $nodeRef->{$node}->{w} = $w;
            $nodeRef->{$node}->{h} = $h;
        }
    }
    close INNODE;
}
#---------------------------------------------------------------------------------------------------------------#
sub calcChipInfo{
    my ($plRef, $chip_info) = @_;
    
    $chip_info->{TRX} = POSIX::DBL_MIN;
    $chip_info->{TRY} = POSIX::DBL_MIN;
    $chip_info->{BLX} = POSIX::DBL_MAX;
    $chip_info->{BLY} = POSIX::DBL_MAX;
    $chip_info->{SITESUM} = 0;
   
    $total_node = 0;
    $total_move_site = 0;
    $total_macro_site = 0;
    $total_macro  = 0;

    #debug
    foreach $key (keys(%{$plRef})){
        $node_x = $plRef->{$key}->{x};
        $node_y = $plRef->{$key}->{y};
        $node_w = $plRef->{$key}->{w};
        $node_h = $plRef->{$key}->{h};
        $total_node ++; 
        if($plRef->{$key}->{fixed} eq 'false') {
            $total_move_site += $node_w * $node_h;
            next;
        }
        if($plRef->{$key}->{fixed} eq 'true') {
            $total_macro_site += $node_w * $node_h;
        }

        if($node_w == 0) {
            next;
        }
        if($node_h == 0) {
            next;
        }
        $total_macro++;

        if( $chip_info->{BLX} > $node_x){
            $chip_info->{BLX} = $node_x;
        }
        if( $chip_info->{BLY} > $node_y){
            $chip_info->{BLY} = $node_y;
        }
        if( $chip_info->{TRX} < ($node_x + $node_w) ){
            $chip_info->{TRX} = $node_x + $node_w;
        }
        if( $chip_info->{TRY} < ($node_y + $node_h) ){
            $chip_info->{TRY} = $node_y + $node_h;
        }
    }
    $chip_info->{sum_move_site}  = $total_move_site;
    $chip_info->{sum_macro_site} = $total_macro_site;
    $design_sites = ($chip_info->{TRX} - $chip_info->{BLX}) * ($chip_info->{TRY} - $chip_info->{BLY});
    $chip_info->{urate} = (($total_move_site + $total_macro_site)/$design_sites );

    printf("Num. Nodes             : %d\n", $total_node);
    printf("Num. Macros            : %d\n", $total_macro);
    printf("Total Move Site        : %d\n", $total_move_site);
    printf("Total Macro Site       : %d\n", $total_macro_site);
    printf("Total Site             : %d\n", $total_move_site + $total_macro_site);
    printf("Macro Placement BBox   : (%d %d) - (%d %d)\n", $chip_info->{BLX}, $chip_info->{BLY}, $chip_info->{TRX}, $chip_info->{TRY});
    printf("Given Design BBox      : (%d %d) - (%d %d)\n", $chip_info->{chipBLX}, $chip_info->{chipBLY},
                                                           $chip_info->{chipBLX} + $chip_info->{width},
                                                           $chip_info->{chipBLY} + $chip_info->{height});
    printf("Est. Design Sites      : %d\n", $design_sites);
    printf("Total Site/Design Site : %.2f\n",($total_move_site + $total_macro_site)/$design_sites );
}
#---------------------------------------------------------------------------------------------------------------#
sub writeAux{
    my ($aux_file, $node_file, $net_file, $wts_file, $pl_file, $scl_file, $shape_file, $route_file) = @_;
    
    $datestring = localtime();
    open(my $OUTFILE, '>', $aux_file) or die "Could not open file '$aux_file' $!";
    printf $OUTFILE "RowBasedPlacement : $node_file $net_file $wts_file $pl_file $scl_file $shape_file $route_file\n";
    close $OUTFILE;
    printf("Generating .aux file   : %s\n",$aux_file);
}
#---------------------------------------------------------------------------------------------------------------#
sub writePl{
    my ($pl, $out_file) = @_;
    
    $datestring = localtime();
    open(my $OUTFILE, '>', $out_file) or die "Could not open file '$out_file' $!";
    printf $OUTFILE "UCLA pl 1.0\n";
    printf $OUTFILE "# Created    :   $datestring\n";
    printf $OUTFILE "# User       :   Generated .pl File for ICCAD2015 Contest\n";
    printf $OUTFILE "\n";

    #foreach $key (sort keys %pl){
    foreach $key (sort keys(%{$pl})){
        my $pt = $pl->{$key};
        if($pt->{fixed} eq 'true') {
            printf $OUTFILE "%s %12d %12d    :  N   /FIXED\n", $key, $pt->{x}, $pt->{y};
        }
        else {
            printf $OUTFILE "%s %12d %12d    :  N\n", $key, $pt->{x}, $pt->{y};
        }
    }
    close $OUTFILE;
    printf("Generating .pl file    : %s\n",$out_file);
} 
#---------------------------------------------------------------------------------------------------------------#
sub writeScl{
    my ( $chip_info, $out_file, $margin) = @_;
    
    $row_height = 12;
    $num_row = ceil( (($chip_info->{TRY} - $chip_info->{BLY})/12 )*1.02);
    $subrow_origin = floor( $chip_info->{BLX} * (1.00 - $margin) );
    $num_site = ceil( ($chip_info->{TRX} - $chip_info->{BLX}) * (1.00 + $margin) );
    $miny = floor($chip_info->{BLY} * (1.00 - $margin));
    if($miny < 0) {
        $miny = 0;
    }

    $datestring = localtime();
    open(my $OUTFILE, '>', $out_file) or die "Could not open file '$out_file' $!";
    printf $OUTFILE "UCLA scl 1.0\n";
    printf $OUTFILE "# Created    :   $datestring\n";
    printf $OUTFILE "# User       :   Generated .scl File for ICCAD2015 Contest\n";
    printf $OUTFILE "\n";
    printf $OUTFILE "NumRows : $num_row\n";
    printf $OUTFILE "\n";
    for(my $iRow = 0; $iRow < $num_row; $iRow++) {
        $coordinate = $miny + $iRow * 12;
        printf $OUTFILE "CoreRow Horizontal\n";
        printf $OUTFILE "  Coordinate    :     $coordinate\n";
        printf $OUTFILE "  Height        :     12\n";
        printf $OUTFILE "  Sitewidth     :     1\n";
        printf $OUTFILE "  Sitespacing   :     1\n";
        printf $OUTFILE "  Siteorient    :     1\n";
        printf $OUTFILE "  Sitesymmetry  :     1\n";
        printf $OUTFILE "  SubrowOrigin  :     $subrow_origin    NumSites  :  $num_site\n";
        printf $OUTFILE "END\n";
    }
    close $OUTFILE;
    printf("Generating .scl file   : %s with marging : %f\n",$out_file, $margin);
} 
#---------------------------------------------------------------------------------------------------------------#
sub writeRoute{
    my ($pl, $chip_info, $out_file, $margin) = @_;

    my $margin_route = $margin + 0.03;
    $tile_w = 40;
    $tile_h = 40;
    $datestring = localtime();
    open(my $OUTFILE, '>', $out_file) or die "Could not open file '$out_file' $!";
    $grid_x = ceil( (($chip_info->{TRX} - $chip_info->{BLX})/$tile_w ) * (1.00 + $margin_route) );
    $grid_y = ceil( (($chip_info->{TRY} - $chip_info->{BLY})/$tile_h ) * (1.00 + $margin_route) );
    #$grid_x = ceil( ($chip_info->{TRX} / $tile_w ) * (1.00 + $margin) );
    #$grid_y = ceil( ($chip_info->{TRY} / $tile_h ) * (1.00 + $margin) );
    $grid_z = 9;
    $num_macro = 0;
    
    $miny = ceil($chip_info->{BLY} * (1.00 - 1*$margin_route));
    if($miny < 0) {
        $miny = 0;
    }
    
    $minx = ceil($chip_info->{BLX} * (1.00 - 1*$margin_route));
    if($minx < 0) {
        $minx = 0;
    }

    printf $OUTFILE "route 1.0\n";
    printf $OUTFILE "# Created    :   $datestring\n";
    printf $OUTFILE "# User       :   Generated .route File for ICCAD2015 Contest\n";
    printf $OUTFILE "\n";
    printf $OUTFILE "Grid                       :    $grid_x    $grid_y    $grid_z\n";
    printf $OUTFILE "VerticalCapacity           :    0    40    0    40    0    44    0    48    0\n";    
    printf $OUTFILE "HorizontalCapacity         :    0    0    40    0    44    0    44    0    48\n";
    printf $OUTFILE "MinWireWidth               :    1    1    1    1    2    2    2    4    4\n";   
    printf $OUTFILE "MinWireSpacing             :    1    1    1    1    2    2    2    4    4\n";    
    printf $OUTFILE "ViaSpacing                 :    0    0    0    0    0    0    0    0    0\n";
    printf $OUTFILE "GridOrigin                 :    %d   %d\n", $minx, $miny;
    printf $OUTFILE "TileSize                   :    %d   %d\n",$tile_w, $tile_h;
    printf $OUTFILE "BlockagePorosity           :    0\n";
    printf $OUTFILE "\n";
    printf $OUTFILE "NumNiTerminals  :  0\n";
    printf $OUTFILE "\n";
    foreach $key (keys(%{$pl})){
        my $pt = $pl->{$key};
        if($pt->{w} == 0) {
            next;
        }
        if($pt->{h} == 0) {
            next;
        }
        if($pt->{fixed} eq 'true') {
            $num_macro ++ ;
        }
    }
    printf $OUTFILE "NumBlockageNodes  :  %d\n", $num_macro;
    printf $OUTFILE "\n";
    foreach $key (keys(%{$pl})){
        my $pt = $pl->{$key};
        if($pt->{w} == 0) {
            next;
        }
        if($pt->{h} == 0) {
            next;
        }
        if($pt->{fixed} eq 'true') {
            printf $OUTFILE "\t%s    4    1    2    3    4\n",$key;
        }
    }
    printf $OUTFILE "\n";
    printf $OUTFILE "NumEdgeCapacityAdjustments  :  0\n";
    printf $OUTFILE "\n";
    close $OUTFILE;
    printf("Generating .route file : %s with marging %f\n",$out_file, $margin_route);
}
#---------------------------------------------------------------------------------------------------------------#
sub calcScale{
    my ($PlaneSize) = @_;
    if ($PlaneSize>10000)    {return 0.015;}
    elsif($PlaneSize>5000) {return 0.03; }
    elsif($PlaneSize>2500) {return 0.05; }
    elsif($PlaneSize>1000) {return 0.08; }
    elsif($PlaneSize>500 ) {return 0.15; }
    return 0.3;   
}
#---------------------------------------------------------------------------------------------------------------#
sub initPS{
    my ($chip_info) = @_;
    my $scale = calcScale( max($chip_info->{width}, $chip_info->{height}) );
    my $scaleFont = 10;
    my $shiftX = $chip_info->{BLX} + 2*$chip_info->{row_height};
    my $shiftY = $chip_info->{BLY} + 2*$chip_info->{row_height};
    printf OUTFILE "/inch {1 mul} def\n";
    printf OUTFILE "%.8f %.8f scale\n", $scale, $scale;
    printf OUTFILE "%d %d translate\n", $shiftX, $shiftY;
    printf OUTFILE "/Times-Roman findfont\n";
    printf OUTFILE "%d scalefont\n", $scaleFont;
    printf OUTFILE "setfont\n";

    printf OUTFILE "/param_begin{\n  mark\n}def\n";
    printf OUTFILE "/param_end{\n";
    printf OUTFILE "  /argc counttomark 1 add def\n";
    printf OUTFILE "  argc 3 ge {\n";
    printf OUTFILE "  argc -1 3 { -1 roll def}for\n";
    printf OUTFILE "  }if\n";
    printf OUTFILE "pop\n}def\n";
}
#---------------------------------------------------------------------------------------------------------------#
sub defineSeg
{
    print OUTFILE "/seg{param_begin /x1 /y1 /x2 /y2 /linew /r /g /b param_end\n";
    print OUTFILE "newpath\n";
    print OUTFILE "x1 y1 moveto\n";
    print OUTFILE "x2 y2 lineto\n";
    print OUTFILE "linew setlinewidth\n";
    print OUTFILE "r g b setrgbcolor\n";
    print OUTFILE "stroke\n";
    print OUTFILE "}def\n";
}
#---------------------------------------------------------------------------------------------------------------#
sub drawSeg
{
    my ($pt1,$pt2,$line_set) = @_;
    printf OUTFILE "%d %d %d %d %.1f %.2f %.2f %.2f fbox\n",
    $pt1->{x}, $pt1->{y}, $pt2->{x}, $pt2->{y}, 
    $line_set->{width}, 
    $line_set->{r}, $line_set->{g}, $line_set->{b};
}
#---------------------------------------------------------------------------------------------------------------#
sub writeCellMovement{
    my ($pl1, $pl2, $chip_info) = @_;
    open OUTFILE, ">cellMovement.ps"; 
    initPS($chip_info);
    defineSeg();
    my %line_set= (
        "width" => "0.2",
        "r" => "1.0",
        "g" => "0.0",
        "b" => "0.0",
    );
    $CNT = 0;
    foreach $key (keys(%{$pl1})){
        ##debug
        #if ($CNT<10) {
        #    printf("%s (%d %d) (%d %d)\n", $key, $pl1->{$key}->{x}, $pl1->{$key}->{y}, $pl2->{$key}->{x}, $pl2->{$key}->{x});
        #}
        #$CNT++;
        my $pt1 = $pl1->{$key};
        my $pt2 = $pl2->{$key};
        drawSeg($pt1, $pt2, \%line_set);
    }
    close OUTFILE;
}
#---------------------------------------------------------------------------------------------------------------#
sub writeDisplaceDAT{
    my ($pl1, $pl2, $chip_info) = @_;
    open OUTFILE, ">displacement.dat";
    my $TtlDisplacement = 0;
    foreach $key (keys(%{$pl1})){
        my $pt1 = $pl1->{$key};
        my $pt2 = $pl2->{$key};
        my $displace_x = $pt1->{x} - $pt2->{x};
        my $displace_y = $pt1->{y} - $pt2->{y};
        $displacement = abs($displace_x) + abs($displace_y);
        $TtlDisplacement += $displacement;
        print OUTFILE $key." ".$displacement."\n";
    }
    print "Total displacement = $TtlDisplacement\n";
    close OUTFILE;
} 
#---------------------------------------------------------------------------------------------------------------#
