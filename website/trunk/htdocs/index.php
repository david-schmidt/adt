<?php

require('setup.php');

$smarty = createSmarty();
$smarty->assign('title', "Apple Disk Transfer");
$smarty->layout('index.markdown');

?>