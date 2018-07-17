<?php
require_once 'db_conn.php';

$userid = $_REQUEST["userid"];
$action = $_REQUEST["action"];

$conn = mysql_conn();

mysqli_set_charset($conn , "utf-8");

switch($action){

case "character":
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
		,ci.attr, ci.exp, ci.nickname, ci.current_vital, ci.state_pnt
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
	Mf:'$row[1]',
	Stamina:'$row[2]',
	Power:'$row[3]',
	Health:'$row[4]',
	Elabo:'$row[5]',
	Vital:'$row[6]',
	Str:'$row[7]',
	Tgh:'$row[8]',
	Dex:'$row[9]',
	Attr:'$row[10]',
	Exp:'$row[11]',
	NickName:'$row[12]',
	CurrentVital:'$row[13]',
	StatePnt:'$row[14]'}";

	mysqli_free_result($result);
	mysqli_close($conn);

	break;

case "pet":
	$result=mysqli_query($conn,
	"select p.pet_id, 
		p.pet_name, 
		p.level,
		p.exp,
		p.current_vital,
		p.vital,
		p.str,
		p.tgh,
		p.dex,
		p.status,
		pb.pet_skill
	from pet p, character_info u, pet_base pb
	where u.user_id = '$userid'
	and u.pet_id = p.char_id
	and p.pet_id = pb.pet_id
	");

	while($row=mysqli_fetch_array($result)){
	$petStatusArray[] = array(
	'pet_id'=>$row[0], 
	"pet_name"=>$row[1],
	'level'=>$row[2],
	'exp'=>$row[3],
	'current_vital'=>$row[4],
	'vital'=>$row[5],
	'str'=>$row[6],
	'tgh'=>$row[7],
	'dex'=>$row[8],
	'status'=>$row[9],
	'skill'=>$row[10]
	);
	}

	echo json_encode(array('PetResults'=>$petStatusArray),JSON_UNESCAPED_UNICODE);

	mysqli_free_result($result);
	mysqli_close($conn);

	break;

}
?>