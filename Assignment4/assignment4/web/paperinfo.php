<?php
// Initialize Smarty.
error_reporting(E_ALL);
ini_set('display_errors', 1);
$topdir = realpath(".");
include 'smarty/Smarty.class.php';
$smarty = new Smarty;
$smarty->setTemplateDir($topdir . '/smarty/templates');
$smarty->setCompileDir($topdir . '/smarty/templates_c');
$smarty->setCacheDir($topdir . '/smarty/cache');
$smarty->setConfigDir($topdir . '/smarty/configs');
//$smarty->assign('paperid', $_GET['id'];);
//$smarty->assign('new1', $_GET['new'];);
// Load config and assign it to Smarty variables.
include 'config.php';
$smarty->assign('phpbase', $WEB_BASEPHP);
$smarty->assign('cgibase', $WEB_BASECGI);
$smarty->assign('hotelgwaddress', $HOTELGW_ADDRESS);
$smarty->assign('hotelgwport', $HOTELGW_PORT);
$smarty->assign('paperaddress', $PAPER_ADDRESS);
$smarty->assign('paperport', $PAPER_PORT);

$smarty->display('extends:tpl/paperinfo.html');
?>