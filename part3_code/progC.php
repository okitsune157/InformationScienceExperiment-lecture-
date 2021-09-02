<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>実装Ｃの結果</title>
	</head>
	<body>
<?php
//実装Ｃをこのファイルに実装してください。

// Create connection
$servername = "localhost";
$username = "shizutaro";
$password = "";//パスワード
$DBName = "CSexp1DB";
$mysqli = new mysqli($servername, $username, $password, $DBName);

// Check connection
if ($mysqli->connect_error) {
	//die("Connection failed: " . $conn->connect_error);
	echo $mysqli->connect_error;
	exit();
}else{
	$mysqli->set_charset("utf8");
}
//以下はダミーコードです。
if(isset($_REQUEST["tag"])){
	echo "[Program B] Input tag = [".$_REQUEST["tag"]."]";
}else{
	echo "[Program B] Please input a tag.";
}

$tag = $_REQUEST["tag"];
$sql="SELECT * FROM geotag WHERE id IN (SELECT id FROM tag WHERE tag= ? ) ORDER BY time DESC LIMIT 100";

if ($stmt = $mysqli->prepare($sql)) {
	// 条件値をSQLにバインドする（補足参照）
	//echo "$zip,$kana,$addr";
	$stmt->bind_param("s", $tag);

	//     // 実行s
	$stmt->execute();

	// 取得結果を変数にバインドする
	$stmt->bind_result($id,$time,$latitude,$longitude,$url);

	printf("<table class=\"table table-striped mt-8\">");
	printf("<tr><td>撮影時刻</td><td>緯度</td><td>経度</td><td>画像</td></tr>");
	while ($stmt->fetch()) {
		echo "<tr>";
		echo "<td>"."$time". "</td>";
		echo "<td>" ." $latitude". "</td>";
		echo "<td>" . " $longitude"."</td>"; 
		echo "<td>" . '<img src="'.$url.'" height="300" width="400" />'."</td>"; 
		echo "</tr>";
	}
	printf("</table>");

}
?>
	</body>
</html>