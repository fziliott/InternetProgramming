<?php
// Initialize Smarty.
ini_set('display_errors', true);

error_reporting(E_ALL);

$topdir = realpath(".");
include 'smarty/Smarty.class.php';
$smarty = new Smarty;
$smarty->setTemplateDir($topdir . '/smarty/templates');
$smarty->setCompileDir($topdir . '/smarty/templates_c');
$smarty->setCacheDir($topdir . '/smarty/cache');
$smarty->setConfigDir($topdir . '/smarty/configs');

// Load config and assign it to Smarty variables.
include 'config.php';
$smarty->assign('phpbase', $WEB_BASEPHP);
$smarty->assign('cgibase', $WEB_BASECGI);
$smarty->assign('hotelgwaddress', $HOTELGW_ADDRESS);
$smarty->assign('hotelgwport', $HOTELGW_PORT);
$smarty->assign('paperaddress', $PAPER_ADDRESS);
$smarty->assign('paperport', $PAPER_PORT);

$err = 'No error';

if ($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) {
	if (socket_connect($socket, $HOTELGW_ADDRESS, $HOTELGW_PORT)) {
		socket_read($socket, 2048);
		socket_write($socket, "l\n");
		$line = trim(socket_read($socket, 2048));
		$rooms = explode("\t", $line);

		$smarty->assign('type1', $rooms[0]);
		$smarty->assign('type2', $rooms[1]);
		$smarty->assign('type3', $rooms[2]);

		socket_close($socket);
	} else {
		$err = $err . "Cannot connect with addr $HOTELGW_ADDRESS:$HOTELGW_PORT!\n";
	}
} else {
	$err = $err . "Cannot create socket!\n";
}

$smarty->display('extends:hotellist.html');
?>
