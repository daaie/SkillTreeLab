<?php

function mysql_conn(){
	$conn=mysqli_connect('192.168.0.115', 'pda', 'pda', 'StoneAge');
	if(!$conn){
	    die('Connect Failed:'.mysql_error().'<br />');
	}
	return $conn;
}

?>