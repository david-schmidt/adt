<?php

$self = $_SERVER["PHP_SELF"];
$script = realpath($_SERVER["SCRIPT_FILENAME"]);
$file = realpath(__FILE__);


$app_root =  ereg_replace(dirname($self) . "$", "", dirname($script));
/* Real path of application root. */
define('APP_ROOT', $app_root);
/* Relative URL of application root */
define('ROOT', str_replace($app_root, "", dirname($file)));
/* Real path of application lib direcitory. */
define('APP_LIB', APP_ROOT . '/WEB-INF/lib/');
/* Real path of application data directory. */
define('APP_DATA', APP_ROOT . '/WEB-INF/data/');
/* Real path of Smarty data directory. */
define('APP_SMARTY_DATA', APP_DATA . 'smarty/');

$app_php_lib = APP_LIB . "php";

/*
echo "<pre>\n";
echo "self: $self\n";
echo "script: $script\n";
echo "file: $file\n";
echo "APP_ROOT: " . APP_ROOT . "\n";
echo "ROOT: " . ROOT . "\n";
echo "APP_LIB: " . APP_LIB . "\n";
echo "APP_DATA: " . APP_DATA . "\n";
echo "APP_SMARTY_DATA: " . APP_SMARTY_DATA . "\n";
echo "APP_PHP_LIB: " . $app_php_lib . "\n";
echo "</pre>";
*/

ini_set('include_path', ini_get('include_path') . ":$app_php_lib");

require("smarty_setup.php");

?>