<?php
class db_anon {
	private static function generate_new_user_codes ($user_id){
		$active_user_code = GenerateRandomString(20);
		$passive_user_code1 = GenerateRandomString(20);
		$passive_user_code2 = GenerateRandomString(20);
		$passive_user_code3 = GenerateRandomString(20);
		$query = "insert into users (user_id, active_user_code, passive_user_code1, passive_user_code2, passive_user_code3) values (?, ?, ?, ?, ?)";
		$values = Array($user_id, $active_user_code, $passive_user_code1, $passive_user_code2, $passive_user_code3);
		return db::insert($query, $values);
	}

	private static function get_primary_key_of_user ($user_id){
		$query = "select id from users where user_id = ?";
		$values = Array($user_id);
		return db::select_single_value($query, $values);
	}

	private static function generate_user_codes_for_new_user ($user_id){
		$id = self::get_primary_key_of_user($user_id);
		return $id > 0 
			? $id 
			: self::generate_new_user_codes($user_id); 
	}

	public static function update_user_temp_code ($user_id, $temp_code){
		$id = self::generate_user_codes_for_new_user($user_id);
		if (!($id > 0)) 
			throw new Exception("Failed to generate user_codes for new user: " . $user_id);
		$query = "insert into user_temp_codes (temp_code, user_id, timestamp_created) values (?, ?, ?)";
		$values = Array($temp_code, $user_id, time());
		$id = db::insert($query, $values);
		if (!($id > 0)) 
			throw new Exception("Failed to generate temp_code for user: " . $user_id);
	}

	public static function check_temp_code_and_user_id ($temp_code, $user_id){
		$query = "select count(*) from user_temp_codes where temp_code = ? and user_id = ?";
		$values = Array($temp_code, $user_id);
		$result = db::select_single_value($query, $values);
		return $result > 0;
	}

	public static function add_user_id_to_temp_code ($user_id, $temp_code){
		$query = "update user_temp_codes set user_id = ? where temp_code = ?";
		$values = Array($user_id, $temp_code);
		return db::update($query, $values);
	}

	public static function check_user_and_constituency ($user_id, $temp_code, $constituency_id){
		$query = "select u.active_user_code from users u, user_temp_codes utc, temp_codes_constituencies tcc 
					where u.user_id = utc.user_id and utc.temp_code = tcc.temp_code and u.user_id = ? and utc.temp_code = ? and tcc.constituency_id = ?";
		$values = Array($user_id, $temp_code, $constituency_id);
		return db::select_single_value($query, $values);
	}

	public static function get_user_codes ($user_id, $temp_code){
		$query = "select active_user_code, passive_user_code1, passive_user_code2, passive_user_code3 from users u, user_temp_codes utc
					where u.user_id = utc.user_id and u.user_id = ? and utc.temp_code = ?";
		$values = Array($user_id, $temp_code);
		return db::select_single_row($query, $values);
	}

	public static function get_active_user_code ($user_id){
		$query = "select active_user_code from users where user_id = ?";
		$values = Array($user_id);
		return db::select_single_value($query, $values);
	}

	public static function update_last_login ($user_id){
		$query = "update users set timestamp_last_login = ? where user_id = ?";
		$values = Array(time(), $user_id);
		return db::update($query, $values);
	}

	public static function update_last_activity ($user_id){
		$query = "update users set timestamp_last_activity = ? where user_id = ?";
		$values = Array(time(), $user_id);
		return db::update($query, $values);
	}

	public static function delete_old_temp_codes (){
		$query = "delete from user_temp_codes where timestamp_created < ?";
		$values = Array(time() - ERASE_TEMP_CODES_AFTER_SECONDS);
		db::delete($query, $values);
		$query = "delete from temp_codes_constituencies where timestamp_created < ?";
		$values = Array(time() - ERASE_TEMP_CODES_AFTER_SECONDS);
		return db::delete($query, $values);
	}

	public static function get_passive_user_code_for_user ($active_user_code){
		$rand = mt_rand(1,3);
		$passive_user_code = "passive_user_code" . $rand;
		$query = "select " . $passive_user_code . " from users where active_user_code = ?";
		$values = Array($active_user_code);
		return db::select_single_value($query, $values);
	}

	public static function get_active_user_code_from_passive ($passive_user_code){
		$query = "select active_user_code from users where passive_user_code1 = ? or passive_user_code2 = ? or passive_user_code3 = ?";
		$values = Array($passive_user_code, $passive_user_code, $passive_user_code);
		return db::select_single_value($query, $values);
	}
}
?>