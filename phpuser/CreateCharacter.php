<?php
require_once 'db_conn.php';

$userid = $_REQUEST["userid"];
$mf = $_REQUEST["mf"];
$nickname = $_REQUEST["nickname"];
$stamina = $_REQUEST["stamina"];
$power = $_REQUEST["power"];
$health = $_REQUEST["health"];
$elabo = $_REQUEST["elabo"];

$conn = mysql_conn();

$result=mysqli_query($conn,
"insert into character_info (user_id,mf,stamina,power,health,elabo,inven_id,pet_id,nickname,current_vital) values
	('$userid',
	'$mf',
	'$stamina',
	'$power',
	'$health',
	'$elabo',
	concat('In_','$userid'),
	concat('Pet_','$userid'),
	'$nickname',
	$stamina * (select weight 
			from status 
			where state_id=1)
	)");

if(!$result){
	echo "{Success:false}";
	mysqli_close($conn);
	exit;
}

echo "{Success:true}";
mysqli_close($conn);

?>