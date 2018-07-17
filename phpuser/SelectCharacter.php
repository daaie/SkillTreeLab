<?php
require_once 'db_conn.php';

$userid = $_REQUEST["userid"];
$action = $_REQUEST["action"];

$conn = mysql_conn();

switch($action){

case "select":
	$result=mysqli_query($conn,
	"SELECT ci.level, ci.mf, ci.stamina, ci.power, ci.health, ci.elabo
		,ci.stamina * (select weight 
			from status 
			where state_id=1) as vital
		,ci.power * (select weight 
			from status 
			where state_id=2) as str
		,ci.health * (select weight 
			from status 
			where state_id=3) as tgh
		,ci.elabo * (select weight 
			from status 
			where state_id=4) as dex
		,ci.attr, ci.nickname
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
	Level:$row[0],
	Mf:'$row[1]',
	Stamina:$row[2],
	Power:$row[3],
	Health:$row[4],
	Elabo:$row[5],
	Vital:$row[6],
	Str:$row[7],
	Tgh:$row[8],
	Dex:$row[9],
	Attr:$row[10],
	NickName:'$row[11]'}";

	mysqli_free_result($result);
	mysqli_close($conn);

	break;

case "delete":
	$result=mysqli_query($conn,"delete from character_info where user_id='$userid'");
	if(!$result){
		echo "{Success:false}";
		mysqli_close($conn);

		exit;
		}

	echo "{Success:true}";
	mysqli_close($conn);

	break;
}
?>