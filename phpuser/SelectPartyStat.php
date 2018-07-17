<?php
require_once 'db_conn.php';

$userid = $_REQUEST["userid"];

$conn = mysql_conn();

$result=mysqli_query($conn,
	"SELECT ci.level, ci.nickname
	FROM character_info ci
	WHERE ci.user_id='$userid'");

	$row = mysqli_fetch_row($result);
	if(!$row){
		echo "{Success:false}";
		mysqli_free_result($result);
		mysqli_close($conn);

		exit;
		}

	echo "{Success:true,
	Level:'$row[0]',
	NickName:'$row[1]'}";

	mysqli_free_result($result);
	mysqli_close($conn);
?>