<?php
require_once 'db_conn.php';

$userid = $_REQUEST["userid"];
$level = $_REQUEST["level"];
$attr = $_REQUEST["attr"];
$stamina = $_REQUEST["stamina"];
$power = $_REQUEST["power"];
$health = $_REQUEST["health"];
$elabo = $_REQUEST["elabo"];
$current_vital = $_REQUEST["current_vital"];
$state_pnt=$_REQUEST["state_pnt"];

$conn = mysql_conn();

$result=mysqli_query($conn,
"update character_info set 
	level = $level,
	attr = $attr,
	stamina = $stamina,
	power = $power,
	health = $health,
	elabo=$elabo,
	current_vital = $current_vital,
	state_pnt= $state_pnt
 where user_id = '$userid'");

if(!$result){
	echo "{Success:false}";
	mysqli_close($conn);
	exit;
}

echo "{Success:true}";
mysqli_commit($conn);
mysqli_close($conn);

?>