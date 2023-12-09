<?php
  
    $file_name = 'Color.json';
   $jsonString = file_get_contents($file_name);
   $data = json_decode($jsonString, true);
   

    // $user='abcd_ef';
    
	if ($_SERVER["REQUEST_METHOD"] == "POST") {
	
	$data['R'] = text_input($_POST["R"]);
	$data['G'] = text_input($_POST["G"]);
	$data['B'] = text_input($_POST["B"]);
   
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