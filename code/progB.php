<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>実装Ｂの結果</title>
	</head>
	<body>
		<?php
		//実装Ｂをこのファイルに実装してください。
		$time_start = microtime(true);
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

		// $tag = $_REQUEST["tag"];
		$tag = "fiat";
		$sql="SELECT * FROM geotag WHERE id IN (SELECT id FROM tag IGNORE INDEX(i_tag) WHERE tag= ? ) ORDER BY time DESC LIMIT 100";

		// $time_start = microtime(true);

		if ($stmt = $mysqli->prepare($sql)) {
			// $time = microtime(true) - $time_start;
			// echo "{$time} 秒";
			
			// 条件値をSQLにバインドする（補足参照）
			//echo "$zip,$kana,$addr";
			
			$stmt->bind_param("s", $tag);

			//$time_start = microtime(true);
			//     // 実行s
			$stmt->execute();

			// $time = microtime(true) - $time_start;
			// echo "{$time} 秒";

			//$time_start = microtime(true);
			// 取得結果を変数にバインドする
			$stmt->bind_result($id,$time,$latitude,$longitude,$url);

			// $time = microtime(true) - $time_start;
			// echo "{$time} 秒";


			printf("<table class=\"table table-striped mt-8\">");
			printf("<tr><td>撮影時刻</td><td>緯度</td><td>経度</td><td>画像</td></tr>");
	
			while ($stmt->fetch()) {
				echo "<tr>";
				echo "<td>"."$time". "</td>";
				// echo "<td>" ." $latitude". "</td>";
				// echo "<td>" . " $longitude"."</td>"; 
				echo "<td> ( " . $latitude . " . " . $longitude . " ) </td>" ;
				echo "<td>" . '<img src="'.$url.'" />'."</td>"; 
				echo "</tr>";
				printf("</table>");
			}
			// $time = microtime(true) - $time_start;
			// echo "{$time} 秒";
		}
		$time = microtime(true) - $time_start;
		echo "{$time} 秒";
		?>
	</body>
</html>