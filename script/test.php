<?php 





$fp = stream_socket_client("tcp://127.0.0.1:8888", $errno, $errstr);

while(true){
    $ip = long2ip(mt_rand() + mt_rand());
	print_r($ip."\n");
	$body = pack('N',strlen($ip));
    $body .= pack('a*',$ip);
    fwrite($fp,$body);
    $len = unpack('Nlen',fread($fp,4));
    $body = unpack("A*addr",fread($fp,$len['len']));
    $addr = $body['addr'];
	print_r($addr."\n");
	//stream_socket_shutdown($fp, STREAM_SHUT_WR);
	//exit;
}

