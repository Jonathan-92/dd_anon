<?php

function GenerateRandomString ($length){
	$Allowed_Chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
	$random_string = "";
	for($i=0; $i<$length; $i++)
	{
		$offset = mt_rand(0,62);
		if ($offset >= strlen($Allowed_Chars))
			throw new Exception('Tried to generate string from invalid char at offset: ' . $offset);
    	$random_string .= $Allowed_Chars[$offset];
	}
	return $random_string;
}

function curl_to_main_server ($main_server_script, $post_vars = null){

	$ch = curl_init();

	curl_setopt($ch, CURLOPT_URL, MAIN_SERVER_URL . $main_server_script);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

	if($post_vars){
		$post_data = http_build_query($post_vars);

		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
	}

	$reply = curl_exec($ch);
	curl_close($ch);

	return $reply;

}

function decrypt_ballot ($ballot){

	$private_key = "-----BEGIN RSA PRIVATE KEY----- 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 -----END RSA PRIVATE KEY-----";

	// Ballot consist of RSA and AES part
	$ballot_decoded = json_decode($ballot, true);

	// decryption using phplibsec RSA library
	include('Crypt/RSA.php');
	$rsa = new Crypt_RSA();
	$rsa->loadKey($private_key);
	$rsa->setEncryptionMode(CRYPT_RSA_ENCRYPTION_PKCS1);

	$ballot_rsa_decrypted = $rsa->decrypt(base64_decode($ballot_decoded['rsa']));
	$ballot_rsa_decrypted_decoded = json_decode($ballot_rsa_decrypted, true);

	// decryption using Giberish AES library
	include('Crypt/GibberishAES.php');
	$ballot_aes_decrypted = GibberishAES::dec($ballot_decoded['aes'], $ballot_rsa_decrypted_decoded['password']);
	$ballot_aes_decrypted_decoded = json_decode($ballot_aes_decrypted, true);

	return $ballot_aes_decrypted_decoded;

}

?>