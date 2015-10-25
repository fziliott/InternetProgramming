<?php
// Initialize Smarty.
$topdir=realpath(".");
include('smarty/Smarty.class.php');
$smarty = new Smarty;
$smarty->setTemplateDir($topdir.'/smarty/templates');
$smarty->setCompileDir($topdir.'/smarty/templates_c');
$smarty->setCacheDir($topdir.'/smarty/cache');
$smarty->setConfigDir($topdir.'/smarty/configs');
$smarty->display('extends:tpl/index.html'>
$smarty->assign('paperid', $_GET["id"];);
$smarty->assign('new_', $_GET["new"];);
$smarty->display('extends:tpl/paperinfo.html');

?>