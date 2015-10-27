<?php
// Initialize Smarty.
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
$todisplay = "participants.html";

if ($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) {
	if (socket_connect($socket, $HOTELGW_ADDRESS, $HOTELGW_PORT)) {
		socket_read($socket, 2048);
		socket_write($socket, "g\n");
		$res = socket_read($socket, 2048);
		$lines = explode("\n", $res);
		array_splice($lines, count($lines) - 1);

		$smarty->assign('customer', $lines);

		socket_close($socket);
	} else {
		$err = "Cannot connect with addr $HOTELGW_ADDRESS:$HOTELGW_PORT!\n";
	}
} else {
	$err = "Cannot contact remote server!\n";
}
if ($err != "No error") {
	$todisplay = "error.html";
	$smarty->assign('error', $err);
}
$smarty->display('extends:tpl/' . $todisplay);
?>
