<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>実装Ａの結果</title>
	</head>
	<body>
<?php
//実装Ａをこのファイルに実装してください。

//--ここから追加--
// $cmd = "./tag_sort2 ".$_REQUEST["tag"];
//$cmd = "/home/pi/public_html/search_v3 ".$_REQUEST["tag"] ." 2>&1 umask 0000";
$cmd = "/home/pi/public_html/search_v3 ".$_REQUEST["tag"];
exec($cmd,$out, $return_ver);

//class Geotag{
//	public $id;
//	public $time;
//	public $latitude;
//    public $longitude;
//    public $url;
//}

//array_multisort($sort, SORT_ASC, $array);
// echo "<table border=1>" ;
// echo "<tr><td>id</td><td>time</td><td>URL</td><td>Where</td></tr>" ;


// $index = 0;
// foreach($out as $result){
// 	echo $out[$index];
// 	$index++;
// }


// exec($command, $output, $status);
// error_log('['.date(DATE_ATOM).'] '.$command."\n".'return code : '.$status."\n", 3, '../logs/ansible-view.log');
// foreach($output as $row){
//     error_log($row."\n", 3, '/var/log/test/test.log');
// }
//----
printf("<table class=\"table table-striped mt-8\">");
printf ("<tr><td>id</td><td>撮影時刻</td><td>撮影場所(緯度,経度)</td><td>写真</td></tr>") ;
//$count_ret = 0;
// foreach($out as $result){
// 	$sub = preg_split("/,/", $result,5 , PREG_SPLIT_NO_EMPTY);

// 	if(count($sub)==5){
// 		$geotag[$count_ret]= new Geotag;
// 		$geotag[$count_ret]->id = $sub[0];	
// 		$geotag[$count_ret]->time = $sub[1];	
// 		$geotag[$count_ret]->latitude = $sub[2];	
// 		$geotag[$count_ret]->longitude = $sub[3];	
// 		$geotag[$count_ret]->url = $sub[4];	
// 		$sort[$count_ret] = $geotag[$count_ret]->time;
//         echo "<tr>" ;
//         echo "<td>" . $geotag[$count_ret]->id. "</td>" ;
//         echo "<td>" . $geotag[$count_ret]->time . "</td>" ;
//         echo "<td><img src=" . $geotag[$count_ret]->url . "></td>" ;
//         //echo "<td>". $url . "</td>" ;
//         echo "<td> ( " . $geotag[$count_ret]->latitude . " . " . $geotag[$count_ret]->longitude . " ) </td>" ;
//         echo "</tr>" ;
//         //$index++;
// 		$count_ret++;
// 	}
// }
// echo "</table><br>" ;
//----
foreach($out as $result){
	$sub = preg_split("/,/", $result,5 , PREG_SPLIT_NO_EMPTY);

	if(count($sub)==5){
        echo "<tr>" ;
        echo "<td>" . $sub[0]. "</td>" ;
        echo "<td>" . $sub[1] . "</td>" ;
		echo "<td> ( " . $sub[2] . " . " . $sub[3] . " ) </td>" ;
        echo "<td><img src=" . $sub[4] . "></td>" ;
        echo "</tr>" ;
        //$index++;
		//$count_ret++;
	}
}
echo "</table><br>" ;

// array_multisort($sort, SORT_DESC, $geotag);

// //echo "<table border=1>" ;
// //echo "<table>"
// printf("<table class=\"table table-striped mt-8\">");
// printf ("<tr><td>id</td><td>撮影時刻</td><td>写真</td><td>撮影場所(緯度,経度)</td></tr>") ;
// $index = 0;
// if($count_ret > 100){
// 	$count_ret = 100;
// }
// while($index < $count_ret){
// 	echo "<tr>" ;
// 	echo "<td>" . $geotag[$index]->id. "</td>" ;
// 	echo "<td>" . $geotag[$index]->time . "</td>" ;
// 	echo "<td><img src=" . $geotag[$index]->url . "></td>" ;
// 	//echo "<td>". $url . "</td>" ;
// 	echo "<td> ( " . $geotag[$index]->latitude . " . " . $geotag[$index]->longitude . " ) </td>" ;
// 	echo "</tr>" ;
// 	$index++;
// }
// echo "</table><br>" ;


//--ここまで追加--

?>
	</body>
</html>