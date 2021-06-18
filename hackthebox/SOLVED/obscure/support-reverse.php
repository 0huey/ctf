<?php
$k="80e32263";
$kh="6f8af44abea0";
$kf="351039f4a7b5";
$p="0UlYyJHG87EJqEz6";

function x($input,$k) {
    $len_k=strlen($k);
    $len_input=strlen($input);
    $o="";

    for($i=0; $i  <  $len_input; ) {
        for($j=0;  ($j < $len_k  &&  $i  <  $len_input);  $j++,  $i++) {
            $o.= $input{ $i } ^ $k{ $j };
        }
    }
    return $o;
}


if( @preg_match("/$kh(.+)$kf/", @file_get_contents("php://stdin"), $m)==1) {

    //~ print_r($m);

    $data = base64_decode($m[1]);

    print_r(gzuncompress(x($data, $k)));

}


?>
