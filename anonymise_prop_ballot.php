<?php

require("init.inc");

if(isset($_POST['user_id']) && isset($_POST['prop_id']) && isset($_POST['ballot'])){
	$active_user_code = db_anon::get_active_user_code($_POST['user_id']);
	$ballot_decoded = decrypt_ballot($_POST['ballot']);
	if($active_user_code == $ballot_decoded['user_code']){
		if($ballot_decoded['user_id'] == $_POST['user_id']){
			$post_vars = Array("user_code" => $active_user_code, "prop_id" => $_POST['prop_id'], "rsa" => $ballot_decoded['rsa'], "aes" => $ballot_decoded['aes']);
			$reply = curl_to_main_server("receive_prop_ballot.php", $post_vars);
			echo $reply == "success" ? "success" : "failed - reply from receive_ballot: " . $reply;
		}
		else echo "failed - user_id mismatch";
	}
	else echo "failed - user code error: stored in ballot was " . $ballot_decoded['user_code'] . " but expected " . $active_user_code;
}
else echo "user_id, prop_id or ballot was not posted";

?>