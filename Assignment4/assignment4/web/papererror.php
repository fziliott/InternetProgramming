<?php
// Initialize Smarty.
$topdir = realpath(".");
include 'smarty/Smarty.class.php';
$smarty = new Smarty;
$smarty->setTemplateDir($topdir . '/smarty/templates');
$smarty->setCompileDir($topdir . '/smarty/templates_c');
$smarty->setCacheDir($topdir . '/smarty/cache');
$smarty->setConfigDir($topdir . '/smarty/configs');
//$smarty->display('extends:tpl/index.html'>
$smarty->assign('reason', $_GET["reason"]);
$smarty->display('extends:tpl/papererror.html');
?>