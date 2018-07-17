<?php
require_once 'db_conn.php';

$userid = $_REQUEST["userid"];
$name = $_REQUEST["name"];
$status = $_REQUEST["status"];
$level = $_REQUEST["level"];
$exp = $_REQUEST["exp"];
$current_vital = $_REQUEST["current_vital"];
$vital = $_REQUEST["vital"];
$str = $_REQUEST["str"];
$tgh = $_REQUEST["tgh"];
$dex = $_REQUEST["dex"];

$conn = mysql_conn();

$result=mysqli_query($conn,
"update pet set 
	level=$level,
	exp=$exp,
	current_vital = $current_vital,
	vital = $vital,
	str=$str,
	tgh = $tgh,
	dex = $dex,
	status = '$status'
 where char_id = concat('Pet_','$userid') 
 and pet_name = '$name'");

if(!$result){
	echo "{Success:false}";
	mysqli_close($conn);
	exit;
}

echo "{Success:true}";
mysqli_commit($conn);
mysqli_close($conn);

?>