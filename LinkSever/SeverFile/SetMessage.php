<?php
  
    $file_name = 'Message.json';
   $jsonString = file_get_contents($file_name);
   $data = json_decode($jsonString, true);
   

    // $user='abcd_ef';
    
	if ($_SERVER["REQUEST_METHOD"] == "POST") {
	
	$data['State'] = text_input($_POST["State"]);
	$data['Mess'] = text_input($_POST["Mess"]);
    
   
	$newJsonString = json_encode($data);
	file_put_contents($file_name, $newJsonString);
	}

	
 function text_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
   ?>  